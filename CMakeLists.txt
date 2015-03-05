cmake_minimum_required(VERSION 3.1.0)

project(SYSBENCH-MONGOCXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(PkgConfig)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default is Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Set PKG_CONFIG_PATH
if(DEFINED $ENV{PKG_CONFIG_PATH})
    set(PKG_CONFIG_PATH $ENV{PKG_CONFIG_PATH})
else()
    set(PKG_CONFIG_PATH "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig")
endif()

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -fPIC ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0 -g")

add_subdirectory(src)
