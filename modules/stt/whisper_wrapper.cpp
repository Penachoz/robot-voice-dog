#include "whisper_wrapper.hpp"
#include "whisper.h"
#include <stdexcept>

WhisperSTT::WhisperSTT(const std::string& model_path) {
    ctx_ = whisper_init_from_file(model_path.c_str());
    if (!ctx_) throw std::runtime_error("whisper_init failed");
}

WhisperSTT::~WhisperSTT() {
    if (ctx_) whisper_free(ctx_);
}

static inline void pcm16_to_float(const std::vector<int16_t>& in, std::vector<float>& out) {
    out.resize(in.size());
    for (size_t i = 0; i < in.size(); ++i) out[i] = in[i] / 32768.0f;
}

std::string WhisperSTT::transcribe(const std::vector<int16_t>& pcm16) {
    std::vector<float> mono;
    pcm16_to_float(pcm16, mono);

    auto params = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);
    params.language = "es";
    params.translate = false;

    if (whisper_full(ctx_, params, mono.data(), (int)mono.size()) != 0)
        return {};

    std::string out;
    const int n = whisper_full_n_segments(ctx_);
    for (int i = 0; i < n; ++i) out += whisper_full_get_segment_text(ctx_, i);
    return out;
}
