#root
cmake_minimum_required (VERSION 3.10)

project(PerformanceTester)

set(PROJECT PerformanceTester)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# The version number.
set(SDK_VERSION_MAJOR 0)
set(SDK_VERSION_MINOR 1)

#cmake_policy(SET CMP0011 NEW)
#cmake_policy(SET CMP0003 OLD)

option(WITH_TESTS "Build tests" ON)

if(WITH_TESTS)
	enable_testing()
endif()

# setup (could be moved to .cmake file)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../Build/Binaries)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../Build/Binaries)
add_definitions(-DUNICODE)

# Add third party modules
#add_subdirectory(ThirdParty)

# Add components
file(GLOB_RECURSE SOURCE_FILES
    "*.h"
    "*.cpp"
)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_executable(PerformanceTester ${SOURCE_FILES})