#include "wavr.h"

// example code of how one might use ChannelIterator
// print out float contents to stdout
// give this as input to plot.py to visualize if you got the right data in
// ChannelIterator left = channel_iterators[0];
// for (float sample : left)
//     std::cout << "LEFT:" << sample << std::endl;
// ChannelIterator right = channel_iterators[1];
// for (float sample : right)
//     std::cout << "RIGHT:" << sample << std::endl;

int main() {
    try {
        wavr::WavFile wav = wavr::read_wav("input.wav");
        wavr::write_wav("out.wav", wav);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
