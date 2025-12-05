//
// Created by Ali Kooshesh on 11/5/25.
//

#ifndef PRIORITY_QUEUE_STUDY_LINEARBASELINE_HPP
#define PRIORITY_QUEUE_STUDY_LINEARBASELINE_HPP

#include<vector>

class LinearBaseLine {
public:

    LinearBaseLine(bool (*)(const std::pair<int, int> &, const std::pair<int, int> &));
    explicit LinearBaseLine(const std::vector<std::pair<int, int>>& data,
        bool (*)(const std::pair<int, int> &, const std::pair<int, int> &));  // bulk build

    void insert(const std::pair<int, int>& v);
    void deleteMin();
    const std::pair<int, int>& findMin() const;
    std::pair<int, int> extractMin();

    // inspectors / modifiers
    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] std::size_t size() const noexcept;
    void clear() noexcept;
    void reserve(std::size_t n); // not implemented


private:
    std::vector<std::pair<int, int>> _heap;
    bool (* priorityFunc)(const std::pair<int, int> &a, const std::pair<int, int> &b);

};

#endif //PRIORITY_QUEUE_STUDY_LINEARBASELINE_HPP