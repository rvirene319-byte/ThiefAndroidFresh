cmake_minimum_required(VERSION 3.22.1)
project(thief_android LANGUAGES CXX)

file(GLOB_RECURSE NATIVE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*native_entry.cpp")
if(NOT NATIVE_SRC)
  message(FATAL_ERROR "native_entry.cpp not found under ${CMAKE_CURRENT_SOURCE_DIR}")
endif()

add_library(thief_android SHARED ${NATIVE_SRC})

find_library(log-lib log)
find_library(android-lib android)

target_link_libraries(thief_android ${log-lib} ${android-lib} GLESv2)
