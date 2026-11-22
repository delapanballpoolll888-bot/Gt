cmake_minimum_required(VERSION 3.18)
project(gt_cheat CXX)
set(CMAKE_CXX_STANDARD 20)

add_library(gt_cheat SHARED
    src/main.cpp
    src/hooks.cpp
)
target_link_libraries(gt_cheat log android)
# Kamu harus download libdobby.so x86_64/arm64 dan taruh di folder jniLibs
