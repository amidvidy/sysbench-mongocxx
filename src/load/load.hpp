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
        uint32_t num_collections{16};
        std::string database_name{"sysbenchdb"};

        uint32_t writer_threads{1};
        uint64_t docs_per_collection{1000};
        uint64_t docs_per_insert{100};
        uint64_t inserts_per_feedback{1};
        uint64_t seconds_per_feedback{10};

        std::string logfile;
        std::string index_technology;
        std::string write_concern{"safe"};

        std::string host{"localhost"};
        int16_t port{27017};

        std::string username{"myuser"};
        std::string password{"mypass"};
    };

    class worker;

    class load_phase {
    public:
        load_phase(options opts);

        load_phase(load_phase&&) = default;
        load_phase& operator=(load_phase&&) = default;

        // noncopyable
        load_phase(const load_phase&) = delete;
        load_phase& operator=(const load_phase&) = delete;

        void run(metrics::collector* collector);
    private:
        options _opts;
        std::vector<worker> _workers;
    };

    class worker {
    public:
        worker(uint32_t worker_id, options* opts);

        void work(metrics::collector* collector);
    private:
        uint32_t _id;
        mongocxx::client _client;
        // non-owning
        const options* _opts;
    };

}  // namespace load
}  // namespace sysbench
