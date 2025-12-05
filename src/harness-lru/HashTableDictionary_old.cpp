//
// Created by Ali A. Kooshesh on 5/9/21.
//

#include "HashTableDictionary.hpp"
#include<iostream>
#include<iomanip>
#include<algorithm>
#include<cassert>

HashTableDictionary::HashTableDictionary(std::size_t large, PROBE_TYPE pType, bool doCompact, double compactionFloor):
    TABLE_SIZE{large}, probeType{pType}, compactionTriggerEffectiveRate(compactionFloor), shouldCompact {doCompact} {
    hashTable.resize(large);
    hashTableMask.resize(large, AVAILABLE);
}

double HashTableDictionary::effectiveLoadFactor() const {
    return static_cast<double>(numberOfTombstones + numberOfActive) / static_cast<double>(TABLE_SIZE);
}


bool HashTableDictionary::insert( const std::string&  v ) {
    // Returns whether the insert was successful.

    if( numberOfActive == TABLE_SIZE) {
        std::cout << "Table is full. This is a serious problem. Terminating\n";
        printStats();
        exit(1);
    }

    const std::size_t idx = memberHelper(v);
    if (hashTableMask.at(idx) == USED && hashTable.at(idx) == v)
        return false;

    assert(hashTableMask.at(idx) != USED);

    hashTable.at(idx) = v;
    if (hashTableMask.at(idx) == DELETED)
        numberOfTombstones--;
    hashTableMask.at(idx) = USED;
    numberOfActive++;
    numInserts++;

    if (maxValuesInTable < numberOfActive)
        maxValuesInTable = numberOfActive;


    if (shouldCompact && effectiveLoadFactor() > compactionTriggerEffectiveRate) {
        std::cout << "Compacting the table...\n";
        // printMask();
        compactTable();
        numCompactions++;
    }

    return true;
}

int HashTableDictionary::size() const {
    return numberOfActive;
}

bool HashTableDictionary::remove(const std::string& v) {
    auto idx = memberHelper(v);
    if( hashTableMask.at(idx) != USED )
        return false;

    if (numberOfActive == TABLE_SIZE && hashTable.at(idx) != v) {
        std::cout << "Returning from remove because table is full and the item is not in the table.\n";
        return false;
    }

    numberOfTombstones++;
    maxTombstones = std::max(numberOfTombstones, maxTombstones);
    hashTableMask.at(idx) = DELETED;
    numberOfActive--;
    numDeletes++;

    return true;
}

void HashTableDictionary::compactTable() {

    if (hashTable.size() == 0)
        return;

    std::vector<std::string> newTable(hashTable.size());
    std::vector<ELEMENT_STATUS> newMask(hashTableMask.size(), AVAILABLE);

    std::cout << "Before compacting the table:\n";
    std::cout << "\tNumber of active cells: " << numberOfActive << std::endl;
    std::cout << "\tNumber of tombstone cells: " << numberOfTombstones << std::endl;
    std::cout << "\tNumber of available cells: " << hashTable.size()-numberOfTombstones-numberOfActive << std::endl;
    std::cout << "\tEffective load factor: " << effectiveLoadFactor() << std::endl;
    auto curNumProbes = totalProbes;
    auto curNumInserts = numInserts;
    hashTable.swap(newTable);
    hashTableMask.swap(newMask);
    numberOfActive = 0;
    numberOfTombstones = 0;

    for (std::size_t i = 0; i < newTable.size(); i++) {
        if (newMask.at(i) == USED)
            insert(newTable.at(i));
    }
    numInserts = curNumInserts;
    totalProbes = curNumProbes;
    std::cout << "\nAfter compacting the table:\n";
    std::cout << "\tNumber of active cells: " << numberOfActive << std::endl;
    std::cout << "\tNumber of tombstone cells: " << numberOfTombstones << std::endl;
    std::cout << "\tNumber of available cells: " << hashTable.size()-numberOfTombstones-numberOfActive << std::endl;
    std::cout << "\tEffective load factor: " << effectiveLoadFactor() << std::endl;

}

