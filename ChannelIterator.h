#pragma once
#include <vector>
#include <cstdint>

class ChannelIterator {
public:
    ChannelIterator(std::vector<float>::iterator it, std::vector<float>::iterator endIt, int step)
        : it(it), endIt(endIt), step(step) {}

    ChannelIterator begin() {
        return *this;
    }

    ChannelIterator end() const {
        return ChannelIterator(endIt, endIt, step);
    }

    ChannelIterator& operator++() {
        std::advance(it, step);
        return *this;
    }

    float& operator*() {
        return *it;
    }

    bool operator!=(const ChannelIterator& other) const {
        return it != other.it;
    }

private:
    std::vector<float>::iterator it;
    std::vector<float>::iterator endIt;
    int step;
};
