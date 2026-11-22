cmake_minimum_required(VERSION 3.18)
project(gt_cheat CXX)
set(CMAKE_CXX_STANDARD 20)

include_directories(src)

add_library(gt_cheat SHARED
    src/main.cpp
    src/hooks.cpp
)

# Load library Dobby (Static .a)
add_library(dobby STATIC IMPORTED)
set_target_properties(dobby PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/src/libdobby.a)

target_link_libraries(gt_cheat dobby log android)
