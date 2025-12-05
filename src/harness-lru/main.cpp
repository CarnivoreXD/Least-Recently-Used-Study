//
// Created by Ali Kooshesh on 11/1/25.
//

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <chrono>
#include "HashTableDictionary.hpp"
#include "Operations.hpp"

using namespace std;

struct RunMetaData {
    std::string profile;
    std::size_t N = 0;
    unsigned seed = 0;
};

struct RunResult {
    std::string impl;
    std::string trace_path;
    RunMetaData run_meta_data;
    double elapsed_ms = 0;
    std::size_t ops_total = 0;
    std::size_t inserts = 0;
    std::size_t erases = 0;
    std::string hash_table_stats;

    RunResult(const RunMetaData &meta) : run_meta_data(meta) {}

    static std::string csv_header() {
        return "impl,profile,trace_path,N,seed,elapsed_ms,ops_total,inserts,erases," +
               HashTableDictionary::csvStatsHeader();
    }

    std::string to_csv_row() const {
        return impl + "," +
               run_meta_data.profile + "," +
               trace_path + "," +
               std::to_string(run_meta_data.N) + "," +
               std::to_string(run_meta_data.seed) + "," +
               std::to_string(elapsed_ms) + "," +
               std::to_string(ops_total) + "," +
               std::to_string(inserts) + "," +
               std::to_string(erases) + "," +
               hash_table_stats;
    }
};

std::size_t tableSizeForN(std::size_t N) {
    static const std::vector<std::pair<std::size_t, std::size_t>> N_and_primes = {
        { 1024,    1279    },
        { 2048,    2551    },
        { 4096,    5101    },
        { 8192,    10273   },
        { 16384,   20479   },
        { 32768,   40849   },
        { 65536,   81931   },
        { 131072,  163861  },
        { 262144,  327739  },
        { 524288,  655243  },
        { 1048576, 1310809 }
    };
    for (const auto& item : N_and_primes) {
        if (item.first == N)
            return item.second;
    }
    return 0;
}

RunResult run_trace_ops(HashTableDictionary &ht,
                        RunResult &runResult,
                        const std::vector<Operation> &ops) {
    // Count ops mostly for sanity check
    for (const auto &op : ops) {
        switch (op.tag) {
            case OpCode::Insert: ++runResult.inserts;
                break;
            case OpCode::Erase: ++runResult.erases;
                break;
        }
    }
    runResult.ops_total = ops.size();
    // One untimed run
    ht.clear();
    std::cout << "Starting the throw-away run for N = " << runResult.run_meta_data.N << std::endl;
    for (const auto &op : ops) {
        switch (op.tag) {
            case OpCode::Insert:
                ht.insert(op.key);
                break;
            case OpCode::Erase:
                ht.remove(op.key);
                break;
        }
    }
    using clock = std::chrono::steady_clock;
    const int numTrials = 7;
    std::vector<double> trials_ms;
    trials_ms.reserve(numTrials);
    for (int i = 0; i < numTrials; ++i) {
        ht.clear();
        std::cout << "Run " << i << " for N = " << runResult.run_meta_data.N << std::endl;
        auto t0 = clock::now();
        for (const auto &op : ops) {
            switch (op.tag) {
                case OpCode::Insert:
                    ht.insert(op.key);
                    break;
                case OpCode::Erase:
                    ht.remove(op.key);
                    break;
            }
        }
        auto t1 = clock::now();
        trials_ms.push_back(std::chrono::duration<double, std::milli>(t1 - t0).count());
    }
    const size_t mid = trials_ms.size() / 2;
    std::nth_element(trials_ms.begin(), trials_ms.begin() + mid, trials_ms.end());
    runResult.elapsed_ms = trials_ms[mid];
    runResult.hash_table_stats = ht.csvStats();
    return runResult;
}

bool load_trace_strict_header(const std::string &path,
                              RunMetaData &runMeta,
                              std::vector<Operation> &out_operations) {
    std::string profile = "";
    int N = 0;
    int seed = 0;
    out_operations.clear();
    std::ifstream in(path);
    if (!in.is_open())
        return false;
    // --- read FIRST line as header
    std::string header;
    if (!std::getline(in, header))
        return false;
    // Look for a non-while-space character
    const auto first = header.find_first_not_of(" \t\r\n");
    // Since this is the first line, we don't expect it to be blank
    // or start with a comment.
    if (first == std::string::npos || header[first] == '#')
        return false;
    // Create a string stream so that we can read the profile name,
    // N, and the seed more easily.
    std::istringstream hdr(header);
    if (!(hdr >> profile >> N >> seed))
        return false;
    runMeta.profile = profile;
    runMeta.N = N;
    runMeta.seed = seed;
    // --- read ops, allowing comments/blank lines AFTER the header ---
    std::string line;
    while (std::getline(in, line)) {
        const auto opCodeIdx = line.find_first_not_of(" \t\r\n");
        if (opCodeIdx == std::string::npos || line[opCodeIdx] == '#')
            continue; // skip blank and comment lines.
        std::istringstream iss(line.substr(opCodeIdx));
        std::string tok;
        if (!(iss >> tok))
            continue;
        std::string word;
        if (tok == "I") {
            if (!(iss >> word)) return false;
            out_operations.emplace_back(OpCode::Insert, word);
        } else if (tok == "E") {
            if (!(iss >> word)) return false;
            out_operations.emplace_back(OpCode::Erase, word);
        } else {
            return false; // unknown token
        }
    }
    return true;
}

