#include "sysbench/metrics/series.hpp"

namespace sysbench {
namespace metrics {

    series::series(std::chrono::milliseconds interval)
        : series{std::move(interval), clock::now()}
    {}

    series::series(std::chrono::milliseconds interval, metrics::time_point start)
        : _data{}
        , _interval{std::move(interval)}
        , _start{std::move(start)}
    {}

    void series::record(uint64_t n) {
        auto now = clock::now();
    }


}  // namespace metrics
}  // namespace sysbench
