cmake_minimum_required(VERSION 3.20)
project(simple_queue)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.53.0 COMPONENTS thread program_options context filesystem regex system)


add_executable(simple_queue main.cpp spsc_queue.h simple_queue.h)
