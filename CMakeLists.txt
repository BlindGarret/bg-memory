cmake_minimum_required (VERSION 3.10 FATAL_ERROR)
project (BGMemoryPools VERSION 0.1 LANGUAGES CXX)

include_directories(include)

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(googletest)

add_subdirectory(bgmemorypools)