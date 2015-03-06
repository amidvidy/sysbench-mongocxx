find_package(mongocxx REQUIRED)
find_package(tbb REQUIRED)

set(sysbench-sources
  data.cpp
  main.cpp
  load/collector.cpp
  load/phase.cpp
  load/worker.cpp
  execute/collector.cpp
  execute/phase.cpp
  execute/worker.cpp
  metrics/series.cpp
)
  
include_directories(
  ${MONGOCXX_INCLUDE_DIRS}
  ${TBB_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/src
)

link_directories(${MONGOCXX_LIBRARY_DIRS})

add_executable(sysbench-mongocxx ${sysbench-sources})
target_link_libraries(sysbench-mongocxx ${MONGOCXX_LIBRARIES})
target_link_libraries(sysbench-mongocxx ${TBB_LIBRARIES})
