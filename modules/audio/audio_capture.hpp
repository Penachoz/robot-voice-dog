#pragma once
#include <cstdint>
#include <vector>
#include <functional>

struct AudioConfig {
    int sample_rate = 16000;
    int frames_per_buffer = 512;
};

// Llama a on_chunk(samples16) continuamente; retornar false para detener.
bool audio_stream_loop(const AudioConfig& cfg,
                       const std::function<bool(const std::vector<int16_t>&)>& on_chunk);
