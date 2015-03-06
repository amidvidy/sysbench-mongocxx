#pragma once

#include <atomic>
#include <vector>

#include "sysbench/execute/options.hpp"
#include "sysbench/execute/worker.hpp"

namespace sysbench {

namespace execute {

    class collector;
    
    class phase {
        friend class worker;
    public:
        phase(options opts);

        phase(phase&&) = default;
        phase& operator=(phase&&) = default;

        // noncopyable
        phase(const phase&) = delete;
        phase& operator=(const phase&) = delete;

        void run(collector* collector);
    private:
        std::atomic<bool> _done;
        options _opts;
        std::vector<worker> _workers;
    };
    
}  // execute
}  // sysbench
