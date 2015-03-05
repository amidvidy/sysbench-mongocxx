#include "execute.hpp"

#include <chrono>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/options/find.hpp>
#include <mongocxx/pipeline.hpp>

#include "metrics.hpp"

namespace sysbench {
namespace execute {

    using namespace bsoncxx::builder::stream;

    execute_phase::execute_phase(options opts) : _opts{std::move(opts)} {
    }

    void execute_phase::run(metrics::collector* collector) {
        // sleep until benchmark is over, then tell writers to stop.
    }

    worker::worker(options* opts)
        : _client{}
        , _opts{std::move(opts)} {
    }

    void worker::work(metrics::collector* collector) {
        auto col = _client["foo"]["bar"];

        while(!_phase->_done.load()) {

            // point selects
            for (int64_t i = 0; i < _opts->num_point_selects; ++i) {
                int32_t start_id = 0;
                mongocxx::options::find opts;
                opts.projection(document{} << "c" << 1 << "_id" << 0 << finalize);
                auto doc = col.find_one(document{} << "_id" << start_id << finalize, opts);
            }

            // simple ranges
            for (int64_t i = 0; i < _opts->num_simple_ranges; ++i) {
                int32_t start_id = 0;
                int32_t end_id = start_id + _opts->range_length - 1;

                auto query = document{} << "_id" << open_document
                                            << "$gte" << start_id
                                        << "$lte" << end_id << close_document
                                        << finalize;
                
                mongocxx::options::find opts;
                opts.projection(document{} << "c" << 1 << "_id" << 0 << finalize);
                
                auto cursor = col.find(query, opts);
                for (auto&& doc : cursor) {
                    std::cout << bsoncxx::to_json(doc) << std::endl;
                }
            }

            // sum ranges
            for (int64_t i = 0; i < _opts->num_sum_ranges; ++i) {
                int32_t start_id = 0;
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
                    std::cout << bsoncxx::to_json(doc) << std::endl;
                }
            }

            // order ranges
            for (int64_t i = 0; i < _opts->num_order_ranges; ++i) {
                int32_t start_id = 0;
                int32_t end_id = start_id + _opts->range_length - 1;

                auto query = document{} << "_id" << open_document
                                            << "$gte" << start_id
                                            << "$lte" << end_id << close_document
                                        << finalize;

                mongocxx::options::find opts;
                opts.projection(document{} << "c" << 1 << "_id" << 0 << finalize);
                opts.sort(document{} << "c" << 1 << finalize);

                auto cursor = col.find(query, opts);
                for (auto&& doc : cursor) {
                    std::cout << bsoncxx::to_json(doc) << std::endl;
                }
            }

            // distinct ranges
            for (int64_t i = 0; i < _opts->num_distinct_ranges; ++i) {
                int32_t start_id = 0;
                int32_t end_id = start_id + _opts->range_length - 1;

                auto query = document{} << "_id" << open_document
                                            << "$gte" << start_id
                                            << "$lte" << end_id << close_document
                                        << finalize;

                auto cursor = col.distinct("c", query);

                for (auto&& doc : cursor) {
                    std::cout << bsoncxx::to_json(doc) << std::endl;
                }
            }

            // index updates
            for (int64_t i = 0; i < _opts->num_indexed_updates; ++i) {
                int32_t start_id = 0;

                auto update = col.update_one(document{} << "_id" << start_id << finalize,
                                              document{} << "$inc" << open_document
                                                             << "k" << 1 << close_document
                                                         << finalize);
            }

            // non-index updates
            for (int64_t i = 0; i < _opts->num_unindexed_updates; ++i) {
                int32_t start_id = 0;

                std::string cval{""};

                auto update = col.update_one(document{} << " _id" << start_id << finalize,
                                              document{} << "$set" << open_document
                                                             << "c" << cval << close_document
                                                         << finalize);
            }

            // inserts
            for (int64_t i = 0; i < _opts->num_inserts; ++i) {
                int32_t start_id = 0;

                auto remove = col.delete_one(document{} << "_id" << start_id << finalize);

                auto doc = document{} << "_id" << start_id
                                      << "k" << 22 // fixme
                                      << "c" << "ccccc" // fixme
                                      << "pad" << "dddd" // fixme
                                      << finalize;

                auto res = col.insert_one(doc);
            }
        }

    }
}
}