std::size_t HashTableDictionary::memberHelper(const std::string& v) {

    std::size_t idx = primaryHashFunction( v );
    std::size_t step = secondaryHashFunction( v );
    int numProbesForThisItem = 1;  // Accounting for the fact that the while loop's condition tests the table.
    std::size_t firstDeleteIdx = hashTable.size();

    while( numProbesForThisItem < TABLE_SIZE && hashTableMask.at(idx) != AVAILABLE &&
            ( hashTableMask.at(idx) == DELETED || hashTable.at(idx) != v ) ) {
        if( hashTableMask.at(idx) == DELETED && firstDeleteIdx == hashTable.size() ) {
            firstDeleteIdx = idx;
        }
        idx = (idx + step) % TABLE_SIZE;
        numProbesForThisItem++;
    }
    // std::cout << std::setw(6) << numComparisons << " comps\n";
    totalProbes += numProbesForThisItem;
    if (numProbesForThisItem == TABLE_SIZE) {
        numFullScans++;
    }
    return hashTableMask.at(idx) == USED && hashTable.at(idx) == v ? idx : (firstDeleteIdx != hashTable.size() ? firstDeleteIdx : idx);
}

bool HashTableDictionary::member(const std::string& v )  {
    // Returns true if v a member. Otherwise, it returns false

    auto idx = memberHelper(v);
    numLookups++;
    return  hashTableMask.at(idx) == USED && hashTable.at(idx) == v;
}

bool HashTableDictionary::empty() const {
    return numberOfActive == 0;
}

void HashTableDictionary::printStats() const {


    const int width = 8;
    std::cout << std::setw(width) << TABLE_SIZE << " table size: " << std::endl;
    std::cout << std::setw(width) << numberOfTombstones << " cells marked as deleted."  << std::endl;
    std::cout << std::setw(width) << numberOfActive << " active cells."  << std::endl;
    std::cout << std::setw(width) << TABLE_SIZE - maxTombstones - numberOfActive << " available elements.\n";
    std::cout << std::setw(width) << maxValuesInTable << " maximum number of values in the table ever." << std::endl;
    std::cout << std::setw(width) << totalProbes << " total probes." << std::endl;

    std::cout << std::endl;
    std::cout << std::setw(width) << numInserts << " inserts."  << std::endl;
    std::cout << std::setw(width) << numDeletes << " deletes."  << std::endl;
    std::cout << std::setw(width) << numLookups << " lookups."  << std::endl;
    std::cout << std::setw(width) << numFullScans << " full scans."  << std::endl;
    std::cout << std::setw(width) << numCompactions << " compactions."  << std::endl;
    std::cout << std::endl;
    std::cout << std::setw(width) << static_cast<int>(static_cast<double>(TABLE_SIZE - numberOfTombstones - numberOfActive) / static_cast<double>(TABLE_SIZE) * 100) <<
        "% ratio of available elements." << std::endl;


    std::cout << std::setw(width) << static_cast<int>(static_cast<double>(numberOfActive) / static_cast<double>(TABLE_SIZE) * 100) <<
        "% load factor." << std::endl;

    std::cout << std::setw(width) << static_cast<int>(static_cast<double>(numberOfActive + numberOfTombstones) / static_cast<double>(TABLE_SIZE) * 100) <<
        "% effective load factor." << std::endl;


    std::cout << std::setw(width) << static_cast<int>(static_cast<double>(numberOfTombstones) / static_cast<double>(TABLE_SIZE) * 100) << "% tombstone fraction." <<  std::endl;

    std::cout << std::endl;

    std::cout << static_cast<double>(totalProbes) / static_cast<double>(numInserts + numDeletes + numLookups) <<
     " average number of probes";

    if (probeType == SINGLE)
        std::cout << " (single probing, " << (shouldCompact ? "compaction on)." : "compaction off).") << std::endl;
    else
        std::cout << " (double probing, " << (shouldCompact ? "compaction on)." : "compaction off).") << std::endl;

}

