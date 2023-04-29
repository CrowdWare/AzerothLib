cmake_minimum_required(VERSION 3.4)
project(test)

# Add the executable for your test program
add_executable(test test.cpp)

# Link against the shared library targets
#target_link_libraries(test PRIVATE shared fmt g3dlite)

# Include the directories for the shared library
target_include_directories(test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../shared)

# Set the path to the g3dlite library
set(G3DLITE_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../build/deps/g3dlite/Debug/g3dlib.lib")

# Check if the library exists at the specified path
if(NOT EXISTS "${G3DLITE_LIB_PATH}")
  # If the library doesn't exist, then set its path based on the build directory
  set(G3DLITE_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../build/AzerothLib/deps/g3dlite/Debug/g3dlib.lib")
endif()

# Link the g3dlite library to the test project
target_link_libraries(test PRIVATE shared fmt "${G3DLITE_LIB_PATH}")
