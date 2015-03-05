#pragma once

#include <atomic>
#include <cstdint>
#include <string>
#include <vector>

#include <mongocxx/client.hpp>

namespace sysbench {

namespace metrics {
    class collector;
}
    
namespace execute {

    struct options {
        uint32_t num_collections;
        uint32_t writer_threads;
        uint32_t seconds_per_feedback;
        std::string database_name;
    };

    class worker;

    class execute_phase {
    public:
        execute_phase(options opts);

        execute_phase(execute_phase&&) = default;
        execute_phase& operator=(execute_phase&&) = default;

        // noncopyable
        execute_phase(const execute_phase&) = delete;
        execute_phase& operator=(const execute_phase&) = delete;

        void run(metrics::collector* collector);
    private:
        std::atomic<bool> _done;
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
        // non-owning
        execute_phase* _phase;
    };
    
}
}
