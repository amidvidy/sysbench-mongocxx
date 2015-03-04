#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>
#include <memory>
#include <thread>

namespace sysbench {
namespace metrics {

    using clock = std::chrono::high_resolution_clock;
    using duration = clock::duration;
    using time_point = clock::time_point;

    enum class load_op {
        k_insert
    };

    enum class execute_op {
        k_point_query,
        k_simple_range,
        k_sum_range,
        k_order_range,
        k_district_range,
        k_indexed_update,
        k_nonindexed_update,
        k_insert
    };

    class collector {
        friend class scoped_operation;

        uint64_t get_inserts();
        
    private:
        // these are threadsafe
        void ops_succeeded(duration dur, load_op op_type, uint64_t num_ops);
        void ops_failed(load_op op_type, uint64_t num_ops);

        std::atomic<uint64_t> _inserts;
    };

    class scoped_operation {
    public:
        scoped_operation(collector* collector, load_op op_type);
        scoped_operation(collector* collector, load_op op_type, uint64_t num_ops);
        ~scoped_operation();
        void succeeded();
    private:
        // non-owning
        collector* _rep;
        load_op _op_type;
        uint64_t _num_ops;
        time_point _start;
        bool _success;
    };

    class console_logger {
       console_logger(collector* rep);
       ~console_logger();
   private:
       collector* _rep;
       std::thread _thread;
   };
}
}
