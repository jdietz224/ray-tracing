cmake_minimum_required(VERSION 3.10)
project(RayTracing VERSION 0.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDRAD_REQUIRED True)
add_executable(RayTracer main.cc)

#It is not necessary to build these subdiretories into libraries because they are header-only implementations, so #including them does everything
target_include_directories(RayTracer PRIVATE stb)
target_include_directories(RayTracer PRIVATE stl-parse/include)
