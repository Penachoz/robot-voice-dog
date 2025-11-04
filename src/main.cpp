#include <iostream>
#include <deque>
#include <cmath>
#include <vector>
#include "modules/audio/audio_capture.hpp"
#include "modules/stt/whisper_wrapper.hpp"
#include "modules/command/command_router.hpp"

static bool is_voice(const std::vector<int16_t>& x, float thr) {
    double e = 0.0; for (auto v : x){ double f = v; e += f*f; }
    e = std::sqrt(e / x.size());
    return e > thr;
}

int main(int argc, char** argv) {
    const std::string model = (argc > 1) ? argv[1] : "./models/ggml-base.bin";
    WhisperSTT stt(model);
    AudioConfig cfg; cfg.sample_rate = 16000; cfg.frames_per_buffer = 512;

    std::deque<int16_t> phrase;
    int silence_chunks = 0;
    const int max_phrase_s = 8;
    const int max_chunks = (16000/512)*max_phrase_s;
    const int end_silence_chunks = 12;
    const float vad_thr = 800.0f;

    std::cout << "[voice] escuchando..." << std::endl;

    audio_stream_loop(cfg, [&](const std::vector<int16_t>& chunk){
        bool voice = is_voice(chunk, vad_thr);
        if (voice) {
            silence_chunks = 0;
            for (auto s : chunk) phrase.push_back(s);
            if ((int)phrase.size() > max_chunks) phrase.clear();
        } else {
            if (!phrase.empty()) ++silence_chunks;
            if (silence_chunks >= end_silence_chunks && !phrase.empty()) {
                std::vector<int16_t> pcm(phrase.begin(), phrase.end());
                phrase.clear(); silence_chunks = 0;

                auto text = stt.transcribe(pcm);
                if (!text.empty()) {
                    std::cout << "[STT] " << text << std::endl;
                    auto cmd = parse_command_es(text);
                    if (cmd) {
                        std::cout << "[CMD] ";
                        switch (*cmd){
                            case Command::FORWARD: std::cout << "FORWARD"; break;
                            case Command::STOP:    std::cout << "STOP";    break;
                            case Command::LEFT:    std::cout << "LEFT";    break;
                            case Command::RIGHT:   std::cout << "RIGHT";   break;
                            case Command::SIT:     std::cout << "SIT";     break;
                            case Command::STAND:   std::cout << "STAND";   break;
                        }
                        std::cout << std::endl;
                    }
                }
            }
        }
        return true;
    });
    return 0;
}
