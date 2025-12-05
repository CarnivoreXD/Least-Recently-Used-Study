//
// Created by Kooshesh on 11/4/25.
//


#pragma once
#include<tuple>

inline bool comparator(std::pair<int, int> v1, std::pair<int, int> v2) {
    if(v1.first != v2.first)
        return v1.first > v2.first;
    return v1.second < v2.second;
}