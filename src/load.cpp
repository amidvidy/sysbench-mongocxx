#include <iostream>
#include <random>
#include <thread>

#include <bsoncxx/builder/stream/document.hpp>

#include <mongocxx/bulk_write.hpp>

#include "load.hpp"
#include "metrics.hpp"

namespace sysbench {
namespace load {

    namespace {

        using randgen = std::mt19937;

        // TODO, why is std::rand() so much faster than all the
        // stuff in <random> ??
        std::string random_string(const char* mask, randgen& r) {
            //std::uniform_int_distribution<int> distribution;
            std::string s(mask);
            for (std::size_t i = 0; i < s.length(); ++i) {
                if (s[i] == '#') {
                    //s[i] = 'a' + static_cast<char>(distribution(r) % 26);
                    s[i] = 'a' + static_cast<char>(std::rand() % 26);
                }
            }
            return s;
        }

        auto long_mask = "###########-###########-###########-###########-###########-###########-###########-###########-###########-###########";
        auto short_mask = "###########-###########-###########-###########-###########";
    }

    void loader::load(metrics::collector* collector) {
        std::vector<std::thread> threads;

        for (auto&& worker : _workers) {
            threads.emplace_back([&worker, collector]() { worker.work(collector); });
        }

        // TODO: use something better like a barrier.
        for (auto&& thread : threads) {
            thread.join();
        }
    }

    loader::loader(options opts) : _opts(std::move(opts)) {
        for (std::size_t i = 0; i < _opts.writer_threads; ++i) {
            _workers.emplace_back(&_opts);
        }
    }

    worker::worker(options* opts)
        : _client{}
        , _opts{std::move(opts)} {
        }

    void worker::work(metrics::collector* collector) {
        int64_t doc_id{0};
        randgen entropy;
        try {
            auto db = _client["foo"];

            std::string colname{random_string(short_mask, entropy)};

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
                        << "c" << random_string(long_mask, entropy)
                        << "pad" << random_string(short_mask, entropy);

                    bulk.append(mongocxx::model::insert_one{doc});
                }
                metrics::scoped_operation op(collector, metrics::load_op::k_insert, _opts->docs_per_insert);
                col.bulk_write(bulk);
                op.succeeded();
            }
        } catch (const std::exception& ex) {
            /** FIXME: need actual exception handling **/
            std::cerr << "Fatal error: " << ex.what() << std::endl;
            std::abort();
        }
    }

}  // namespace load
}  // namespace sysbench
