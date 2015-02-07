find_package(mongocxx REQUIRED)

file(GLOB sources *.cpp)

include_directories(
  ${MONGOCXX_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

link_directories(${MONGOCXX_LIBRARY_DIRS})
add_executable(sysbench-mongocxx ${sources})
target_link_libraries(sysbench-mongocxx ${MONGOCXX_LIBRARIES})
