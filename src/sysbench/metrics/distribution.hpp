#pragma once

#include <cstdint>

#include <tbb/atomic.h>
#include <tbb/concurrent_unordered_map.h>

namespace sysbench {
namespace metrics {

  class distribution {
  public:
      distribution(uint64_t granularity);
    
      // threadsafe
      void record(uint64_t measurement) const;
      uint64_t percentile(double pct) const;
    
  private:
      mutable tbb::atomic<uint64_t> _cardinality;
      mutable tbb::concurrent_unordered_map<uint64_t, tbb::atomic<uint64_t>> _buckets;
      uint64_t _granularity;
  };

}  // namespace metrics
}  // namespace sysbench


