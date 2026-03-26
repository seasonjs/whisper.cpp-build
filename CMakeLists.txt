cmake_minimum_required(VERSION 3.12)

project("whisper.cpp-build")

include(FetchContent)
include(./cmake/fetch_prebuilt_ggml.cmake)

set(
    WHISPER_SOURCE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/whisper-src"
    CACHE PATH
    "Optional local whisper source tree. When present it is preferred over downloading whisper."
)

set(GGML_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/ggml" CACHE PATH "Path to an unpacked ggml SDK")
set(GGML_RELEASE_BASE_URL "https://github.com/seasonjs/ggml.cpp-build/releases/download" CACHE STRING "Base release URL, for example https://github.com/seasonjs/ggml.cpp-build/releases/download")
set(GGML_RELEASE_TAG "v0.9.8" CACHE STRING "Release tag containing the prebuilt ggml SDK archives")

# add dependencies
get_filename_component(_whisper_local_source_dir "${WHISPER_SOURCE_DIR}" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Ensure non-empty build number for whisper.cpp versioning

set(WHISPER_UPSTREAM_TAG "v1.8.4" CACHE STRING "Default whisper build number" FORCE)


# Strategy: ggml dynamic + whisper static
# Note: We'll use BUILD_SHARED_LIBS=ON to build both as shared initially,
# but whisper.cpp's CMake allows us to use ggml as dynamic

# Important: Build shared libraries initially (ggml + whisper)
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libs" FORCE)
set(WHISPER_USE_SYSTEM_GGML ON CACHE BOOL "Use system ggml" FORCE)

message("check WHISPER_UPSTREAM_TAG ${WHISPER_UPSTREAM_TAG}")

add_compile_definitions(GGML_MAX_NAME=128)

if (EXISTS "${GGML_ROOT_DIR}")
    ggml_import_prebuilt(
        ROOT_DIR "${GGML_ROOT_DIR}"
        OUT_ROOT_DIR GGML_PACKAGE_ROOT
    )
elseif (GGML_RELEASE_BASE_URL)
    ggml_import_prebuilt(
        RELEASE_BASE_URL "${GGML_RELEASE_BASE_URL}"
        RELEASE_TAG "${GGML_RELEASE_TAG}"
        OUT_ROOT_DIR GGML_PACKAGE_ROOT
    )
else()
    message(FATAL_ERROR
        "Set GGML_ROOT_DIR to an unpacked ggml SDK or provide GGML_RELEASE_BASE_URL so CMake can download one.")
endif()

if (EXISTS "${_whisper_local_source_dir}/CMakeLists.txt")
    message(STATUS "Using local whisper source tree: ${_whisper_local_source_dir}")
    add_subdirectory("${_whisper_local_source_dir}" "${CMAKE_BINARY_DIR}/whisper")
else()
    message(STATUS "Fetching whisper.cpp ${WHISPER_UPSTREAM_TAG} from GitHub")
    FetchContent_Declare(
        whisper
        GIT_REPOSITORY https://github.com/ggml-org/whisper.cpp.git
        GIT_TAG        ${WHISPER_UPSTREAM_TAG}
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(whisper)
endif()

# We don't need to copy ggml's runtime binaries to the output directory
# ggml_copy_runtime_binaries(main_app
#     ROOT_DIR "${GGML_PACKAGE_ROOT}"
# )
