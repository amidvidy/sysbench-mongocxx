#include "sysbench/execute/phase.hpp"

#include <thread>

namespace sysbench {
namespace execute {

    phase::phase(options opts)
        : _done{false}
        , _opts{std::move(opts)} {

            for (std::size_t i = 0; i < _opts.writer_threads; ++i) {
                _workers.emplace_back(i, &_opts, this);
            }
        }

    void phase::run(collector* collector) {
        std::vector<std::thread> threads;
        
        // hack, need extra newline here.
        std::cout << std::endl;
        std::cout << "** EXECUTE PHASE STARTING **" << std::endl;

        for (auto&& worker : _workers) {
            threads.emplace_back([&worker, collector]() { worker.work(collector); });
        }

        std::this_thread::sleep_for(_opts.phase_total_time);
        _done.store(true);
        
        // TODO: use something better like a barrier.
        for (auto&& thread : threads) {
            thread.join();
        }
    }
    
}  // namespace execute
}  // namespace sysbench
