#include "wavr.h"

int main() {
    try {
        wavr::WavFile wav = wavr::read_wav("input.wav");
        auto channels = wavr::channel_iterators(wav);
        for (float sample : channels[0])
            std::cout << "LEFT:" << sample << std::endl;
        for (float sample : channels[1])
            std::cout << "RIGHT:" << sample << std::endl;
        wavr::write_wav("out.wav", wav);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
