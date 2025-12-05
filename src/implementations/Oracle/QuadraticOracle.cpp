//
// Created by Ali Kooshesh on 11/5/25.
//

#include "QuadraticOracle.hpp"
#include <cassert>
#include<iostream>


QuadraticOracle::QuadraticOracle(bool(*comp)(const std::pair<int, int> &, const std::pair<int, int> &)) {
    priorityFunc = comp;
}

QuadraticOracle::QuadraticOracle(const std::vector<std::pair<int, int>> &data,
    bool(*comp)(const std::pair<int, int> &, const std::pair<int, int> &)) {
    priorityFunc = comp;
    for (auto item: data) {
        insert(item);
    }
}

bool QuadraticOracle::isSorted() {

    for (std::size_t i = 1; i < _heap.size(); i++) {
        assert(priorityFunc(_heap.at(i), _heap.at(i-1)));
    }
    return true;
}


void QuadraticOracle::insert(const std::pair<int, int> &v) {

    if (_heap.empty()) {
        _heap.emplace_back(v.first, v.second);
        return;
    }

    _heap.emplace_back(v.first, v.second);
    auto cur_idx = static_cast<int>(_heap.size()) - 2;
    while (cur_idx >= 0 && priorityFunc( _heap.at(cur_idx), v) ) {
        _heap.at(cur_idx + 1 ) = _heap.at(cur_idx);
        --cur_idx;
    }
    _heap.at(cur_idx + 1 ) = v;
//    std::cout << "After insert. the first and the last elements\n";
//    std::cout << _heap.at(0).first << " " << _heap.at(_heap.size()-1).first << std::endl;

    assert(isSorted());
}

void QuadraticOracle::deleteMin() {
    assert(!empty());
    _heap.pop_back();
}

const std::pair<int, int> & QuadraticOracle::findMin() const {
    assert(!empty());
    return _heap.back();
}

std::pair<int, int> QuadraticOracle::extractMin() {
    assert(!empty());
    auto min = _heap.back();
    _heap.pop_back();
    return min;
}

bool QuadraticOracle::empty() const noexcept {
    return _heap.empty();
}

std::size_t QuadraticOracle::size() const noexcept {
    return _heap.size();
}

void QuadraticOracle::clear() noexcept {
    _heap.clear();
}
