cmake_minimum_required(VERSION 3.14)
project("whisper-build")


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT XCODE AND NOT MSVC AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED true)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

include(whisper)
include_directories(${whisper_SOURCE_DIR})

include(dr_libs)
include_directories(${dr_libs_SOURCE_DIR})

set(WHISPER_ABI whisper-abi)
add_library(${WHISPER_ABI} SHARED whisper_abi.cpp whisper_abi.h ${dr_libs_SOURCE_DIR}/dr_wav.h)

target_include_directories(${WHISPER_ABI} PUBLIC .)

set_target_properties(${WHISPER_ABI} PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_compile_definitions(${WHISPER_ABI} PUBLIC WHISPER_SHARED WHISPER_BUILD)
target_link_libraries(${WHISPER_ABI} PRIVATE whisper ${CMAKE_THREAD_LIBS_INIT})

