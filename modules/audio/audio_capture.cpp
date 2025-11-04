#include "audio_capture.hpp"
#include <portaudio.h>

bool audio_stream_loop(const AudioConfig& cfg,
                       const std::function<bool(const std::vector<int16_t>&)>& on_chunk) {
    if (Pa_Initialize() != paNoError) return false;

    PaStream* stream = nullptr;
    PaStreamParameters inParams{};
    inParams.device = Pa_GetDefaultInputDevice();
    if (inParams.device == paNoDevice) { Pa_Terminate(); return false; }
    inParams.channelCount = 1;
    inParams.sampleFormat = paInt16;
    inParams.suggestedLatency = Pa_GetDeviceInfo(inParams.device)->defaultLowInputLatency;
    inParams.hostApiSpecificStreamInfo = nullptr;

    if (Pa_OpenStream(&stream, &inParams, nullptr, cfg.sample_rate, cfg.frames_per_buffer,
                      paNoFlag, nullptr, nullptr) != paNoError) {
        Pa_Terminate();
        return false;
    }
    Pa_StartStream(stream);

    std::vector<int16_t> buf(cfg.frames_per_buffer);
    bool cont = true;
    while (cont) {
        if (Pa_ReadStream(stream, buf.data(), cfg.frames_per_buffer) != paNoError) break;
        cont = on_chunk(buf);
    }

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();
    return true;
}
