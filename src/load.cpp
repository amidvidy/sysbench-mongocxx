#include <iostream>
#include <random>

#include <bsoncxx/builder.hpp>

#include <mongocxx/bulk_write.hpp>

#include "load.hpp"

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

    void loader::load() {
        worker w{&_opts};
        w.work();
    }

    loader::loader(options opts) : _opts(std::move(opts)) {}

    loader::~loader() {
        // shutdown workers
    }

    worker::worker(options* opts)
        : _client{}
        , _opts{std::move(opts)} {}

    void worker::work() {
        int64_t doc_id{0};
        auto col = _client["foo"]["bar"];
        col.drop();
        randgen entropy;
        try {
            auto rounds = _opts->docs_per_collection/_opts->docs_per_insert;
            for (int64_t cur_round = 0; cur_round < rounds; ++cur_round) {
                mongocxx::bulk_write bulk{false /* unordered */};
                for (int64_t i = 0; i < _opts->docs_per_insert; ++i) {
                    ++doc_id;
                    bsoncxx::builder::document doc;
                    doc << "_id" << doc_id
                        << "k" << static_cast<int32_t>(entropy())
                        << "c" << random_string(long_mask, entropy)
                        << "pad" << random_string(short_mask, entropy);
                    bulk.append(mongocxx::model::insert_one{doc});
                }
                col.bulk_write(bulk);
                std::cout << cur_round << "/" << rounds << std::endl;
            }
        } catch (const std::exception& ex) {
            /** FIXME: need actual exception handling **/
            std::cerr << "Fatal error: " << ex.what() << std::endl;
        }
    }

}  // namespace load
}  // namespace sysbench
