//
// Created by Ali Kooshesh on 11/1/25.
//

#include <iostream>
#include <fstream>

#include "../../../utils/TraceConfig.hpp"
#include<random>
#include<iomanip>

void generateTrace(const unsigned seed,
    const std::size_t n,
    TraceConfig &config,
    std::uniform_int_distribution<int> &dist,
    std::mt19937& gen ) {

    // create and open the output file name
    auto outputFileName = config.makeTraceFileName(seed, n);
    std::cout << "File name: " << outputFileName << std::endl;
    std::ofstream out(outputFileName.c_str());
    if (!out.is_open()) {
        std::cerr << "Failed to open file " << outputFileName << std::endl;
        exit(1);
    }
    out << config.profileName << " " << n << " " << seed << std::endl;

    // Generate N inserts.
    unsigned id = 0;        // id serves as a tiebreaker. Don't use the loop variable for
                            // this purpose because we have multiple loops and could
                            // accidentally generate duplicate IDs.
    int spaceBeforeNumber = 10;
    for (unsigned i = 0; i < n; ++i) {
        out << "I " << std::setw(spaceBeforeNumber) << dist(gen) << std::setw(spaceBeforeNumber) << id++ << "\n";
    }
    // Generate N-1 extractMin, extractMin, insert
    const unsigned stop_idx = 2 * n - 1;
    for (unsigned i = n; i < stop_idx; ++i) {
        out << "E\nE\n";
        out << "I " << std::setw(spaceBeforeNumber) << dist(gen) << std::setw(spaceBeforeNumber) << id++ << "\n";
    }
    out.close();
}

int choose_key_upper_bound(unsigned int N) {
    // You can change the upperbound to
    // see how that effects the frequency (key)
    // that get generated.

    if (N <= 50)   // this is for the benefit of our small data set.
        return 3;
    if (N <= (1u << 12))
        return 12;
    if (N <= (1u << 15))
        return 32;
    if (N <= (1u << 18))
        return 64;
    return 128; // for 2^19 and 2^20
}


void generateBatchTrace(const unsigned seed,
    const std::size_t n,
    TraceConfig &config,
    std::uniform_int_distribution<int> &dist,
    std::mt19937& gen ) {

    // create and open the output file name
    auto outputFileName = config.makeTraceFileName(seed, n);
    std::cout << "File name: " << outputFileName << std::endl;
    std::ofstream out(outputFileName.c_str());
    if (!out.is_open()) {
        std::cerr << "Failed to open file " << outputFileName << std::endl;
        exit(1);
    }
    out << config.profileName << " " << n << " " << seed << std::endl;

    // Generate N inserts.
    unsigned id = 0;        // id serves as a tiebreaker. Don't use the loop variable for
    // this purpose because we have multiple loops and could
    // accidentally generate duplicate IDs.
    int spaceBeforeNumber = 10;
    for (unsigned i = 0; i < n; ++i) {
        out << "I " << std::setw(spaceBeforeNumber) << dist(gen) << std::setw(spaceBeforeNumber) << id++ << "\n";
    }
    // Generate N-1 extractMin, extractMin, insert
    for (unsigned i = 0; i < n; ++i)
    {
        out << "E\n";
    }

    out.close();
}

int main() {

    // TraceConfig provides pre-configured values such as N and seed
    {
        TraceConfig config( std::string("huffman_coding"));
        for (auto seed: config.seeds) {  // currently, we are using one seed only.
            std::mt19937 rng(seed);   // create a random-number generator using "seed"

            for (auto n: config.Ns) {
                // We need to limit the range of values that get generated.
                // To model the Huffman tree behavior, we want to generate a lot of
                // duplicates. So, we use choose_key_upper_bound to increase
                // the upperbound according to the size of the trace.
                const unsigned key_min = 1, key_max = choose_key_upper_bound(n);
                std::uniform_int_distribution<int> dist(key_min, key_max);

                generateTrace(seed, n, config, dist, rng);
            }

        }
    }


    TraceConfig config( std::string("batch_then_drain"));
    for (auto seed: config.seeds) {  // currently, we are using one seed only.
        std::mt19937 rng(seed);   // create a random-number generator using "seed"

        for (auto n: config.Ns) {
            // We need to limit the range of values that get generated.
            // To model the Huffman tree behavior, we want to generate a lot of
            // duplicates. So, we use choose_key_upper_bound to increase
            // the upperbound according to the size of the trace.
            const unsigned key_min = 1, key_max = (1u << 20);
            std::uniform_int_distribution<int> dist(key_min, key_max);

            generateBatchTrace(seed, n, config, dist, rng);
        }

    }



    std::cout << "\n ======= ALL TRACES GENERATED ======" << std::endl;
    return 0;
}