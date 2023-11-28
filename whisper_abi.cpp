#include <string>
#include <map>
#include <vector>
#include <iterator>
#include <algorithm>

#define DR_WAV_IMPLEMENTATION

#include "dr_wav.h"
#include "whisper.h"
#include "whisper_abi.h"

const static std::map<std::string, enum whisper_sampling_strategy> whisper_sampling_strategy_map = {
        {"WHISPER_SAMPLING_GREEDY",      WHISPER_SAMPLING_GREEDY},
        {"WHISPER_SAMPLING_BEAM_SEARCH", WHISPER_SAMPLING_BEAM_SEARCH},
};

bool parse_wav(std::vector<uint8_t> wav_data, std::vector<float> &pcmf32, std::vector<std::vector<float>> &pcmf32s,
               bool stereo) {
    drwav wav;

    if (drwav_init_memory(&wav, wav_data.data(), wav_data.size(), nullptr) == false) {
        return false;
    }


    if (wav.channels != 1 && wav.channels != 2) {
        return false;
    }

    if (stereo && wav.channels != 2) {
        return false;
    }

    if (wav.sampleRate != WHISPER_SAMPLE_RATE) {
        return false;
    }

    if (wav.bitsPerSample != 16) {
        return false;
    }

    const uint64_t n = wav_data.empty() ? wav.totalPCMFrameCount : wav_data.size() /
                                                                   (wav.channels * wav.bitsPerSample / 8);

    std::vector<int16_t> pcm16;
    pcm16.resize(n * wav.channels);
    drwav_read_pcm_frames_s16(&wav, n, pcm16.data());
    drwav_uninit(&wav);

    // convert to mono, float
    pcmf32.resize(n);
    if (wav.channels == 1) {
        for (uint64_t i = 0; i < n; i++) {
            pcmf32[i] = float(pcm16[i]) / 32768.0f;
        }
    } else {
        for (uint64_t i = 0; i < n; i++) {
            pcmf32[i] = float(pcm16[2 * i] + pcm16[2 * i + 1]) / 65536.0f;
        }
    }

    if (stereo) {
        // convert to stereo, float
        pcmf32s.resize(2);

        pcmf32s[0].resize(n);
        pcmf32s[1].resize(n);
        for (uint64_t i = 0; i < n; i++) {
            pcmf32s[0][i] = float(pcm16[2 * i]) / 32768.0f;
            pcmf32s[1][i] = float(pcm16[2 * i + 1]) / 32768.0f;
        }
    }

    return true;
}

void whisper_context_params_set_use_gpu(struct whisper_context_params *params, bool use_gpu) {
    if (params != nullptr) {
        params->use_gpu = use_gpu;
    }
}

void whisper_full_params_set_strategy(struct whisper_full_params *params, const char *strategy) {
    if (params != nullptr) {
        const auto str = std::string(strategy);
        const auto it = whisper_sampling_strategy_map.find(str);
        if (it != whisper_sampling_strategy_map.end()) {
            params->strategy = it->second;
        }

    }

}

void whisper_full_params_set_n_threads(struct whisper_full_params *params, int n_threads) {
    if (params != nullptr) {
        params->n_threads = n_threads;
    }
}

void whisper_full_params_set_n_max_text_ctx(struct whisper_full_params *params, int n_max_text_ctx) {
    if (params != nullptr) {
        params->n_max_text_ctx = n_max_text_ctx;
    }
}

void whisper_full_params_set_offset_ms(struct whisper_full_params *params, int offset_ms) {
    if (params != nullptr) {
        params->offset_ms = offset_ms;
    }
}

void whisper_full_params_set_duration_ms(struct whisper_full_params *params, int duration_ms) {
    if (params != nullptr) {
        params->duration_ms = duration_ms;
    }
}

void whisper_full_params_set_translate(struct whisper_full_params *params, bool translate) {
    if (params != nullptr) {
        params->translate = translate;
    }
}

void whisper_full_params_set_no_context(struct whisper_full_params *params, bool no_context) {
    if (params != nullptr) {
        params->no_context = no_context;
    }
}

void whisper_full_params_set_no_timestamps(struct whisper_full_params *params, bool no_timestamps) {
    if (params != nullptr) {
        params->no_timestamps = no_timestamps;
    }
}

void whisper_full_params_set_single_segment(struct whisper_full_params *params, bool single_segment) {
    if (params != nullptr) {
        params->single_segment = single_segment;
    }
}

