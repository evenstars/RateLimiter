cmake_minimum_required(VERSION 3.17)
project(ChangRateLimiter)

set(CMAKE_CXX_STANDARD 14)

add_executable(ChangRateLimiter main.cpp src/ChangRateLimiter.h src/impl/ChangRateLimiterImpl.cpp src/impl/ChangRateLimiterImpl.h)