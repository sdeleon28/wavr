#include <cstdint>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <libkern/OSByteOrder.h>
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
    std::vector<float> samples;
};

struct Chunk {
    RiffChunkDescriptor riff;
    FmtSubChunk fmt;
    DataSubChunk data;
};

int main() {
    std::ifstream file("input.wav", std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size == -1)
    {
        std::cerr << "The file input.wav probably doesn't exist" << std::endl;
        return 2;
    }

    // In a more realistic scenario, I would be chunking this file. Since I was explicitly told to
    // assume the program only needs to run for this file and I was asked to minimize file access,
    // I'm just loading the whole thing into memory.
    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size))
    {
        std::cerr << "Failed to open input.dat for reading" << std::endl;
        return 2;
    }

    std::ostringstream out_stream;

    Chunk chunk;

    int i = 0;

    std::cout << "file size: " << size << std::endl;
    std::cout << "=== riff ===" << std::endl;
    chunk.riff.id[0] = buffer[i + 0];
    chunk.riff.id[1] = buffer[i + 1];
    chunk.riff.id[2] = buffer[i + 2];
    chunk.riff.id[3] = buffer[i + 3];
    chunk.riff.id[4] = '\0';
    chunk.riff.chunk_size = *reinterpret_cast<uint32_t*>(&buffer[4]);
    chunk.riff.format[0] = buffer[i + 8];
    chunk.riff.format[1] = buffer[i + 9];
    chunk.riff.format[2] = buffer[i + 10];
    chunk.riff.format[3] = buffer[i + 11];
    chunk.riff.format[4] = '\0';
    std::cout << "chunk_size: " << chunk.riff.chunk_size << std::endl;
    std::cout << "format: " << chunk.riff.format << std::endl;

    std::cout << "=== fmt ====" << std::endl;
    chunk.fmt.id[0] = buffer[i + 12];
    chunk.fmt.id[1] = buffer[i + 13];
    chunk.fmt.id[2] = buffer[i + 14];
    chunk.fmt.id[3] = buffer[i + 15];
    chunk.fmt.id[4] = '\0';
    chunk.fmt.size = *reinterpret_cast<uint32_t*>(&buffer[i + 16]);
    chunk.fmt.audio_format = *reinterpret_cast<uint16_t*>(&buffer[i + 20]);
    chunk.fmt.num_channels = *reinterpret_cast<uint16_t*>(&buffer[i + 22]);
    chunk.fmt.sample_rate = *reinterpret_cast<uint32_t*>(&buffer[i + 24]);
    chunk.fmt.byte_rate = *reinterpret_cast<uint32_t*>(&buffer[i + 28]);
    chunk.fmt.block_align = *reinterpret_cast<uint16_t*>(&buffer[i + 32]);
    chunk.fmt.bits_per_sample = *reinterpret_cast<uint16_t*>(&buffer[i + 34]);

    std::cout << "id: " << chunk.fmt.id << std::endl;
    std::cout << "size: " << chunk.fmt.size << std::endl;
    std::cout << "audio_format: " << chunk.fmt.audio_format << std::endl;
    std::cout << "num_channels: " << chunk.fmt.num_channels << std::endl;
    std::cout << "sample_rate: " << chunk.fmt.sample_rate << std::endl;
    std::cout << "byte_rate: " << chunk.fmt.byte_rate << std::endl;
    std::cout << "block_align: " << chunk.fmt.block_align << std::endl;
    std::cout << "bits_per_sample: " << chunk.fmt.bits_per_sample << std::endl;
    std::cout << "=== data ===" << std::endl;

    /* std::ofstream out_file("out.txt"); */
    /* if (!out_file.is_open()) { */
    /*     std::cerr << "Failed to open out.txt for writing" << std::endl; */
    /*     return 2; */
    /* } */
    /* out_file << out_stream.str(); */
    /* out_file.close(); */

    return 0;
}
