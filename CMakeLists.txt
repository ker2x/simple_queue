cmake_minimum_required(VERSION 3.20)
project(simple_queue)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(benchmark)
include_directories(benchmark/include)

#set(Boost_INCLUDE_DIR c:/mingw/include/)
#set(Boost_LIBRARY_DIR c:/mingw/lib/)
find_package(Boost 1.76.0 COMPONENTS thread program_options context filesystem regex system)
include_directories(${Boost_INCLUDE_DIR})

add_executable(simple_queue main.cpp spsc_queue.h simple_queue.h)
target_link_libraries(simple_queue benchmark::benchmark)
