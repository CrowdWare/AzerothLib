cmake_minimum_required(VERSION 3.4)
project(test)

# Add the executable for your test program
add_executable(test test.cpp)

# Link against the shared library targets
#target_link_libraries(test PRIVATE shared fmt g3dlite)

# Include the directories for the shared library
target_include_directories(test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../shared)

target_link_libraries(test PRIVATE shared fmt ${CMAKE_CURRENT_SOURCE_DIR}/../build/deps/g3dlite/Debug/g3dlib.lib)


