#include "sysbench/metrics/series.hpp"

namespace sysbench {
namespace metrics {

    series::series(std::chrono::milliseconds tick)
        : series{std::move(tick), clock::now()}
    {}

    series::series(std::chrono::milliseconds tick, metrics::time_point start)
        : _data{}
        , _tick{std::move(tick)}
        , _start{std::move(start)}
    {}

    std::size_t series::current_bucket() const {
        auto now = clock::now();
        auto elapsed_millis =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - _start).count();
        return elapsed_millis / _tick.count();
    }

    duration series::elapsed() const {
        return clock::now() - _start;
    }

    std::chrono::milliseconds series::tick() const {
        return _tick;
    }

    void series::record(uint64_t n) const {
        auto bucket = current_bucket();
        // ensure we have space.
        // todo: figure out ugly initalization issue
        _data.grow_to_at_least(bucket + 1);
        _data[bucket].fetch_add(n);
        _total.fetch_add(n);
    }

    uint64_t series::get_total() const {
        return _total.load();
    }

    uint64_t series::get_current() const {
        auto bucket = current_bucket();
        _data.grow_to_at_least(bucket + 1);
        return _data[bucket].load();
    }

}  // namespace metrics
}  // namespace sysbench
