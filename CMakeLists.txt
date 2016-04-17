cmake_minimum_required(VERSION 2.8)
project(LongestPrefixMatchingwithPruning)

set(CMAKE_CXX_FLAGS "-std=c++0x -O2  -pedantic -Wredundant-decls")

set(SOURCE_FILES ./hash_function.cpp ./md5.cpp ./sha1.cpp ./Trie.cpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_executable(LPMwithPruning ${SOURCE_FILES} main.cpp)
