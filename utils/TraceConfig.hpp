//
// Created by Ali Kooshesh on 11/1/25.
//

#ifndef PRIORITY_QUEUE_STUDY_TRACECONFIG_HPP
#define PRIORITY_QUEUE_STUDY_TRACECONFIG_HPP

#include<string>
#include<vector>
#include<cassert>
#include<iostream>

struct TraceConfig {

    explicit TraceConfig(const std::string &pName):
        profileName(pName) {
        Ns.push_back(13);      // This is modeled after lab07 data which had 13 unique words.
        // Generates N = 2^10, 2^11, ..., 2^20
        constexpr int start_exp = 10, end_exp = 20;
        for (int exp = start_exp; exp <= end_exp; exp++)
            Ns.push_back(1 << exp);

    }


    std::vector<unsigned> seeds = {23};  // only one seed to get started.
    std::vector<unsigned> Ns;
    std::string traceDirectory = "../../../traces"; // awkward!
    std::string implementationName;
    std::string profileName;

    std::string makeTraceFileName(const unsigned int seed, const unsigned n) {
        assert(profileName != "");
        return traceDirectory + "/" +
                profileName + "/" +
                profileName +  "_N_" + std::to_string(n) + "_S_" + std::to_string(seed) + ".trace";
    }
};


#endif //PRIORITY_QUEUE_STUDY_TRACECONFIG_HPP