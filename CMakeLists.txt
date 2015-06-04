cmake_minimum_required(VERSION 3.2)
project(dukv8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

include_directories(include deps/duktape deps/ustl deps/tracey)

file(GLOB SOURCE_FILES "src/*.cpp")

add_executable(dukv8
    ${SOURCE_FILES}
    deps/duktape/duktape.c
    samples/main.cpp)