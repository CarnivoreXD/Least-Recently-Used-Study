//
// Created by Ali A. Kooshesh on 5/5/21.
//

#include "BinomialQueue.hpp"
#include<iostream>
#include <cassert>

BinomialQueue::BinomialQueue(bool (*pFunc)(const std::pair<int, int> &, const std::pair<int, int> &)) {
    priorityFunc = pFunc;
}

void BinomialQueue::clear() {
    deleteAllNodes();
    heapOrderedTrees.clear();
    numNodesInTheQueue = 0;
}

std::pair<int, int> BinomialQueue::extractMin() {
    auto min = findMin();
    deleteMin();
    return min;
}


BQnode *BinomialQueue::joinTrees(BQnode *tree1, BQnode *tree2) const {
    // This is a single step in the process of inserting a new node
    // in the queue and in merging two queues after a deleteMin operation.

    // pre-condition: each of the two trees, tree1 and tree2
    //   is a heap-ordered tree and the two have the same number of
    //   nodes.

    // The function joins the trees rooted at tree1 and tree2 by making
    // the one whose value is smaller the parent of the other one and
    // returns a pointer to the parent tree.

    if( priorityFunc(tree1->value(), tree2->value()) ) {
        tree1->addChild(tree2);
        return tree1;
    }

    tree2->addChild(tree1);
    return tree2;
}

void BinomialQueue::insertTreeAtIndex(BQnode *treeToInsert, std::size_t idx) {
    // treeToInsert is a heap-ordered tree that contains 2^idx nodes.
    // This function inserts treeToInsert into the Binomial Queue starting
    // at index specified by idx.

    // When function insert calls this function, treeToInsert contains a single
    // node and "idx" is zero.

    auto carry = treeToInsert;
    for( std::size_t i = idx; i < heapOrderedTrees.size(); i++ ) {
        // carry is a pointer to the root of a heap-ordered tree that contains
        // 2^i nodes. In this iteration of the loop, either carry gets inserted
        // into heapOrderedTrees, in which case the loop stops, or we merge the two
        // trees, heapOrderedTrees[i] and carry, which results in a tree with 2^(i+1)
        // nodes.

        // Pre-conditions:
        //    1. heapOrderedTrees satisfies the Binomial Queue invariant.
        //    2. heapOrderedTrees[heapOrderedTree.size() - 1] is not nullptr.

        if(heapOrderedTrees.at(i) == nullptr) {
            heapOrderedTrees.at(i) = carry;
            carry = nullptr;
            break;
        }
        carry = joinTrees(carry, heapOrderedTrees.at(i));
        heapOrderedTrees.at(i) = nullptr;
    }

    if(carry != nullptr )  // add a new tree to the queue, extending the number of its trees.
        heapOrderedTrees.push_back(carry);
}

void BinomialQueue::insert(const std::pair<int, int> &v) {
    // Insert a new node into the queue. Please see insertTreeAtIndex
    // for details of the insert operation.

    // Pre-condition: heapOrderedTrees satisfies the Binomial Queue invariant.

    auto newTree = new BQnode(v);
    insertTreeAtIndex( newTree, 0 );
    numNodesInTheQueue++;
    // assertSizeOfBinomialQueue(); // comment out when in production!

}

std::pair<int, int> BinomialQueue::findMin() {
    assert(!empty());
    // Pre-condition -- the queue is not empty.
    return treeWithMinValue()->value();

}

void BinomialQueue::assertSizeOfBinomialQueue() {
    if( empty() && numNodesInTheQueue == 0)
        return;

    if( empty() ) {
        std::cout << "BinomialQueue::assertSizesOfBQTrees queue is empty, but it should have had ";
        std::cout << numNodesInTheQueue << " nodes. Terminating...\n";
        exit(1);
    }

    if( heapOrderedTrees.at(heapOrderedTrees.size() - 1) == nullptr ) {
        std::cout << "BinomialQueue::assertSizesOfBQTrees: the last element of heapOrderedTree is null.\n";
        std::cout << "This violates the Binomial Queue invariant. Terminating...\n";
        exit(2);
    }

    int expectedNumberOfNodes = 1;
    int totalNodes = 0;
    for( size_t i = 0; i < heapOrderedTrees.size(); i++ ) {
        if(heapOrderedTrees[i] == nullptr) {
            expectedNumberOfNodes *= 2;
            continue;
        }
        int numNodesInCurrentTree = assertSizeOfTree(heapOrderedTrees[i]);
        if( numNodesInCurrentTree != expectedNumberOfNodes ) {
            std::cout << "BinomialQueue::assertSizesOfBQTrees: number of nodes in tree with index " << i <<
                " is " << numNodesInCurrentTree << ".\n";
            std::cout << "It should have been " << expectedNumberOfNodes << ". Terminating..." << std::endl;
            exit(3);
        }
        totalNodes += numNodesInCurrentTree;
        expectedNumberOfNodes *= 2;
    }

    if( numNodesInTheQueue != totalNodes ) {
        std::cout << "The queue was supposed to have " << numNodesInTheQueue << "nodes.\n";
        std::cout << "It turns out to have " << totalNodes << " nodes. Terminating...\n";
        exit(3);
    }
}

