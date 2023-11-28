include(FetchContent)
# FetchContent_MakeAvailable was not added until CMake 3.14
if(${CMAKE_VERSION} VERSION_LESS 3.14)
    include(add_FetchContent_MakeAvailable.cmake)
endif()

set(DR_LIBS_GIT_TAG  9eed1be421749ba68a87e5b4c3b10858f8580689)
set(DR_LIBS_GIT_URL  https://github.com/mackron/dr_libs)
set(BUILD_SHARED_LIBS OFF)

FetchContent_Declare(
        dr_libs
        GIT_REPOSITORY ${DR_LIBS_GIT_URL}
        GIT_TAG        ${DR_LIBS_GIT_TAG}
)

FetchContent_MakeAvailable(dr_libs)