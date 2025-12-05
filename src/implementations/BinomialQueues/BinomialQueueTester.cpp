//
// Created by Ali A. Kooshesh on 5/2/22.
//

#include<iostream>
#include "BinomialQueue.hpp"
#include "SmallIntMixedOperations.hpp"

enum Actions {
    INSERT,
    DELETE_MIN,
    FIND_MIN
};


Actions generateAction() {
    static const int ACTION_RANGE = 10;

    enum ActionProbability {
        INSERT_PROB = 0,        // Probability of insert is 40% -- 0, 1, 2, and 4 out of 10.
        DELETE_MIN_PROB = 4,    // Probability of delete min is 20$ -- 4, and 5 out of 10.
        FIND_MIN_PROB = 6       // Probability of find min is 40% -- 6, 7, 8, and 9.
    };

    int action = rand() % ACTION_RANGE;
    if( action < DELETE_MIN_PROB )
        return INSERT;
    else if( action < FIND_MIN_PROB)
        return DELETE_MIN;
    else
        return FIND_MIN;
}

void testBinomialQueue(BinomialQueue &queue, int numTestsToRun) {

    const int rangeOfValuesToBeGeneratedForTests = 10000;

    const int randomGenSeed = 49;
    srand(randomGenSeed);

    SmallIntMixedOperations smallIntInvertedList(rangeOfValuesToBeGeneratedForTests);

    int numTestFinished = 0;
    int id = 0;
    while ( numTestFinished < numTestsToRun ) {
        Actions action = generateAction();
        if (action == INSERT) {
            int valueToInsert = rand() % rangeOfValuesToBeGeneratedForTests;
            std::cout << "Action is INSERT -- value to insert is: " << valueToInsert << std::endl;
            queue.insert({valueToInsert, id++});
            smallIntInvertedList.insert(valueToInsert);
        } else if (action == DELETE_MIN ) {
            if (queue.empty())
                std::cout << "Action is DELETE_MIN: Will ignore delete min as the queue is empty.\n";
            else {
                std::cout << "Action is DELETE_MIN: min value to be deleted is: " << queue.findMin().first << std::endl;
                auto [min, _id] = queue.findMin();
                if(smallIntInvertedList.empty()) {
                    std::cout << "Queue has elements that are not in the inverted list. This is a bug.\n";
                    exit(1);
                }
                if( !smallIntInvertedList.member(min) ) {
                    std::cout << "The min value in the heap is not a member of the inverted list. This is a bug.\n";
                    exit(1);
                }
                smallIntInvertedList.remove(queue.findMin().first);
                queue.deleteMin();
            }
        } else {  // must be find min
            if (queue.empty()) {
                if (!smallIntInvertedList.empty()) {
                    std::cout << "Queue is empty but the inverted list is not. This is a bug.\n";
                    exit(1);
                }
                std::cout << "Action is FIND_MIN: Will ignore find min as the queue is empty.\n";
            } else {
                if( queue.findMin().first != smallIntInvertedList.minValue()) {
                    std::cout << "Smallest values in the queue is " << queue.findMin().first <<
                              ". But, the smallest value in the inverted list is " << smallIntInvertedList.minValue() <<
                              "This is a bug.\n";
                    exit(1);
                }
                std::cout << "Action is FIND_MIN: The min value is: " << queue.findMin().first << std::endl;
            }
        }
        numTestFinished++;
    }

    if( queue.empty()) {
        std::cout << "The queue is empty after having run the tests.\n";
    } else {
        std::cout << "Finished testing. The following values are what is left in the queue.\n";
        std::cout << "They are going to be printed in non-decreasing order.\n";

        while(! queue.empty() ) {
            std::cout << queue.findMin().first << std::endl;
            queue.deleteMin();
        }

    }
}
