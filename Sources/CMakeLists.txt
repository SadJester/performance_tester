#root
cmake_minimum_required (VERSION 3.10)
include("../cmake/HunterGate.cmake")

set(_hunter_result "${CMAKE_GENERATOR}")

HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.299.tar.gz"
    SHA1 "3d215b4bfac80824778fe9a93510e5f651d79e3a"
)

project(PerformanceTester)

message(STATUS ${HUNTER_Boost_VERSION})

hunter_add_package(Boost COMPONENTS container)
find_package(Boost 1.67 CONFIG REQUIRED COMPONENTS container)

set(PROJECT PerformanceTester)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

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
target_link_libraries(PerformanceTester PUBLIC Boost::container)