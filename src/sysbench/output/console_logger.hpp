#pragma once

#include <memory>
#include <thread>

namespace sysbench {

namespace metrics {
    class collector;
}

namespace output {

    template<typename Collector>
    class console_logger {
    public:
        console_logger(Collector* collect)
            : _collect{std::move(collect)}
            , _thread{}
            , _done{false} {
        }

        ~console_logger() {
            std::cout << "destroying logger" << std::endl;
            _done.store(true);
            if (_thread.joinable()) {
                _thread.join();
            }
        }
        void start() {
            _thread = std::thread([this]() {
                auto interval = std::chrono::seconds{1};
                while (!this->_done.load()) {
                    std::cout << _collect->report_string() << std::endl;
                    std::this_thread::sleep_for(interval);
                }
            });
        }
    private:
        Collector* _collect;
        std::thread _thread;
        std::atomic<bool> _done;
    };

}  // namespace output
}  // namespace sysbench
