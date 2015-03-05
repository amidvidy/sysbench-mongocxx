#pragma once

#include <memory>
#include <thread>

namespace sysbench {

namespace metrics {
    class collector;
}

namespace report {

    class console_logger {
    public:
        console_logger(metrics::collector* rep);
        ~console_logger();
        void start();
    private:
        metrics::collector* _rep;
        std::thread _thread;
        std::atomic<bool> _done;
    };

}  // namespace report
}  // namespace sysbench
