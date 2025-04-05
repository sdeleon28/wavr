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

    RiffChunkDescriptor riff;
    riff.id[0] = buffer[0];
    riff.id[1] = buffer[1];
    riff.id[2] = buffer[2];
    riff.id[3] = buffer[3];
    riff.id[4] = '\0';

    /* std::ofstream out_file("out.txt"); */
    /* if (!out_file.is_open()) { */
    /*     std::cerr << "Failed to open out.txt for writing" << std::endl; */
    /*     return 2; */
    /* } */
    /* out_file << out_stream.str(); */
    /* out_file.close(); */

    return 0;
}
