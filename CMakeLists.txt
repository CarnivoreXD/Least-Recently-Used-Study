cmake_minimum_required(VERSION 3.17)
project(priority-queue-study)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src/implementations/BinaryHeapInVector)
add_subdirectory(src/implementations/BinomialQueues utils/comparator.cpp)


add_executable(top_level_main
        main.cpp
        src/harness/RunResults.hpp
)


add_executable(harness
        src/harness/Operation.hpp
        src/harness/main.cpp
        src/implementations/BinaryHeapInVector/BinaryHeapInVector.cpp
        src/implementations/BinaryHeapInVector/BinaryHeapInVector.hpp
        utils/comparator.cpp
        src/implementations/BinomialQueues/BinomialQueue.cpp
        src/implementations/BinomialQueues/BinomialQueue.hpp
        src/implementations/BinomialQueues/BQnode.cpp
        src/harness/RunMetaData.hpp
        src/implementations/LinearBaseLine/LinearBaseLine.cpp
        src/implementations/LinearBaseLine/LinearBaseLine.hpp
        src/implementations/Oracle/QuadraticOracle.cpp
        src/implementations/Oracle/QuadraticOracle.hpp
)

add_executable(huffman_trace
        src/trace-generators/huffman_coding/main.cpp
)



add_executable(harness-lru
        src/harness-lru/main.cpp
        src/harness-lru/HashTableDictionary.hpp
        src/harness-lru/HashTableDictionary.cpp
)

add_executable(lru
        src/trace-generators/lru/main.cpp
)