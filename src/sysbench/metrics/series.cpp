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

    void series::record(int64_t n) const {
        auto bucket = current_bucket();
        // ensure we have space.
        auto zero = tbb::atomic<int64_t>{0};
        _data.grow_to_at_least(bucket + 1, zero);
        _data[bucket].fetch_and_add(n);
        _total.fetch_and_add(n);
    }

    int64_t series::get_total() const {
        return _total;
    }

    int64_t series::get_last() const {
      auto cur = current_bucket();
      if (current_bucket() == 0) {
        return 0;
      }

      return _data[cur - 1];

    }

}  // namespace metrics
}  // namespace sysbench
