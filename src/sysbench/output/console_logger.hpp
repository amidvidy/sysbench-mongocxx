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
        console_logger(Collector* rep);
        ~console_logger();
        void start();
    private:
        Collector* _rep;
        std::thread _thread;
        std::atomic<bool> _done;
    };

}  // namespace output
}  // namespace sysbench
