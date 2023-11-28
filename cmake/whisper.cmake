include(FetchContent)
# FetchContent_MakeAvailable was not added until CMake 3.14
if(${CMAKE_VERSION} VERSION_LESS 3.14)
    include(add_FetchContent_MakeAvailable.cmake)
endif()

set(WHISPER_GIT_TAG  ffdb5c4735a2410dbc209c180199890966895ce6)
set(WHISPER_GIT_URL  https://github.com/ggerganov/whisper.cpp)
set(BUILD_SHARED_LIBS OFF)

FetchContent_Declare(
  whisper
  GIT_REPOSITORY ${WHISPER_GIT_URL}
  GIT_TAG        ${WHISPER_GIT_TAG}
)

FetchContent_MakeAvailable(whisper)
include_directories(${whisper_SOURCE_DIR})

target_compile_definitions(whisper PUBLIC
        WHISPER_SHARED
        GGML_SHARED
)
target_compile_definitions(whisper PRIVATE
        WHISPER_BUILD
        GGML_BUILD
)

set_target_properties(whisper PROPERTIES POSITION_INDEPENDENT_CODE ON)