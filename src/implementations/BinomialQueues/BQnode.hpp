//
// Created by Ali A. Kooshesh on 5/5/21.
//

#ifndef BINOMIALQUEUES_BQNODE_HPP
#define BINOMIALQUEUES_BQNODE_HPP

#include<vector>
#include<tuple>

/*
 * A Binomial Queue node hold a value (an int in our implementation) and
 * uses a vector to keep track of its children. It is a passive class in
 * that it doesn't impose any value or structural property on its children.
 *
 * For the benefit of benchmarking, we keep track of instances std::pair
 */

class BQnode {
public:
    BQnode(const std::pair<int, int> &v);

    [[nodiscard]] const std::pair<int, int> &value();
    [[nodiscard]] const std::pair<int, int> &value() const;

    void print();

    void addChild(BQnode *child);
    [[nodiscard]] const std::vector<BQnode *> &children();
    [[nodiscard]] const std::vector<BQnode *> &children() const;

private:
    std::pair<int, int> nodeValue;
    std::vector<BQnode *> _children;
};


#endif //BINOMIALQUEUES_BQNODE_HPP
