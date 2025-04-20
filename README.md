# collection of utilities for dealing with audio files

## wavr

```cpp
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
```
