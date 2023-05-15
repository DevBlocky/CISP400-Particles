cmake_minimum_required(VERSION 3.25)
project(particles)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

add_executable(particles src/main.cpp src/Engine.cpp src/Particle.cpp src/brand.cpp src/Matrices.cpp)

target_link_libraries(particles sfml-system sfml-window sfml-graphics)
