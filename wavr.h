// wavr.h - Header-only library for reading and writing WAV files
#ifndef WAVR_H
#define WAVR_H

#include <cstdint>
#include <cstring>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

#if defined(__linux__)
#include <endian.h>
#elif defined(__APPLE__)
#include <libkern/OSByteOrder.h>
#define be64toh(x) OSSwapBigToHostInt64(x)
#elif defined(_WIN32)
#include <winsock2.h>
#define be64toh(x) _byteswap_uint64(x)
#else
#error Platform not supported
#endif

namespace wavr {

struct RiffChunkDescriptor {
    char id[5];
    uint32_t chunk_size;
    char format[5];
};

struct FmtSubChunk {
    char id[5];
    uint32_t size;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
};

struct DataSubChunk {
    char id[5];
    uint32_t size;
    std::vector<int16_t> samples;
    std::vector<float> float_samples;
};

struct WavFile {
    RiffChunkDescriptor riff;
    FmtSubChunk fmt;
    DataSubChunk data;
};

inline WavFile read_wav(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Failed to open " + filename);
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    if (size == -1) {
        throw std::runtime_error("Failed to determine size of " + filename);
    }

    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size)) {
        throw std::runtime_error("Failed to read data from " + filename);
    }

    WavFile wav;
    // Parse RIFF header
    wav.riff.id[0] = buffer[0];
    wav.riff.id[1] = buffer[1];
    wav.riff.id[2] = buffer[2];
    wav.riff.id[3] = buffer[3];
    wav.riff.id[4] = '\0';
    wav.riff.chunk_size = *reinterpret_cast<uint32_t*>(&buffer[4]);
    wav.riff.format[0] = buffer[8];
    wav.riff.format[1] = buffer[9];
    wav.riff.format[2] = buffer[10];
    wav.riff.format[3] = buffer[11];
    wav.riff.format[4] = '\0';

    // Parse fmt subchunk
    wav.fmt.id[0] = buffer[12];
    wav.fmt.id[1] = buffer[13];
    wav.fmt.id[2] = buffer[14];
    wav.fmt.id[3] = buffer[15];
    wav.fmt.id[4] = '\0';
    wav.fmt.size = *reinterpret_cast<uint32_t*>(&buffer[16]);
    wav.fmt.audio_format = *reinterpret_cast<uint16_t*>(&buffer[20]);
    wav.fmt.num_channels = *reinterpret_cast<uint16_t*>(&buffer[22]);
    wav.fmt.sample_rate = *reinterpret_cast<uint32_t*>(&buffer[24]);
    wav.fmt.byte_rate = *reinterpret_cast<uint32_t*>(&buffer[28]);
    wav.fmt.block_align = *reinterpret_cast<uint16_t*>(&buffer[32]);
    wav.fmt.bits_per_sample = *reinterpret_cast<uint16_t*>(&buffer[34]);

    // Find and parse data subchunk
    uint32_t offset = 12 + 8 + wav.fmt.size;
    while (offset + 8 <= static_cast<uint32_t>(buffer.size())) {
        char id[5] = { buffer[offset], buffer[offset + 1], buffer[offset + 2], buffer[offset + 3], '\0' };
        uint32_t subchunk_size = *reinterpret_cast<uint32_t*>(&buffer[offset + 4]);
        if (std::string(id) == "data") {
            DataSubChunk data;
            data.id[0] = id[0]; data.id[1] = id[1]; data.id[2] = id[2]; data.id[3] = id[3]; data.id[4] = '\0';
            data.size = subchunk_size;
            data.samples.reserve(subchunk_size / sizeof(int16_t));
            data.float_samples.reserve(subchunk_size / sizeof(int16_t));
            for (uint32_t i = 0; i < subchunk_size; i += sizeof(int16_t)) {
                int16_t sample;
                std::memcpy(&sample, &buffer[offset + 8 + i], sizeof(int16_t));
                data.samples.push_back(sample);
                data.float_samples.push_back(static_cast<float>(sample) / 32768.0f);
            }
            wav.data = std::move(data);
            break;
        }
        offset += 8 + subchunk_size;
    }

    return wav;
}

inline void write_wav(const std::string& filename, const WavFile& wav) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        throw std::runtime_error("Failed to open " + filename + " for writing");
    }
    // Write RIFF header
    outFile.write(wav.riff.id, 4);
    outFile.write(reinterpret_cast<const char*>(&wav.riff.chunk_size), sizeof(wav.riff.chunk_size));
    outFile.write(wav.riff.format, 4);
    // Write fmt subchunk
    outFile.write(wav.fmt.id, 4);
    outFile.write(reinterpret_cast<const char*>(&wav.fmt.size), sizeof(wav.fmt.size));
    outFile.write(reinterpret_cast<const char*>(&wav.fmt.audio_format), sizeof(wav.fmt.audio_format));
    outFile.write(reinterpret_cast<const char*>(&wav.fmt.num_channels), sizeof(wav.fmt.num_channels));
    outFile.write(reinterpret_cast<const char*>(&wav.fmt.sample_rate), sizeof(wav.fmt.sample_rate));
    outFile.write(reinterpret_cast<const char*>(&wav.fmt.byte_rate), sizeof(wav.fmt.byte_rate));
    outFile.write(reinterpret_cast<const char*>(&wav.fmt.block_align), sizeof(wav.fmt.block_align));
    outFile.write(reinterpret_cast<const char*>(&wav.fmt.bits_per_sample), sizeof(wav.fmt.bits_per_sample));
    // Write data subchunk
    outFile.write(wav.data.id, 4);
    outFile.write(reinterpret_cast<const char*>(&wav.data.size), sizeof(wav.data.size));
    outFile.write(reinterpret_cast<const char*>(wav.data.samples.data()),
                  wav.data.samples.size() * sizeof(int16_t));
}

} // namespace wavr

#endif // WAVR_H
