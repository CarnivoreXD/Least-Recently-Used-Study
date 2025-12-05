//
// Created by Ali Kooshesh on 10/31/25.
//

#pragma once
#include <vector>
#include <functional>
#include <cstddef>


class BinaryHeapInVector {
public:

    // constructors (exactly two)
    BinaryHeapInVector(bool (*)(const std::pair<int, int> &, const std::pair<int, int> &));
    explicit BinaryHeapInVector(const std::vector<std::pair<int, int>>& data,
        bool (*)(const std::pair<int, int> &, const std::pair<int, int> &));  // bulk build

    void insert(const std::pair<int, int>& v);
    void deleteMin();
    const std::pair<int, int>& findMin() const;
    std::pair<int, int> extractMin();

    // inspectors / modifiers
    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] std::size_t size() const noexcept;
    void clear() noexcept;
    void reserve(std::size_t n); // not implemented


private:
    std::vector<std::pair<int, int>> _heap;
    bool (* priorityFunc)(const std::pair<int, int> &a, const std::pair<int, int> &b);
    void siftDown(int i);
};
