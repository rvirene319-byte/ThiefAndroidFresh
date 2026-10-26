cmake_minimum_required(VERSION 3.22.1)
project(thief_android LANGUAGES CXX)

add_library(thief_android SHARED native_entry.cpp)

find_library(log-lib log)
find_library(android-lib android)

target_link_libraries(thief_android ${log-lib} ${android-lib} GLESv2)
