cmake_minimum_required(VERSION 3.4)
project(azerothlib)

set(CMAKE_CXX_STANDARD 17)

# Add the subdirectory for the shared library
add_subdirectory(shared)
add_subdirectory(deps/fmt)
add_subdirectory(deps/g3dlite)
add_subdirectory(deps/zlib)
add_subdirectory(tests)

set(SOURCES
    lib/main.cpp 
    lib/pch.cpp
    lib/logonserverclient.cpp
)

add_library(azerothlib STATIC ${SOURCES})

# Link against the shared library targets
target_link_libraries(azerothlib PRIVATE shared fmt)

# Include the directories for the shared library
target_include_directories(azerothlib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/shared)