cmake_minimum_required(VERSION 3.17)
project(BinomialQueues)

set(CMAKE_CXX_STANDARD 17)

add_executable(BinomialQueues main.cpp BQnode.cpp BQnode.hpp BinomialQueue.cpp BinomialQueue.hpp SmallIntMixedOperations.cpp SmallIntMixedOperations.hpp InvertedListDictionary.cpp InvertedListDictionary.hpp BinomialQueueTester.cpp
        comparator.hpp ../../../utils/comparator.cpp)