void whisper_full_params_set_print_special(struct whisper_full_params *params, bool print_special) {
    if (params != nullptr) {
        params->print_special = print_special;
    }
}

void whisper_full_params_set_print_progress(struct whisper_full_params *params, bool print_progress) {
    if (params != nullptr) {
        params->print_progress = print_progress;
    }
}

void whisper_full_params_set_print_realtime(struct whisper_full_params *params, bool print_realtime) {
    if (params != nullptr) {
        params->print_realtime = print_realtime;
    }
}

void whisper_full_params_set_print_timestamps(struct whisper_full_params *params, bool print_timestamps) {
    if (params != nullptr) {
        params->print_timestamps = print_timestamps;
    }
}

void whisper_full_params_set_token_timestamps(struct whisper_full_params *params, bool token_timestamps) {
    if (params != nullptr) {
        params->token_timestamps = token_timestamps;
    }
}

void whisper_full_params_set_thold_pt(struct whisper_full_params *params, float thold_pt) {
    if (params != nullptr) {
        params->thold_pt = thold_pt;
    }
}

void whisper_full_params_set_thold_ptsum(struct whisper_full_params *params, float thold_ptsum) {
    if (params != nullptr) {
        params->thold_ptsum = thold_ptsum;
    }
}

void whisper_full_params_set_max_len(struct whisper_full_params *params, int max_len) {
    if (params != nullptr) {
        params->max_len = max_len;
    }
}

void whisper_full_params_set_split_on_word(struct whisper_full_params *params, bool split_on_word) {
    if (params != nullptr) {
        params->split_on_word = split_on_word;
    }
}

void whisper_full_params_set_max_tokens(struct whisper_full_params *params, int max_tokens) {
    if (params != nullptr) {
        params->max_tokens = max_tokens;
    }
}

void whisper_full_params_set_speed_up(struct whisper_full_params *params, bool speed_up) {
    if (params != nullptr) {
        params->speed_up = speed_up;
    }
}

void whisper_full_params_set_debug_mode(struct whisper_full_params *params, bool debug_mode) {
    if (params != nullptr) {
        params->debug_mode = debug_mode;
    }
}

void whisper_full_params_set_audio_ctx(struct whisper_full_params *params, int audio_ctx) {
    if (params != nullptr) {
        params->audio_ctx = audio_ctx;
    }
}

void whisper_full_params_set_tdrz_enable(struct whisper_full_params *params, bool tdrz_enable) {
    if (params != nullptr) {
        params->tdrz_enable = tdrz_enable;
    }
}

void whisper_full_params_set_initial_prompt(struct whisper_full_params *params, const char *initial_prompt) {
    if (params != nullptr) {
        params->initial_prompt = initial_prompt;
    }
}

void whisper_full_params_set_prompt_tokens(struct whisper_full_params *params, const int32_t *prompt_tokens) {
    if (params != nullptr) {
        params->prompt_tokens = prompt_tokens;
    }
}

void whisper_full_params_set_prompt_n_tokens(struct whisper_full_params *params, int prompt_n_tokens) {
    if (params != nullptr) {
        params->prompt_n_tokens = prompt_n_tokens;
    }
}

void whisper_full_params_set_language(struct whisper_full_params *params, const char *language) {
    if (params != nullptr) {
        params->language = language;
    }
}

void whisper_full_params_set_detect_language(struct whisper_full_params *params, bool detect_language) {
    if (params != nullptr) {
        params->detect_language = detect_language;
    }
}

void whisper_full_params_set_suppress_blank(struct whisper_full_params *params, bool suppress_blank) {
    if (params != nullptr) {
        params->suppress_blank = suppress_blank;
    }
}

void whisper_full_params_set_suppress_non_speech_tokens(struct whisper_full_params *params,
                                                        bool suppress_non_speech_tokens) {
    if (params != nullptr) {
        params->suppress_non_speech_tokens = suppress_non_speech_tokens;
    }
}

void whisper_full_params_set_temperature(struct whisper_full_params *params, float temperature) {
    if (params != nullptr) {
        params->temperature = temperature;
    }
}

void whisper_full_params_set_max_initial_ts(struct whisper_full_params *params, float max_initial_ts) {
    if (params != nullptr) {
        params->max_initial_ts = max_initial_ts;
    }
}

void whisper_full_params_set_length_penalty(struct whisper_full_params *params, float length_penalty) {
    if (params != nullptr) {
        params->length_penalty = length_penalty;
    }
}

