#include "sysbench/metrics/series.hpp"

namespace sysbench {
namespace metrics {

    series::series(std::chrono::milliseconds interval)
        : series{std::move(interval), clock::now()}
    {}

    series::series(std::chrono::milliseconds interval, metrics::time_point start)
        : _interval{std::move(interval)}
        , _start{std::move(start)}
    {}

}  // namespace metrics
}  // namespace sysbench
