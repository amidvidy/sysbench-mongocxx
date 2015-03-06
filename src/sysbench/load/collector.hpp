#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <thread>

#include "sysbench/load/operations.hpp"
#include "sysbench/load/options.hpp"
#include "sysbench/metrics/time.hpp"
#include "sysbench/metrics/series.hpp"

namespace sysbench {
namespace load {

    class scoped_operation;

    class collector {
        friend class scoped_operation;

        public:
            collector(options opts);

            std::string report_string() const;
            // std::string report_json();
        private:
            // these are threadsafe
           void ops_succeeded(metrics::duration dur, load::operation op, uint64_t num_ops);
           void ops_failed(load::operation op_type, uint64_t num_ops);

           options _opts;
           metrics::series _inserts;

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
        metrics::time_point _start;
        bool _success;
    };

}  // namespace load
}  // namespace sysbench