void whisper_full_params_set_temperature_inc(struct whisper_full_params *params, float temperature_inc) {
    if (params != nullptr) {
        params->temperature_inc = temperature_inc;
    }
}

void whisper_full_params_set_entropy_thold(struct whisper_full_params *params, float entropy_thold) {
    if (params != nullptr) {
        params->entropy_thold = entropy_thold;
    }
}

void whisper_full_params_set_logprob_thold(struct whisper_full_params *params, float logprob_thold) {
    if (params != nullptr) {
        params->logprob_thold = logprob_thold;
    }
}

void whisper_full_params_set_no_speech_thold(struct whisper_full_params *params, float no_speech_thold) {
    if (params != nullptr) {
        params->no_speech_thold = no_speech_thold;
    }
}

void whisper_full_params_set_greedy_best_of(struct whisper_full_params *params, int best_of) {
    if (params != nullptr) {
        params->greedy.best_of = best_of;
    }
}

void whisper_full_params_set_beam_search_beam_size(struct whisper_full_params *params, int beam_size) {
    if (params != nullptr) {
        params->beam_search.beam_size = beam_size;
    }
}

void whisper_full_params_set_beam_search_patience(struct whisper_full_params *params, float patience) {
    if (params != nullptr) {
        params->beam_search.patience = patience;
    }
}

struct whisper_context *
whisper_init_from_file_with_params_ref(const char *path_model, struct whisper_context_params *params) {
    return whisper_init_from_file_with_params(path_model, *params);
}

struct whisper_context *
whisper_init_from_buffer_with_params_ref(void *buffer, size_t buffer_size, struct whisper_context_params *params) {
    return whisper_init_from_buffer_with_params(buffer, buffer_size, *params);
}

struct whisper_context *
whisper_init_from_file_with_params_ref_no_state(const char *path_model, struct whisper_context_params *params) {
    return whisper_init_from_file_with_params_no_state(path_model, *params);
}

struct whisper_context *whisper_init_from_buffer_with_params_ref_no_state(void *buffer, size_t buffer_size,
                                                                          struct whisper_context_params *params) {
    return whisper_init_from_buffer_with_params_no_state(buffer, buffer_size, *params);
}

int whisper_full_ref(
        struct whisper_context *ctx,
        struct whisper_full_params *params,
        const uint8_t *samples,
        int n_samples
) {
    std::vector<uint8_t> wav;
    wav.reserve(n_samples);
    std::copy(samples, samples + n_samples, std::back_inserter(wav));
    std::vector<float> pcmf32;               // mono-channel F32 PCM
    std::vector<std::vector<float>> pcmf32s; // stereo-channel F32 PCM
    if (!parse_wav(wav, pcmf32, pcmf32s, false)) {
        return -1000;
    }
    return whisper_full(ctx, *params, pcmf32.data(), static_cast<int >(pcmf32.size()));
}

int whisper_full_ref_with_state(
        struct whisper_context *ctx,
        struct whisper_state *state,
        struct whisper_full_params *params,
        const uint8_t *samples,
        int n_samples
) {
    std::vector<uint8_t> wav;
    wav.reserve(n_samples);
    std::copy(samples, samples + n_samples, std::back_inserter(wav));
    std::vector<float> pcmf32;               // mono-channel F32 PCM
    std::vector<std::vector<float>> pcmf32s; // stereo-channel F32 PCM
    if (!parse_wav(wav, pcmf32, pcmf32s, false)) {
        return -1000;
    }
    return whisper_full_with_state(ctx, state, *params, pcmf32.data(), static_cast<int >(pcmf32.size()));
}

int whisper_full_ref_parallel(
        struct whisper_context *ctx,
        struct whisper_full_params *params,
        const uint8_t *samples,
        int n_samples,
        int n_processors
) {
    std::vector<uint8_t> wav;
    wav.reserve(n_samples);
    std::copy(samples, samples + n_samples, std::back_inserter(wav));
    std::vector<float> pcmf32;               // mono-channel F32 PCM
    std::vector<std::vector<float>> pcmf32s; // stereo-channel F32 PCM
    if (!parse_wav(wav, pcmf32, pcmf32s, false)) {
        return -1000;
    }
    auto result = whisper_full_parallel(ctx, *params, pcmf32.data(), static_cast<int >(pcmf32.size()), n_processors);
    return result;
}

