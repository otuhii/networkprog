cmake_minimum_required(VERSION 3.10)

project(GarbageCollector)
set(SOURCE_FILES garbage.c)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(garbage ${SOURCE_FILES})
