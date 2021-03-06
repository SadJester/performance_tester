set ("${PROJECT}_BINARY_DIR" Binaries/CMake)
set ("${PROJECT}_SOURCE_DIR" utilities)
set ("${PROJECT}_LIB_DIR" lib)

set (CMAKE_INCLUDE_PATH ${${PROJECT}_SOURCE_DIR})
set (CMAKE_LIBRARY_PATH ${${PROJECT}_LIB_DIR})
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/${${PROJECT}_BINARY_DIR})
set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_BUILD_TYPE Debug)

set (NOT_DEFIED_COMPILER "Compiler is not defined.")

if ( WITH_DEBUG_MODE )
     ADD_DEFINITIONS( -D_DEBUG=1)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

hunter_config(Boost
    VERSION ${HUNTER_Boost_VERSION}
    CMAKE_ARGS USE_CONFIG_FROM_BOOST=ON
)

if ( CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
    set(MY_CXX_FLAGS  "-Wall -std=c++17 -std=gnu++17 -fmessage-length=0 -v")
    set(CMAKE_CXX_FLAGS "-O0 ${MY_CXX_FLAGS}")
elseif (MSVC_VERSION GREATER_EQUAL "1900")
 include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++latest")
    endif()
else ()
    message (FATAL_ERROR ${NOT_DEFIED_COMPILER})
endif ()