int BinomialQueue::size() const {
    // numNodesInTheTree keeps track of the number of nodes in the queue. However,
    // we will verify that we actually have the right number of trees and the sum of
    // the number of nodes in the tree is in fact equal to numNodesInTheTree.

    if(numNodesInTheQueue == 0) {
        if(! empty() ) {
            std::cout << "The count of the nodes in the tree, based on inserts and delete is zero.\n";
            std::cout << "However, the queue is not empty. This is a bug! Terminating.\n";
            exit(1);
        }
        return 0;
    }

    int numNodes = 0;
    int treeSize = 1;
    for( std::size_t i = 0; i < heapOrderedTrees.size(); i++ ) {
        if( heapOrderedTrees.at(i) != nullptr )
            numNodes += treeSize;
        treeSize *= 2;
    }

    if( numNodes != numNodesInTheQueue ) {
        std::cout << "The number of nodes in the queue, based on the number of trees in the queue is " << numNodes << std::endl;
        std::cout << "However, the number of nodes based on inserts and deletes is " << numNodesInTheQueue << std::endl;
        std::cout << "These two counts do not match. That means the queue is not well constructed.\n";
        exit(1);
    }
    return numNodes;
}

int BinomialQueue::assertSizeOfTree(BQnode *tree) {
    if (tree == nullptr || tree->children().empty())
        return tree == nullptr ? 0 : 1;

    int expectedNumberOfNodes = 1;
    int totalNumNodes = 1;
    auto children = tree->children();
    for (std::size_t i = 0; i < children.size(); i++) {
        int thisChildsNumNodes = assertSizeOfTree(children[i]);
        if (thisChildsNumNodes != expectedNumberOfNodes) {
            std::cout << "No good...\n";
            exit(3);
        }
        totalNumNodes += thisChildsNumNodes;
        expectedNumberOfNodes *= 2;
    }
    return totalNumNodes;
}

void BinomialQueue::deleteMin() {
    // pre-condition -- empty() is false

    BQnode *minTree = treeWithMinValue();
    std::size_t minTreeIdx = 0;
    while( minTreeIdx < heapOrderedTrees.size() &&
            heapOrderedTrees.at(minTreeIdx) != minTree ) {
        minTreeIdx++;
    }

    if( minTreeIdx == heapOrderedTrees.size()) {
        // This is unexpected as minTree should not be nullptr;
        // it should point to one of the trees in our queue.
        // Need to throw ane exception here.
        std::cout << "Serious unexpected behavior in BinomialQueue::deleteMin.\n";
        return;
    }
    // minTree points to the tree that contains the min value of the queue
    // minTreeIdx is the index of minTree in heapOrderedTrees.
    // assert(minTreeIdx != heapOrderedTrees.size());
    heapOrderedTrees.at(minTreeIdx) = nullptr;
    while( !empty() && heapOrderedTrees.at(heapOrderedTrees.size() - 1) == nullptr)
        heapOrderedTrees.pop_back();

    auto children = minTree->children();
    for(std::size_t i = 0; i < children.size(); i++ ) {
        insertTreeAtIndex(children.at(i), i);
    }
    numNodesInTheQueue--;
    // assertSizeOfBinomialQueue();
}

bool BinomialQueue::empty() const {
    return heapOrderedTrees.empty();
}

void BinomialQueue::printHeapOrderedTree(BQnode *tree) {
    if( tree == nullptr )
        return;

    // print the value of the root node (tree)
    // printHeapOrderedTree(tree->leftSubtree())
    // printHeapOrderedTree(tree->rightSubtree())

    tree->print();
    for( auto child: tree->children() )
        printHeapOrderedTree(child);
}

void BinomialQueue::print() {
    for(auto tree: heapOrderedTrees )
        printHeapOrderedTree(tree);
}

BQnode *BinomialQueue::treeWithMinValue() {
    // Pre-condition -- the queue is not empty
    assert(!empty());
    BQnode *minTree = nullptr;
    for( auto treeRoot: heapOrderedTrees ) {
        if( treeRoot == nullptr )
            continue;
        if(minTree == nullptr)
            minTree = treeRoot;
        else if(treeRoot->value() < minTree->value())
            minTree = treeRoot;
        // else minTree has the minimum value among trees that we have inspected so far.
    }
    return minTree;
}

void BinomialQueue::deleteTree(BQnode *tree) {
    if( tree == nullptr ) {
        return;
    }

    if (tree->children().empty()) {
        delete tree;
        return;
    }

    for( auto child: tree->children() ) {
        deleteTree(child);
    }
    delete tree;
}


void BinomialQueue::deleteAllNodes() {
    for(auto tree: heapOrderedTrees )
        deleteTree(tree);
}

BinomialQueue::~BinomialQueue() {
    deleteAllNodes();
}