#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>
#include <memory>
#include <thread>

#include "sysbench/load/operations.hpp"

namespace sysbench {
namespace metrics {

    using clock = std::chrono::high_resolution_clock;
    using duration = clock::duration;
    using time_point = clock::time_point;

    class collector {
        friend class scoped_operation;
        
    public:
        uint64_t total_inserts();
    private:
        // these are threadsafe
        void ops_succeeded(duration dur, load::operation op, uint64_t num_ops);
        void ops_failed(load::operation op_type, uint64_t num_ops);

        std::atomic<uint64_t> _inserts;
    };

    class scoped_operation {
    public:
        scoped_operation(collector* collector, load::operation op_type);
        scoped_operation(collector* collector, load::operation op_type, uint64_t num_ops);
        ~scoped_operation();
        void succeeded();
    private:
        // non-owning
        collector* _rep;
        load::operation _op_type;
        uint64_t _num_ops;
        time_point _start;
        bool _success;
    };

    //using scoped_load_operation = scoped_operation<load_collector, load_op>;
    //using scoped_exec_operation = scoped_operation<execute_collector, load_op>;

}
}
