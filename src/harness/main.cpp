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
#include <utility>


#include "Operation.hpp"
#include "RunResults.hpp"
#include "../implementations/BinaryHeapInVector/BinaryHeapInVector.hpp"
#include "../../utils/comparator.hpp"
#include "../implementations/BinomialQueues/BinomialQueue.hpp"
#include "../implementations/LinearBaseLine/LinearBaseLine.hpp"
#include "../implementations/Oracle/QuadraticOracle.hpp"

using namespace std;

// Assumes:
//  - struct Op { OpCode tag; int key; int id; }
//  - enum class OpCode { Insert, FindMin, DeleteMin, ExtractMin };
//  - struct RunResult { impl, trace_path, profile, N, seed, elapsed_ns, inserts, findmins, deletemins, extractmins, ... }
//  - The implementation 'Impl' has: clear(), insert(std::pair<int,int>), findMin(), deleteMin(), extractMin().

template<class Impl>
RunResult run_trace_ops(Impl &pq,
                        RunResult &runResult,
                        const std::vector<Operation> &ops) {
    // Count ops mostly for sanity check
    for (const auto &op: ops) {
        switch (op.tag) {
            case OpCode::Insert: ++runResult.inserts;
                break;
            case OpCode::FindMin: ++runResult.findMins;
                break;
            case OpCode::DeleteMin: ++runResult.deleteMins;
                break;
            case OpCode::ExtractMin: ++runResult.extractMins;
                break;
        }
    }
    // One untimed run

    pq.clear();
    std::cout << "Starting the throw-away run for N = " << runResult.run_meta_data.N << std::endl;
    for (const auto &op: ops) {
        switch (op.tag) {
            case OpCode::Insert:
                pq.insert(std::pair<int, int>{op.key, op.id});
                break;
            case OpCode::FindMin:
                (void) pq.findMin();
                break;
            case OpCode::DeleteMin:
                pq.deleteMin();
                break;
            case OpCode::ExtractMin:
                (void) pq.extractMin();
                break;
        }
    }

    using clock = std::chrono::steady_clock;
    const int numTrials = 7;

    std::vector<std::int64_t> trials_ns;
    trials_ns.reserve(numTrials);

    for (int i = 0; i < numTrials; ++i) {
        pq.clear();
        std::cout << "Run " << i << " for N = " << runResult.run_meta_data.N << std::endl;
        auto t0 = clock::now();
        for (const auto &op: ops) {
            switch (op.tag) {
                case OpCode::Insert:
                    pq.insert(std::pair<int, int>{op.key, op.id});
                    break;
                case OpCode::FindMin:
                    (void) pq.findMin();
                    break;
                case OpCode::DeleteMin:
                    pq.deleteMin();
                    break;
                case OpCode::ExtractMin:
                    (void) pq.extractMin(); // convenience op = findMin + deleteMin
                    break;
            }
        }
        auto t1 = clock::now();
        trials_ns.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
    }

    const size_t mid = trials_ns.size() / 2;     // the median of 0..numTrials
    std::nth_element(trials_ns.begin(), trials_ns.begin()+mid, trials_ns.end());
    runResult.elapsed_ns = trials_ns[mid];

    return runResult;
}

// Assumes OpCode/Op (with the two arguments) are defined

// The first line of the header must contain:  <profile> <N> <seed>
// After the header: blank lines and lines starting with '#' are okay
// and will be ignored.
// Opcodes: I <key> <id> | F | D | E

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
        std::string tok; //
        if (!(iss >> tok))
            continue;

        if (tok == "I") {
            int key, id;
            if (!(iss >> key >> id)) return false;
            out_operations.emplace_back(OpCode::Insert, key, id);
        } else if (tok == "F") {
            out_operations.emplace_back(OpCode::FindMin);
        } else if (tok == "D") {
            out_operations.emplace_back(OpCode::DeleteMin);
        } else if (tok == "E") {
            out_operations.emplace_back(OpCode::ExtractMin);
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
    for (const auto &entry: it) {
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
    vector<string> profiles = {"huffman_coding", "batch_then_drain"};
    vector<RunResult> runResults;

    for (const auto& profileName: profiles) {
        const auto traceDir = std::string("../../traces/") + "/" + profileName;

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

        for (auto traceFile: traceFiles) {
            const auto pos = traceFile.find_last_of("/\\");
            auto traceFileBaseName = (pos == std::string::npos) ? traceFile : traceFile.substr(pos + 1);

            std::vector<Operation> operations;
            RunMetaData run_meta_data;
            load_trace_strict_header(traceFile, run_meta_data, operations);

            if (run_meta_data.N < 1 << 16) {
                RunResult oneRunResult_i0(run_meta_data);
                QuadraticOracle oracle(compare_pair);
                oneRunResult_i0.impl = std::string("quadratic_oracle");
                oneRunResult_i0.trace_path = traceFileBaseName;
                run_trace_ops(oracle, oneRunResult_i0, operations);
                runResults.emplace_back(oneRunResult_i0);
            }

            RunResult oneRunResult_i1(run_meta_data);
            BinaryHeapInVector binaryHeap(compare_pair);
            oneRunResult_i1.impl = std::string("binary_heap");
            oneRunResult_i1.trace_path = traceFileBaseName;
            run_trace_ops(binaryHeap, oneRunResult_i1, operations);
            runResults.emplace_back(oneRunResult_i1);

            RunResult oneRunResult_i2(run_meta_data);
            BinomialQueue binomialQueue(compare_pair);
            oneRunResult_i2.impl = std::string("binomial_queue");
            oneRunResult_i2.trace_path = traceFileBaseName;
            run_trace_ops(binomialQueue, oneRunResult_i2, operations);
            runResults.emplace_back(oneRunResult_i2);

            RunResult oneRunResult_i3(run_meta_data);
            LinearBaseLine linear_base_line(compare_pair);
            oneRunResult_i3.impl = std::string("linear_base");
            oneRunResult_i3.trace_path = traceFileBaseName;
            run_trace_ops(linear_base_line, oneRunResult_i3, operations);
            runResults.emplace_back(oneRunResult_i3);

        }
    }
    if (runResults.size() == 0) {
        std::cerr << "No trace files found.\n";
        return 1;
    }
    std::cout << RunResult::csv_header() << std::endl;
    for (auto run: runResults) {
        std::cout << run.to_csv_row() << std::endl;
    }

    return 0;
}
