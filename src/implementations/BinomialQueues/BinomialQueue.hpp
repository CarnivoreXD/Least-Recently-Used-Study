//
// Created by Ali A. Kooshesh on 5/5/21.
//

#ifndef BINOMIALQUEUES_BINOMIALQUEUE_HPP
#define BINOMIALQUEUES_BINOMIALQUEUE_HPP

/* This class provides an implementation for Binomial Queues: https://en.wikipedia.org/wiki/Binomial_heap
 * It uses a vector to represent the heap-ordered trees that form the queue.
 * Suppose the Binomial Queue consists of k tree, including null trees. Then,
 * for 0 <= i <= k - 1, heapOrderedTrees[i] is either a null pointer or it
 * points to a tree that contains 2^i nodes. We refer to this property as
 * the Binomial Queue invariant.
 *
 * The run-time of the queue operations is as follows
 * 1. Insert: O(log n)
 * 2. findMin: O(log n)
 * 3. deleteMin: O(log n)
 * 4. empty: O(1)
 * 5. size: O(1)
 * 6. merge: O(log n)
 *
 * The ADT doesn't provide an explicit merge operation even though it implements it
 * when it performs delete min.
*/
#include<tuple>
#include "BQnode.hpp"

class BinomialQueue {
public:
    explicit BinomialQueue(bool (*)(const std::pair<int, int> &, const std::pair<int, int> &));
    void insert(const std::pair<int, int> &v);
    std::pair<int, int> findMin();
    std::pair<int, int> extractMin();
    void deleteMin();
    bool empty() const;
    int size() const;
    void clear();
    void print();

    ~BinomialQueue();

private:
    std::vector<BQnode *> heapOrderedTrees;
    int numNodesInTheQueue = 0;


    bool (* priorityFunc)(const std::pair<int, int> &a, const std::pair<int, int> &b);
    void printHeapOrderedTree(BQnode *tree);
    BQnode *treeWithMinValue();
    void insertTreeAtIndex(BQnode *tree, std::size_t idx);
    int assertSizeOfTree(BQnode *tree);
    void assertSizeOfBinomialQueue();
    BQnode *joinTrees(BQnode *tree1, BQnode *tree2) const;
    void deleteAllNodes();
    void deleteTree(BQnode *tree);
};


#endif //BINOMIALQUEUES_BINOMIALQUEUE_HPP
