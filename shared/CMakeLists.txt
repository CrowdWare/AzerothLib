cmake_minimum_required(VERSION 3.4)
project(shared)

set(CMAKE_CXX_STANDARD 17)

add_library(shared STATIC log.cpp pch.cpp)
set_target_properties(shared PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(shared PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../build/deps/g3dlite/Debug/g3dlib.lib)

target_include_directories(shared PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../deps/g3dlite/include)