void HashTableDictionary::printCounts() {
    std::cout << std::setw(20) << ' ';
    for(size_t i = 0; i < TABLE_SIZE; i++) {
        std::cout << std::setw(5) << i;
    }
    std::cout << std::endl << std::setw(20) << ' ';
    for(size_t i = 0; i < TABLE_SIZE; i++) {
        if(hashTableMask.at(i) == USED)
            std::cout << std::setw(5) << hashTable.at(i);
        else
            std::cout << std::setw(5) << "-";
    }

    std::cout << std::endl;

    for(size_t i = 0; i < TABLE_SIZE; i++) {
        if (hashTableMask.at(i) == USED) {
            std::cout << std::setw(7) << i << std::setw(7) << hashTable.at(i) << " (" << std::setw(2) << probePath.at(i).size() << ')';
            std::sort(probePath.at(i).begin(), probePath.at(i).end());
            int prevIdx = 0;
            std::cout << std::setw(5) << ' ';
            for(size_t j = 0; j < probePath.at(i).size(); j++) {
                auto [idx, seqNum] = probePath.at(i).at(j);
                std::cout << std::setw(5 * (idx - prevIdx)) << seqNum;
                prevIdx = idx;
            }

            std::cout << std::endl;
        }

    }

    std::cout << "\nindex, value, num probes\n";

    for(int i = 0; i < TABLE_SIZE; i++)
            if(hashTableMask.at(i) == USED)
                std::cout << std::setw(4) << i << std::setw(4) << hashTable.at(i) << std::setw(6) << numProbes.at(i) << std::endl;
}


std::size_t HashTableDictionary::primaryHashFunction(const std::string& v) {

    std::size_t idx = 0;
    for (unsigned char c : v) {
        idx = (idx * 131 + c) % TABLE_SIZE;     // base 131
    }
    return idx;                                 // 0..LARGE_TWIN-1
}


std::size_t HashTableDictionary::secondaryHashFunction(const std::string& v) {
    if (probeType == SINGLE)
        return 1;                // linear probing


    std::size_t idx = 0;
    for (unsigned char c : v) {
        idx = (idx * 257 + c) % (TABLE_SIZE - 1);      // base 257
    }
    return 1 + idx;                                    // 1..LARGE_TWIN-1  (gcd(step, LARGE_TWIN)=1)
}

void inRed(char c) {
    std::cout << "\x1b[31m" << c << "\x1b[0m";
}

void inBlue(char c) {
    std::cout << "\x1b[34m" << c << "\x1b[0m";
}
void inYellow(char c) {
    std::cout << "\x1b[33m" << c << "\x1b[0m";
}

void inGreen(char c) {
    std::cout << "\x1b[32m" << c << "\x1b[0m";
}

void inMagenta(char c) {
    std::cout << "\x1b[35m" << c << "\x1b[0m";
}

void inCyan(char c) {
    std::cout << "\x1b[36m" << c << "\x1b[0m";
}

void HashTableDictionary::printMask(ELEMENT_STATUS es) {
    for(size_t i = 0; i < TABLE_SIZE; i++) {
        if(hashTableMask.at(i) == USED)
            inRed(es == USED ? '-' : ' ');
        else if (hashTableMask.at(i) == AVAILABLE)
            inYellow(es == AVAILABLE ? '-' : ' ');
        else if( hashTableMask.at(i) == DELETED)
            inGreen(es == DELETED ? '-' : ' ');
        else {
            std::cout << "\nUnrecognize element type with value: " << hashTableMask.at(i) << "." << std::endl;
            exit(1);
        }
        if(  (i + 1) % 100 == 0)
            std::cout << '.' << std::endl;
    }
    std::cout << std::endl;
}

void HashTableDictionary::printMask() {
    std::cout << "Elements in use map.\n";
    printMask(USED);
    std::cout << "\nElements deleted map.\n";
    printMask(DELETED);
    std::cout << "\nElements available map.\n";
    printMask(AVAILABLE);
}
