cmake_minimum_required(VERSION 3.4)
project(shared)

set(CMAKE_CXX_STANDARD 17)

add_library(shared STATIC log.cpp pch.cpp)
set_target_properties(shared PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(shared PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../deps/g3dlite/include)


# Set the path to the g3dlite library
set(G3DLITE_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../build/deps/g3dlite/Debug/g3dlib.lib")

# Check if the library exists at the specified path
if(NOT EXISTS "${G3DLITE_LIB_PATH}")
  # If the library doesn't exist, then set its path based on the build directory
  set(G3DLITE_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../build/AzerothLib/deps/g3dlite/Debug/g3dlib.lib")
endif()

# Link the g3dlite library to the test project
target_link_libraries(shared PRIVATE "${G3DLITE_LIB_PATH}")