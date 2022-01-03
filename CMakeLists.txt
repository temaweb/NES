cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project name
project(NES VERSION 1.0)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# create emulator target
add_executable(emulator)

# Add emulator sources
target_sources(emulator PRIVATE 
    "src/main.cc" 
    "src/map.cc"
    "src/bus.cc"
    "src/cpu.cc"
)

target_link_libraries(emulator ${SDL2_LIBRARIES})