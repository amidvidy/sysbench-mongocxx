#include "sysbench/load/worker.hpp"

#include <random>

#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/bulk_write.hpp>

#include "sysbench/data.hpp"
#include "sysbench/load/collector.hpp"
#include "sysbench/load/options.hpp"

namespace sysbench {
namespace load {

    worker::worker(uint32_t worker_id, options* opts)
        : _id{worker_id}
        , _client{}
        , _opts{std::move(opts)} {
    }

    void worker::work(collector* collector) {
        int64_t doc_id{0};
        std::random_device rd;
        data::randgen entropy{rd()};
        try {
            auto db = _client[_opts->database_name];

            std::string colname{std::string{"sysbench"} + std::to_string(_id)};

            auto col = db[colname];

            if (db.has_collection(colname)) {
                col.drop();
            }

            auto rounds = _opts->docs_per_collection/_opts->docs_per_insert;

            for (int64_t cur_round = 0; cur_round < rounds; ++cur_round) {

                mongocxx::bulk_write bulk{false /* unordered */};

                for (int64_t i = 0; i < _opts->docs_per_insert; ++i) {

                    ++doc_id;
                    bsoncxx::builder::stream::document doc;

                    doc << "_id" << doc_id
                        << "k" << static_cast<int32_t>(entropy())
                        << "c" << data::random_string(data::long_mask, &entropy)
                        << "pad" << data::random_string(data::short_mask, &entropy);

                    bulk.append(mongocxx::model::insert_one{doc});
                }
                scoped_operation op(collector, operation::k_insert, _opts->docs_per_insert);
                col.bulk_write(bulk);
                op.succeeded();
            }
        } catch (const std::exception& ex) {
            /** FIXME: need actual exception handling **/
            std::cerr << "Fatal error: " << ex.what() << std::endl;
            std::abort();
        }
    }
    
}
}
