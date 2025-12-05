#include <iostream>
#include <fstream>

#include "BinomialQueue.hpp"
#include "SmallIntMixedOperations.hpp"
#include "BinomialQueueTester.hpp"
#include "../../../utils/comparator.hpp"
const bool fromInput = false;


void testUsingInput(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " nameOfAnInputFile\n";
        exit(1);
    }
    std::ifstream inputStream;
    inputStream.open(argv[1], std::ios::in);
    if (!inputStream.is_open()) {
        std::cout << "Unable top open " << argv[1] << ". Terminating...";
        exit(1);
    }


    int key, id = 0;
    BinomialQueue priorityQueue(compare_pair);
    while( inputStream >> key && inputStream >> id ) {
      //        std::cout << "inserting <" << inputValue << ", " << id << ">" << std::endl;
        priorityQueue.insert({key, id++});
    }

    std::cout <<"The queue contains the following values.\n";
    while( ! priorityQueue.empty() ) {
      auto [key1, id1] = priorityQueue.findMin();
      priorityQueue.deleteMin();
      std::cout << key1 << " " << id1 << std::endl;
    }
  
}


int main(int argc, char *argv[]) {

    // This program creates and tests a Binomial Queue.
    // It requires an input file. 

  if(fromInput) 
    testUsingInput(argc, argv);
  else {
    BinomialQueue priorityQueue(compare_pair);
    int numTestsToRun = 1000;
    testBinomialQueue(priorityQueue, numTestsToRun);
  }
  return 0;
}
