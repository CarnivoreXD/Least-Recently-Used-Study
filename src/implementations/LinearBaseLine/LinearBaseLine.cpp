//
// Created by Ali Kooshesh on 11/5/25.
//

#include <cassert>
#include "LinearBaseLine.hpp"

LinearBaseLine::LinearBaseLine(bool(*comp)(const std::pair<int, int> &, const std::pair<int, int> &)) {
    priorityFunc = comp;
}

LinearBaseLine::LinearBaseLine(const std::vector<std::pair<int, int>> &data,
    bool(*comp)(const std::pair<int, int> &, const std::pair<int, int> &)) {
    for (auto item: data) {
        _heap.emplace_back(item.first, item.second);
    }
    priorityFunc = comp;
}

void LinearBaseLine::insert(const std::pair<int, int> &v) {
    _heap.emplace_back(v.first, v.second);
}

void LinearBaseLine::deleteMin() {
    assert(!empty());
    _heap.pop_back();
}

const std::pair<int, int> & LinearBaseLine::findMin() const {
    assert(!empty());
    return _heap.back();
}

std::pair<int, int> LinearBaseLine::extractMin() {
    assert(!empty());
    auto min = _heap.back();
    _heap.pop_back();
    return min;
}

bool LinearBaseLine::empty() const noexcept {
    return _heap.empty();
}

std::size_t LinearBaseLine::size() const noexcept {
    return _heap.size();
}

void LinearBaseLine::clear() noexcept {
    _heap.clear();
}
