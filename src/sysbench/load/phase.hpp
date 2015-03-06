#pragma once

#include <vector>

#include "sysbench/load/options.hpp"
#include "sysbench/load/worker.hpp"

namespace sysbench {
namespace load {

    class phase {
    public:
        phase(options opts);

        phase(phase&&) = default;
        phase& operator=(phase&&) = default;

        // noncopyable
        phase(const phase&) = delete;
        phase& operator=(const phase&) = delete;

        void run(collector* collector);
    private:
        options _opts;
        std::vector<worker> _workers;
    };
    
}
}
