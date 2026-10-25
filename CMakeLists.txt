cmake_minimum_required(VERSION 3.22)
project(ThiefAndroidFresh LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(ANDROID)
    add_library(thief_android SHARED
        app/src/main/cpp/native_entry.cpp
    )

    target_include_directories(thief_android PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    find_library(android-lib android REQUIRED)
    find_library(log-lib log REQUIRED)

    target_link_libraries(thief_android PRIVATE
        ${android-lib}
        ${log-lib}
    )
endif()
