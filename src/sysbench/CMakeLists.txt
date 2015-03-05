find_package(mongocxx REQUIRED)

set(sources
  data.cpp
  execute.cpp
  load.cpp
  main.cpp
  metrics.cpp
  report.cpp
)


  
include_directories(
  ${MONGOCXX_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

link_directories(${MONGOCXX_LIBRARY_DIRS})
add_executable(sysbench-mongocxx ${sources})
target_link_libraries(sysbench-mongocxx ${MONGOCXX_LIBRARIES})
