cmake_minimum_required(VERSION 3.10)

project(GarbageCollector)
set(SOURCE_FILES garbage.c)

add_executable(garbage garbage.c garbage.h)
