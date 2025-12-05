#include <iostream>

#include <tuple>
#include <fstream>
#include <vector>
#include <utility>

#include "BinaryHeapInVector.hpp"
#include "../../../utils/comparator.hpp"



int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " nameOfAnInputFile\n";
        std::exit(1);
    }

    std::ifstream input(argv[1]);
    if (!input) {
        std::cout << "Unable to open " << argv[1] << ". Terminating...\n";
        std::exit(1);
    }

    // Read (key, id) pairs
    std::vector<std::pair<int, int>> items;
    int key, id;
    while (input >> key >> id) {
        items.emplace_back(key, id);
    }

    // Build heap by copying the vector, then heapify (O(N))
    BinaryHeapInVector heap(items, compare_pair);

    // Drain: print key and id per line (min first)
    while (!heap.empty()) {
        const std::pair<int, int>& m = heap.findMin();
        std::cout << m.first << ' ' << m.second << '\n';
        heap.deleteMin();
    }

    return 0;
}
