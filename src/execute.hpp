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
        uint32_t num_collections{1};
        uint32_t writer_threads{8};
        uint32_t seconds_per_feedback{1};

        uint32_t num_point_selects{1};
        uint32_t num_simple_ranges{1};
        uint32_t num_sum_ranges{1};
        uint32_t num_order_ranges{1};
        uint32_t num_distinct_ranges{1};
        uint32_t num_indexed_updates{1};
        uint32_t num_unindexed_updates{1};
        uint32_t num_inserts{1};

        uint32_t range_length;
        
        std::string database_name{"sysbenchdb"};
    };


    class worker;
    
    class execute_phase {
        friend class worker;
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
        worker(uint32_t worker_id, options* opts, execute_phase* phase);
        void work(metrics::collector* collector);
    private:
        uint32_t _id;
        mongocxx::client _client;
        // non-owning
        const options* _opts;
        // non-owning
        execute_phase* _phase;
    };
    
}
}
