//
// Created by Ali Kooshesh on 11/1/25.
//

#pragma once
#include <cassert>

enum class OpCode {
    Insert,     // I key id
    FindMin,    // F
    DeleteMin,  // D
    ExtractMin  // E (findMin + deleteMin)
  };

struct Operation {
    OpCode tag;
    int key;   // meaningful only for Insert
    int id;    // meaningful only for Insert

    // Constructor for non-insert ops: F, D, E
    explicit Operation(OpCode op_code) : tag(op_code), key(0), id(0) {
        assert(op_code != OpCode::Insert);
    }

    // Constructor for insert ops: I key id
    Operation(OpCode op_code, int k, int i) : tag(op_code), key(k), id(i) {
        assert(op_code == OpCode::Insert);
    }

    void print() const {
        switch (tag) {
            case OpCode::Insert:
                std::cout << "I" << " " << key << " " << id << std::endl;
                break;
            case OpCode::FindMin:
                std::cout << "F" << std::endl;
                break;

            case OpCode::DeleteMin:
                std::cout << "D" << std::endl;
                break;
            case OpCode::ExtractMin:
                std::cout << "E" << std::endl;
                break;
            default:
                std::cout << "Unknown operation" << std::endl;
        }

    }
    // Identify the instance
    bool isInsert()     const { return tag == OpCode::Insert; }
    bool isFindMin()    const { return tag == OpCode::FindMin; }
    bool isDeleteMin()  const { return tag == OpCode::DeleteMin; }
    bool isExtractMin() const { return tag == OpCode::ExtractMin; }
};
