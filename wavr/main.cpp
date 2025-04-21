#include "wavr.h"

int main() {
    try {
        wavr::WavFile wav = wavr::read_wav("input.wav");
        auto channels = wavr::channels(wav);
        for (float sample : channels[0])
            std::cout << "LEFT:" << sample << std::endl;
        for (float sample : channels[1])
            std::cout << "RIGHT:" << sample << std::endl;
        wavr::write_wav("out.wav", wav);

        // Apply .5f gain using block-processing API
        const uint32_t block_size = 1024;
        wavr::process(wav, [&wav](float* data, uint32_t frames) {
            uint32_t total_samples = frames * wav.fmt.num_channels;
            for (uint32_t i = 0; i < total_samples; ++i) {
                data[i] *= 0.5f;
            }
        }, block_size);

        // Convert processed float samples back to int16 for writing
        for (size_t i = 0; i < wav.data.samples.size(); ++i) {
            float f = wav.data.float_samples[i];
            if (f > 1.0f) f = 1.0f;
            if (f < -1.0f) f = -1.0f;
            wav.data.samples[i] = static_cast<int16_t>(f * 32767.0f);
        }

        wavr::write_wav("lower.wav", wav);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
