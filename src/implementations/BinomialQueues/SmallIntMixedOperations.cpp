//
// Created by Ali A. Kooshesh on 5/9/21.
//

#include "SmallIntMixedOperations.hpp"
#include<iostream>
#include <cstdint>

int SmallIntMixedOperations::minValue() {
    if(empty())
        return INT32_MAX;

    int smallest = verifier.at(0);
    for( auto v: verifier )
        if( v < smallest )
            smallest = v;
    return smallest;
}
