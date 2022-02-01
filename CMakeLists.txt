cmake_minimum_required(VERSION 3.19)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# For Apple M1 compile x86 layer for debugging
if (CMAKE_APPLE_SILICON_PROCESSOR)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
endif()

add_subdirectory("ext/fmt")

# set the project name
project(NES VERSION 1.0)

if(MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

# create emulator target
add_executable(emulator)

# Add emulator sources
target_sources(emulator PRIVATE  
    "src/bus/bus.cc"
    "src/cpu/cpu.cc"
    "src/cpu/map.cc"
    "src/cpu/mem.cc"
    "src/cpu/status.cc"
    "src/log.cc"
    "src/main.cc"
)

# add target-specific include directory
target_include_directories(emulator PUBLIC "src")

# add {fmt} library
target_link_libraries(emulator fmt::fmt)