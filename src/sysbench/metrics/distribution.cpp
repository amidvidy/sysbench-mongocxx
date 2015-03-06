#include "sysbench/metrics/distribution.hpp"

#include <iostream>
#include <utility>

namespace sysbench {
namespace metrics {
   // todo, implement granularity
  distribution::distribution(uint64_t granularity)
    : _cardinality{0}
    , _buckets{}
    , _granularity{std::move(granularity)}
  {}

  void distribution::record(int64_t measurement) const {
      _cardinality.fetch_and_add(1);
      _buckets.grow_to_at_least(measurement+1, tbb::atomic<int64_t>{0});
      _buckets[measurement].fetch_and_add(1);

  }

  // this implementation will give approximate answers, but its a good estimation.
  int64_t distribution::percentile(double pct) const {
      int64_t target = static_cast<uint64_t>(pct * _cardinality);
      std::cout << target << std::endl;
      // todo, iterate backwards or forwards depending on pct.
      int64_t current = 0;
      for (auto&& bucket : _buckets.range()) { 
          target -= bucket;
          if (target <= 0) {
              return current;
          }
          ++current;
      }
      std::abort();
  }

}
}  // namespace sysbench
