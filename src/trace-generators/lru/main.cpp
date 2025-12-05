//
// Created by Ali Kooshesh on 11/1/25.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <unordered_map>
#include <algorithm>
#include <random>
#include "../../../utils/TraceConfig.hpp"

std::vector<std::string> loadWords(const std::string& filename, std::size_t count) {
    std::vector<std::string> words;
    words.reserve(count);
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cerr << "Error: Cannot open words file: " << filename << std::endl;
        exit(1);
    }
    std::string word;
    while (words.size() < count && (in >> word)) {
        words.push_back(word);
    }
    if (words.size() < count) {
        std::cerr << "Warning: Only loaded " << words.size() << " words, needed " << count << std::endl;
    }
    return words;
}

std::vector<std::string> buildAccessStream(const std::vector<std::string>& allWords,
                                           std::size_t N, std::mt19937& rng) {
    std::vector<std::string> bag;
    bag.reserve(12 * N);

    for (std::size_t i = 0; i < N && i < allWords.size(); ++i) {
        bag.push_back(allWords[i]);
    }

    for (std::size_t i = N; i < 2 * N && i < allWords.size(); ++i) {
        for (int j = 0; j < 5; ++j) {
            bag.push_back(allWords[i]);
        }
    }

    for (std::size_t i = 2 * N; i < 4 * N && i < allWords.size(); ++i) {
        for (int j = 0; j < 3; ++j) {
            bag.push_back(allWords[i]);
        }
    }

    std::shuffle(bag.begin(), bag.end(), rng);
    return bag;
}

void generateLRUTrace(const unsigned seed,
                      const std::size_t n,
                      TraceConfig& config,
                      const std::vector<std::string>& allWords,
                      std::mt19937& rng) {
    auto outputFileName = config.makeTraceFileName(seed, n);
    std::cout << "File name: " << outputFileName << std::endl;
    std::ofstream out(outputFileName.c_str());
    if (!out.is_open()) {
        std::cerr << "Failed to open file " << outputFileName << std::endl;
        exit(1);
    }
    out << config.profileName << " " << n << " " << seed << std::endl;

    std::vector<std::string> accessStream = buildAccessStream(allWords, n, rng);

    std::list<std::string> lruList;
    std::unordered_map<std::string, std::list<std::string>::iterator> residentMap;

    for (const auto& w : accessStream) {
        auto it = residentMap.find(w);

        if (it != residentMap.end()) {
            lruList.erase(it->second);
            lruList.push_front(w);
            residentMap[w] = lruList.begin();
            out << "I " << w << "\n";
        }
        else if (residentMap.size() < n) {
            lruList.push_front(w);
            residentMap[w] = lruList.begin();
            out << "I " << w << "\n";
        }
        else {
            std::string victim = lruList.back();
            lruList.pop_back();
            residentMap.erase(victim);
            lruList.push_front(w);
            residentMap[w] = lruList.begin();
            out << "E " << victim << "\n";
            out << "I " << w << "\n";
        }
    }

    out.close();
}

int main(int argc, char* argv[]) {
    std::string wordsFile = "20980712_uniq_words.txt";
    if (argc >= 2) {
        wordsFile = argv[1];
    }

    TraceConfig config(std::string("lru_profile"));

    std::size_t maxN = *std::max_element(config.Ns.begin(), config.Ns.end());
    std::cout << "Loading words from " << wordsFile << "..." << std::endl;
    std::vector<std::string> allWords = loadWords(wordsFile, 4 * maxN);
    std::cout << "Loaded " << allWords.size() << " words." << std::endl;

    for (auto seed : config.seeds) {
        std::mt19937 rng(seed);
        for (auto n : config.Ns) {
            generateLRUTrace(seed, n, config, allWords, rng);
        }
    }

    std::cout << "\n ======= ALL LRU TRACES GENERATED ======" << std::endl;
    return 0;
}