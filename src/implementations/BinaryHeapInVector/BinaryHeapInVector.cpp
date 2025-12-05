//
// Created by Ali Kooshesh on 10/31/25.
//

#include "BinaryHeapInVector.hpp"

#include <cassert>
#include <utility>   // std::swap


BinaryHeapInVector::BinaryHeapInVector(bool (*comp)(const std::pair<int, int> &, const std::pair<int, int> &)):
  priorityFunc(comp) {
  // 1-based indexing sentinel at index 0
  _heap.push_back({0, 0});
}

void BinaryHeapInVector::clear() noexcept{
  _heap.clear();
  _heap.push_back({0, 0});

}

BinaryHeapInVector::BinaryHeapInVector(const std::vector<std::pair<int, int>>& data,
  bool (*comp)(const std::pair<int, int> &, const std::pair<int, int> &)) {
  priorityFunc = comp;

  // We intentionally do not use a move constructor so that when
  // using multiple queues to compare the run-time of operations
  // for multiple queues that provide the same interface.

  for (auto &v: data)
    _heap.emplace_back(v);

  // Heapify (1-based indexing). Percolate down from floor(size/2) … 1.
  for (std::size_t i = size() / 2; i > 0; --i) {
    siftDown(i);
  }
  
}

void BinaryHeapInVector::insert(const std::pair<int, int>& v) {
  _heap.push_back(v);

  // percolate up (1-based)
  size_t childIdx  = _heap.size() - 1;
  size_t parentIdx = childIdx / 2;

  while (parentIdx > 0 && priorityFunc(_heap.at(childIdx), _heap.at(parentIdx))) {
    std::swap(_heap.at(parentIdx), _heap.at(childIdx));
    childIdx  = parentIdx;
    parentIdx = childIdx / 2;
  }
}

void BinaryHeapInVector::siftDown(int i) {
  // percolate down from root (1-based)
  size_t parentIdx = i;
  size_t childIdx  = 2 * parentIdx;

  while (childIdx <= size()) {
    if (childIdx + 1 <= size() &&
        priorityFunc(_heap.at(childIdx + 1), _heap.at(childIdx))) {
      ++childIdx;
        }
    if (!priorityFunc(_heap.at(childIdx), _heap.at(parentIdx)))
      break;
    std::swap(_heap.at(childIdx), _heap.at(parentIdx));
    parentIdx = childIdx;
    childIdx  = 2 * parentIdx;
  }


}


void BinaryHeapInVector::deleteMin() {
  // Precondition: not empty (ignoring sentinel)
  assert(!empty());

  // Move last element to root and shrink
  _heap.at(1) = std::move(_heap.back());
  _heap.pop_back();
  siftDown(1);

}

const std::pair<int, int>& BinaryHeapInVector::findMin() const {
  // Precondition: not empty
  assert(!empty());
  return _heap.at(1);
}

std::pair<int, int> BinaryHeapInVector::extractMin() {
  assert(!empty());
  std::pair<int, int> minCopy = _heap.at(1); // return by value (simple and safe)
  deleteMin();
  return minCopy;
}


bool BinaryHeapInVector::empty() const noexcept {
  return size() == 0;
}


std::size_t BinaryHeapInVector::size() const noexcept {
  return _heap.size() - 1; // exclude sentinel
}




