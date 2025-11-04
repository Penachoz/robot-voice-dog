#pragma once
#include <string>
#include <vector>
#include <cstdint>

class WhisperSTT {
public:
    explicit WhisperSTT(const std::string& model_path);
    ~WhisperSTT();

    // PCM int16 -> texto (es)
    std::string transcribe(const std::vector<int16_t>& pcm16);

private:
    struct whisper_context* ctx_ = nullptr;
};
