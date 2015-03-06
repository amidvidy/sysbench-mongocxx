#include "sysbench/execute/worker.hpp"

#include <chrono>
#include <exception>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/options/find.hpp>
#include <mongocxx/pipeline.hpp>

#include "sysbench/data.hpp"
#include "sysbench/execute/collector.hpp"
#include "sysbench/execute/operations.hpp"
#include "sysbench/execute/options.hpp"
#include "sysbench/execute/phase.hpp"

namespace sysbench {
namespace execute {

    using namespace bsoncxx::builder::stream;

    worker::worker(uint32_t worker_id, options* opts, phase* phase)
        : _id{worker_id}
        , _client{}
        , _opts{std::move(opts)}
        , _phase{std::move(phase)} {
    }

    void worker::work(collector* collector) {
        auto col = _client[_opts->database_name][std::string{"sysbench"} + std::to_string(_id)];

        data::randgen entropy;
        std::uniform_int_distribution<int32_t> id_gen(0, _opts->docs_per_collection);

        while (!_phase->_done.load()) {

            scoped_operation txn(collector, operation::k_transaction);

            // point selects
            for (int64_t i = 0; i < _opts->num_point_selects; ++i) {
                try {
                    int32_t start_id = id_gen(entropy);
                    mongocxx::options::find opts;

                    auto projection = document{} << "c" << 1 << "_id" << 0 << finalize;
                    opts.projection(projection);

                    scoped_operation op(collector, operation::k_point_query);

                    auto doc = col.find_one(document{} << "_id" << start_id << finalize, opts);
                    if (doc) {
                        //std::cout << bsoncxx::to_json(doc.value()) << std::endl;
                    }
                    op.succeeded();
                } catch (const std::exception& ex) {
                    // FIXME
                }
            }

            // simple ranges
            for (int64_t i = 0; i < _opts->num_simple_ranges; ++i) {
                try {
                    int32_t start_id = id_gen(entropy);
                    int32_t end_id = start_id + _opts->range_length - 1;

                    auto query = document{} << "_id" << open_document
                                                << "$gte" << start_id
                                                << "$lte" << end_id << close_document
                                            << finalize;

                    mongocxx::options::find opts;
                    auto project = document{} << "c" << 1 << "_id" << 0 << finalize;
                    opts.projection(project);

                    scoped_operation op(collector, operation::k_simple_range);
                    auto cursor = col.find(query, opts);
                    for (auto&& doc : cursor) {
                        //std::cout << bsoncxx::to_json(doc) << std::endl;
                    }
                    op.succeeded();
                } catch (const std::exception& ex) {
                    // FIXME
                }
            }

            // sum ranges
            for (int64_t i = 0; i < _opts->num_sum_ranges; ++i) {
                try {
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

                   scoped_operation op(collector, operation::k_sum_range);
                   auto cursor = col.aggregate(pipeline);
                   for (auto&& doc : cursor) {
                       //std::cout << bsoncxx::to_json(doc) << std::endl;
                   }
                   op.succeeded();
                } catch (const std::exception& ex) {
                    // FIXME
                }
            }

            // order ranges
            for (int64_t i = 0; i < _opts->num_order_ranges; ++i) {
                try {
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

                    scoped_operation op(collector, operation::k_order_range);
                    auto cursor = col.find(query, opts);

                    for (auto&& doc : cursor) {
                        //std::cout << bsoncxx::to_json(doc) << std::endl;
                    }
                    op.succeeded();
                } catch (const std::exception& ex) {
                    // FIXME
                }
            }

            // distinct ranges
            for (int64_t i = 0; i < _opts->num_distinct_ranges; ++i) {
                try {
                    int32_t start_id = id_gen(entropy);
                    int32_t end_id = start_id + _opts->range_length - 1;

                    auto query = document{} << "_id" << open_document
                                                << "$gte" << start_id
                                                << "$lte" << end_id << close_document
                                            << finalize;

                    scoped_operation op(collector, operation::k_distinct_range);
                    auto cursor = col.distinct("c", query);
                    for (auto&& doc : cursor) {
                        //std::cout << bsoncxx::to_json(doc) << std::endl;
                    }
                    op.succeeded();
                } catch (const std::exception& ex) {
                    // FIXME
                }
            }

            // index updates
            for (int64_t i = 0; i < _opts->num_indexed_updates; ++i) {
                try {
                    int32_t start_id = id_gen(entropy);

                    scoped_operation op(collector, operation::k_indexed_update);
                    auto update = col.update_one(document{} << "_id" << start_id << finalize,
                                                 document{} << "$inc" << open_document
                                                                 << "k" << 1 << close_document
                                                             << finalize);
                    op.succeeded();
                } catch (const std::exception& ex) {
                    // FIXME
                }
            }

            // non-index updates
            for (int64_t i = 0; i < _opts->num_unindexed_updates; ++i) {
                try {
                    int32_t start_id = id_gen(entropy);

                    auto update = document{}
                        << "$set" << open_document
                            << "c" << data::random_string(data::long_mask, &entropy)
                        << close_document << finalize;

                    scoped_operation op(collector, operation::k_nonindexed_update);
                    auto res = col.update_one(document{} << " _id" << start_id << finalize,
                                              update);
                    op.succeeded();
                } catch (const std::exception& ex) {
                    // FIXME
                }
            }

            // inserts
            for (int64_t i = 0; i < _opts->num_inserts; ++i) {
                try {
                    int32_t start_id = id_gen(entropy);
                    auto remove = col.delete_one(document{} << "_id" << start_id << finalize);
                    auto doc = document{}
                        << "_id" << start_id
                        << "k" << id_gen(entropy)+1
                        << "c" << data::random_string(data::long_mask, &entropy)
                        << "pad" << data::random_string(data::short_mask, &entropy)
                        << finalize;

                    scoped_operation op(collector, operation::k_insert);
                    auto res = col.insert_one(doc);
                    op.succeeded();
                } catch (const std::exception& ex) {
                    // FIXME
                }
            }

            txn.succeeded();
        }

    }

}  // namespace execute
}  // namespace sysbench
