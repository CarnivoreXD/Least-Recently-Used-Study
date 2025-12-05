cmake_minimum_required(VERSION 3.17)
project(BinaryHeapInVector)

set(CMAKE_CXX_STANDARD 17)

add_executable(BinaryHeapInVector main.cpp
        BinaryHeapInVector.cpp
        BinaryHeapInVector.hpp
        ../../../utils/comparator.hpp
        ../../../utils/comparator.cpp
        ../../../utils/constants.hpp
        ../../../utils/TraceConfig.cpp
        ../../../utils/TraceConfig.hpp)
