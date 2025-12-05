//
// Created by Ali A. Kooshesh on 5/9/21.
//

#ifndef HASHTABLESOPENADDRESSING_HASHTABLEDICTIONARY_HPP
#define HASHTABLESOPENADDRESSING_HASHTABLEDICTIONARY_HPP

#include<vector>
#include<string>

class HashTableDictionary {
    // enum { TABLE_SIZE = 3169 };
    enum ELEMENT_STATUS {AVAILABLE, DELETED, USED};

public:
    enum PROBE_TYPE {SINGLE, DOUBLE};

    HashTableDictionary( std::size_t tableSize_,
        PROBE_TYPE probeType, bool doCompact=false, double compactionTriggerRate=0.95);



    bool insert( const std::string& v );
    bool member( const std::string& v );
    bool remove( const std::string& v);
    [[nodiscard]] bool empty() const;
    [[nodiscard]] int size() const;
    void printStats() const;
    void printCounts();


    void printMask();
    void printMask(ELEMENT_STATUS status);


private:
    std::size_t  TABLE_SIZE;
    PROBE_TYPE probeType;

    std::vector<std::string> hashTable;
    std::vector<ELEMENT_STATUS> hashTableMask;
    std::vector<int> numProbes;
    std::vector<std::vector<std::pair<int, int>>> probePath;

    std::size_t primaryHashFunction( const std::string&  v );
    std::size_t secondaryHashFunction( const std::string&  v );
    std::size_t memberHelper( const std::string& v );
    [[nodiscard]] double effectiveLoadFactor() const;

    void compactTable();

    double compactionTriggerEffectiveRate = 0.95;

    bool shouldCompact = false;

    int numLookups = 0;
    int numDeletes = 0;
    int numInserts = 0;

    int numCompactions = 0;

    int numHits = 0;
    int numMisses = 0;
    int numFullScans = 0;

    std::int64_t totalProbes = 0;

    int numberOfActive = 0;
    int numberOfTombstones = 0;
    int maxTombstones = 0;

    int maxValuesInTable = 0;
};


#endif //HASHTABLESOPENADDRESSING_HASHTABLEDICTIONARY_HPP
