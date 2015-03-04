#pragma once

#include <cstdint>

#include <string>
#include <vector>
#include <utility>

#include <mongocxx/client.hpp>

namespace sysbench {

namespace metrics {
    class collector;
}  // namespace collector
    
namespace load {

    struct options {
        // TODO switch some to unsigned
        int32_t ncollections{16};
        std::string database_name;

        int32_t writer_threads{8};
        int64_t docs_per_collection{100000};
        int64_t docs_per_insert{100};
        int64_t inserts_per_feedback{-1};
        int64_t seconds_per_feedback{10};

        std::string logfile;
        std::string index_technology;
        std::string write_concern{"safe"};

        std::string host{"localhost"};
        int16_t port{27017};

        std::string username{"myuser"};
        std::string password{"mypass"};
    };

    class worker;

    class loader {
    public:
        loader(options opts);

        loader(loader&&) = default;
        loader& operator=(loader&&) = default;

        // noncopyable
        loader(const loader&) = delete;
        loader& operator=(const loader&) = delete;

        bool load(metrics::collector* collector);
    private:
        options _opts;
        std::vector<worker> _workers;
    };

    class worker {
    public:
        worker(options* opts);

        void work(metrics::collector* collector);
    private:
        mongocxx::client _client;
        // non-owning
        const options* _opts;
    };

}  // namespace load
}  // namespace sysbench
