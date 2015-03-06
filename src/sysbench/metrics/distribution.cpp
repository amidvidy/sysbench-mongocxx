#include "sysbench/metrics/distribution.hpp"

#include <utility>

namespace sysbench {
namespace metrics {
   // todo, implement granularity
  distribution::distribution(uint64_t granularity)
    : _cardinality{0}
    , _buckets{}
    , _granularity{std::move(granularity)}
  {}

  void distribution::record(uint64_t measurement) const {
      _cardinality.fetch_and_add(1);
      auto res = _buckets.insert(std::make_pair(measurement, tbb::atomic<uint64_t>{1}));
      if (!res.second) {
          // there was already a key for this measurement
          res.first->second.fetch_and_add(1);
      }

  }

  // this implementation will give approximate answers, but its a good estimation.
  uint64_t distribution::percentile(double pct) const {
      uint64_t target = static_cast<uint64_t>(pct * _cardinality);
      // todo, iterate backwards or forwards depending on pct.
      for (auto&& bucket : _buckets) { 
          target -= bucket.second;
          if (target <= 0) {
              return bucket.first;
          }
      }
      std::abort();
  }

}
}  // namespace sysbench
