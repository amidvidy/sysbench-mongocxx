#include "sysbench/load/phase.hpp"

#include <thread>

namespace sysbench {
namespace load {

    void phase::run(collector* collector) {
        std::vector<std::thread> threads;
        std::cout << "** LOAD PHASE STARTING **" << std::endl;
        for (auto&& worker : _workers) {
            threads.emplace_back([&worker, collector]() {
                worker.work(collector);
            });
        }
        // TODO: use something better like a barrier.
        for (auto&& thread : threads) {
            thread.join();
        }
    }

    phase::phase(options opts) : _opts(std::move(opts)) {
        for (std::size_t i = 0; i < _opts.writer_threads; ++i) {
            _workers.emplace_back(i, &_opts);
        }
    }
    
}
}
