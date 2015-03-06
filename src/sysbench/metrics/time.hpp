#pragma once

#include <chrono>

namespace sysbench {
namespace metrics {

    using clock = std::chrono::high_resolution_clock;

    using duration = clock::duration;
    using time_point = clock::time_point;

}  // namespace metrics
}  // namespace sysbench
