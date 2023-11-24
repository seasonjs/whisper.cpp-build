#ifndef WHISPER_CPP_BUILD_WHISPER_ABI_H
#define WHISPER_CPP_BUILD_WHISPER_ABI_H

#ifdef WHISPER_SHARED
#    ifdef _WIN32
#        ifdef WHISPER_BUILD
#            define WHISPER_API __declspec(dllexport)
#        else
#            define WHISPER_API __declspec(dllimport)
#        endif
#    else
#        define WHISPER_API __attribute__ ((visibility ("default")))
#    endif
#else
#    define WHISPER_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif //WHISPER_CPP_BUILD_WHISPER_ABI_H
