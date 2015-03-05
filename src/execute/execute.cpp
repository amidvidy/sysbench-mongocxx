#include "execute.hpp"

#include <chrono>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/options/find.hpp>
#include <mongocxx/pipeline.hpp>

#include "data.hpp"
#include "metrics.hpp"

namespace sysbench {
namespace execute {

    using namespace bsoncxx::builder::stream;

    execute_phase::execute_phase(options opts)
        : _done{false}
        , _opts{std::move(opts)} {

        for (std::size_t i = 0; i < _opts.writer_threads; ++i) {
            _workers.emplace_back(i, &_opts, this);
        }
    }

    void execute_phase::run(metrics::collector* collector) {
        std::vector<std::thread> threads;
        std::cout << "starting execute" << std::endl;
        for (auto&& worker : _workers) {
            threads.emplace_back([&worker, collector]() { worker.work(collector); });
        }
        std::cout << "threads running" << std::endl;


        std::this_thread::sleep_for(std::chrono::seconds{20});
        _done.store(true);
        
        // TODO: use something better like a barrier.
        for (auto&& thread : threads) {
            thread.join();
        }
    }

    worker::worker(uint32_t worker_id, options* opts, execute_phase* phase)
        : _id{worker_id}
        , _client{}
        , _opts{std::move(opts)}
        , _phase{std::move(phase)} {
    }

    void worker::work(metrics::collector* collector) {
        auto col = _client[_opts->database_name][std::string{"sysbench"} + std::to_string(_id)];

        data::randgen entropy;
        std::uniform_int_distribution<int32_t> id_gen(0, _opts->docs_per_collection);
        
        while(!_phase->_done.load()) {

            // point selects
            for (int64_t i = 0; i < _opts->num_point_selects; ++i) {
                int32_t start_id = id_gen(entropy);
                mongocxx::options::find opts;

                auto projection = document{} << "c" << 1 << "_id" << 0 << finalize;
                opts.projection(projection);

                auto doc = col.find_one(document{} << "_id" << start_id << finalize, opts);
                
                if (doc) {
                    //std::cout << bsoncxx::to_json(doc.value()) << std::endl;
                }
            }

            // simple ranges
            for (int64_t i = 0; i < _opts->num_simple_ranges; ++i) {
                int32_t start_id = id_gen(entropy);
                int32_t end_id = start_id + _opts->range_length - 1;

                auto query = document{} << "_id" << open_document
                                            << "$gte" << start_id
                                        << "$lte" << end_id << close_document
                                        << finalize;
                
                mongocxx::options::find opts;
                auto project = document{} << "c" << 1 << "_id" << 0 << finalize;
                opts.projection(project);
                
                auto cursor = col.find(query, opts);

                for (auto&& doc : cursor) {
                    //std::cout << bsoncxx::to_json(doc) << std::endl;
                }
            }

            // sum ranges
            for (int64_t i = 0; i < _opts->num_sum_ranges; ++i) {
                int32_t start_id = id_gen(entropy);
                int32_t end_id = start_id + _opts->range_length - 1;

                mongocxx::pipeline pipeline{};

                pipeline.match(document{} << "_id" << open_document
                                              << "$gte" << start_id
                                              << "$lte" << end_id << close_document
                                          << finalize);

                pipeline.project(document{} << "k" << 1 << "_id" << 0 << finalize);

                pipeline.group(document{} << "_id" << bsoncxx::types::b_null{}
                                          << "average" << open_document
                                              << "$sum" << "$k" << close_document
                                          << finalize);

                auto cursor = col.aggregate(pipeline);
                for (auto&& doc : cursor) {
                    //std::cout << bsoncxx::to_json(doc) << std::endl;
                }
            }

            // order ranges
            for (int64_t i = 0; i < _opts->num_order_ranges; ++i) {
                int32_t start_id = id_gen(entropy);
                int32_t end_id = start_id + _opts->range_length - 1;

                auto query = document{} << "_id" << open_document
                                            << "$gte" << start_id
                                            << "$lte" << end_id << close_document
                                        << finalize;

                mongocxx::options::find opts;
                auto project = document{} << "c" << 1 << "_id" << 0 << finalize;
                opts.projection(project);
                auto sort = document{} << "c" << 1 << finalize;
                opts.sort(sort);

                auto cursor = col.find(query, opts);
                for (auto&& doc : cursor) {
                    //std::cout << bsoncxx::to_json(doc) << std::endl;
                }
            }

            // distinct ranges
            for (int64_t i = 0; i < _opts->num_distinct_ranges; ++i) {
                int32_t start_id = id_gen(entropy);
                int32_t end_id = start_id + _opts->range_length - 1;

                auto query = document{} << "_id" << open_document
                                            << "$gte" << start_id
                                            << "$lte" << end_id << close_document
                                        << finalize;

                auto cursor = col.distinct("c", query);

                for (auto&& doc : cursor) {
                    //std::cout << bsoncxx::to_json(doc) << std::endl;
                }
            }

            // index updates
            for (int64_t i = 0; i < _opts->num_indexed_updates; ++i) {
                int32_t start_id = id_gen(entropy);

                auto update = col.update_one(document{} << "_id" << start_id << finalize,
                                              document{} << "$inc" << open_document
                                                             << "k" << 1 << close_document
                                                         << finalize);
            }

            // non-index updates
            for (int64_t i = 0; i < _opts->num_unindexed_updates; ++i) {
                int32_t start_id = id_gen(entropy);

                auto update = col.update_one(document{} << " _id" << start_id << finalize,
                                             document{}
                                             << "$set" << open_document
                                             << "c" << data::random_string(data::long_mask, &entropy)
                                             << close_document
                                             << finalize);
            }

            // inserts
            for (int64_t i = 0; i < _opts->num_inserts; ++i) {
                int32_t start_id = id_gen(entropy);
                auto remove = col.delete_one(document{} << "_id" << start_id << finalize);
                auto doc = document{} << "_id" << start_id
                                      << "k" << id_gen(entropy)+1
                                      << "c" << data::random_string(data::long_mask, &entropy)
                                      << "pad" << data::random_string(data::short_mask, &entropy)
                                      << finalize;

                auto res = col.insert_one(doc);
            }
        }

    }
}
}