void find_trace_files_or_die(const std::string &dir,
                             const std::string &profile_prefix,
                             std::vector<std::string> &out_files) {
    namespace fs = std::filesystem;
    out_files.clear();
    std::error_code ec;
    fs::path p(dir);
    if (!fs::exists(p, ec)) {
        std::cerr << "Error: directory '" << dir << "' does not exist";
        if (ec) std::cerr << " (" << ec.message() << ")";
        std::cerr << "\n";
        std::exit(1);
    }
    if (!fs::is_directory(p, ec)) {
        std::cerr << "Error: path '" << dir << "' is not a directory";
        if (ec) std::cerr << " (" << ec.message() << ")";
        std::cerr << "\n";
        std::exit(1);
    }
    fs::directory_iterator it(p, ec);
    if (ec) {
        std::cerr << "Error: cannot iterate directory '" << dir << "': "
                << ec.message() << "\n";
        std::exit(1);
    }
    const std::string suffix = ".trace";
    for (const auto &entry : it) {
        if (!entry.is_regular_file(ec)) {
            if (ec) {
                std::cerr << "Error: is_regular_file failed for '"
                        << entry.path().string() << "': " << ec.message() << "\n";
                std::exit(1);
            }
            continue;
        }
        const std::string name = entry.path().filename().string();
        const bool has_prefix = (name.rfind(profile_prefix, 0) == 0);
        const bool has_suffix = name.size() >= suffix.size() &&
                                name.compare(name.size() - suffix.size(),
                                             suffix.size(), suffix) == 0;
        if (has_prefix && has_suffix) {
            out_files.push_back(entry.path().string());
        }
    }
    std::sort(out_files.begin(), out_files.end()); // stable order for reproducibility
}

int main() {
    vector<string> profiles = {"lru_profile"};
    vector<RunResult> runResults;
    std::ofstream outputFile("../../csvs/hashmapProfile.csv");
    for (const auto& profileName : profiles) {
        const auto traceDir = std::string("../../traces/") + profileName;
        vector<string> traceFiles;
        find_trace_files_or_die(traceDir, profileName, traceFiles);
        /*
        for (auto file: traceFiles) {
            std::cout << file << "\n";
        }
        */
        if (traceFiles.size() == 0) {
            std::cerr << "No trace files found for: " << profileName << std::endl;
            continue;
        }
        for (auto traceFile : traceFiles) {
            const auto pos = traceFile.find_last_of("/\\");
            auto traceFileBaseName = (pos == std::string::npos) ? traceFile : traceFile.substr(pos + 1);
            std::vector<Operation> operations;
            RunMetaData run_meta_data;
            load_trace_strict_header(traceFile, run_meta_data, operations);

            std::size_t tableSize = tableSizeForN(run_meta_data.N);
            if (tableSize == 0) {
                std::cerr << "Skipping N = " << run_meta_data.N << " (no table size mapping)" << std::endl;
                continue;
            }

            RunResult oneRunResult_i0(run_meta_data);
            HashTableDictionary hashTable_single(tableSize, HashTableDictionary::SINGLE, true);
            oneRunResult_i0.impl = std::string("hash_map_single");
            oneRunResult_i0.trace_path = traceFileBaseName;
            run_trace_ops(hashTable_single, oneRunResult_i0, operations);
            runResults.emplace_back(oneRunResult_i0);
            if (run_meta_data.N == 2048 || run_meta_data.N == 32768) {
                std::ofstream mapFile("../../maps/single_N" + std::to_string(run_meta_data.N) + ".txt");
                if (mapFile.is_open()) {
                    mapFile << "compaction_on single_probing " << tableSize << "\n\n";
                    mapFile << hashTable_single.beforeCompactionMap << "\n\n";
                    mapFile << hashTable_single.afterCompactionMap;
                    mapFile.close();
                }
            }
            

            RunResult oneRunResult_i1(run_meta_data);
            HashTableDictionary hashTable_double(tableSize, HashTableDictionary::DOUBLE, true);
            oneRunResult_i1.impl = std::string("hash_map_double");
            oneRunResult_i1.trace_path = traceFileBaseName;
            run_trace_ops(hashTable_double, oneRunResult_i1, operations);
            runResults.emplace_back(oneRunResult_i1);

            if (run_meta_data.N == 2048 || run_meta_data.N == 32768) {
                std::ofstream mapFile("../../maps/double_N" + std::to_string(run_meta_data.N) + ".txt");
                if (mapFile.is_open()) {
                    mapFile << "compaction_on double_probing " << tableSize << "\n\n";
                    mapFile << hashTable_single.beforeCompactionMap << "\n\n";
                    mapFile << hashTable_single.afterCompactionMap;
                    mapFile.close();
                }
            }
        }
    }

    if (!outputFile.is_open()){
        std::cerr << "Error: could not open the file for writing" <<  std::endl;
        return 1;
    }
    if (runResults.size() == 0) {
        std::cerr << "No trace files found.\n";
        return 1;
    }
    outputFile << RunResult::csv_header() << std::endl;
    for (auto run : runResults) {
        outputFile << run.to_csv_row() << std::endl;
    }
    
    outputFile.close();

    std::cout << "Data successfully written to data.csv" << std::endl;
    return 0;
}