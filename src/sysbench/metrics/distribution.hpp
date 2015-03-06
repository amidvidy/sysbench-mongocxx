#pragma once

#include <cstdint>

#include <tbb/atomic.h>
#include <tbb/concurrent_vector.h>

namespace sysbench {
namespace metrics {

  class distribution {
  public:
      distribution(uint64_t granularity);
    
      // threadsafe
      void record(int64_t measurement) const;
      int64_t percentile(double pct) const;
    
  private:
      mutable tbb::atomic<int64_t> _cardinality;
      mutable tbb::concurrent_vector<tbb::atomic<int64_t>> _buckets;
      uint64_t _granularity;
  };

}  // namespace metrics
}  // namespace sysbench


