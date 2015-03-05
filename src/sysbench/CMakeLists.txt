find_package(mongocxx REQUIRED)

set(sysbench-sources
  data.cpp
  main.cpp
  load/phase.cpp
  load/worker.cpp
  execute/phase.cpp
  execute/worker.cpp
  metrics/metrics.cpp
  report/report.cpp
)
  
include_directories(
  ${MONGOCXX_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/src
)

link_directories(${MONGOCXX_LIBRARY_DIRS})
add_executable(sysbench-mongocxx ${sysbench-sources})
target_link_libraries(sysbench-mongocxx ${MONGOCXX_LIBRARIES})
