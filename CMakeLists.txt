cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(AILevelGenerator VERSION 0.2.0)

# =========================================================
# GEODE SDK
# =========================================================

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR
        "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode"
    )
endif()

# =========================================================
# MPG123
# =========================================================

include(FetchContent)

set(BUILD_PROGRAMS OFF CACHE BOOL "" FORCE)
set(BUILD_LIBOUT123 OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    mpg123
    URL https://downloads.sourceforge.net/project/mpg123/mpg123/1.33.7/mpg123-1.33.7.tar.bz2
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_GetProperties(mpg123)

if (NOT mpg123_POPULATED)
    FetchContent_Populate(mpg123)
endif()

add_subdirectory(
    ${mpg123_SOURCE_DIR}/ports/cmake
    ${CMAKE_CURRENT_BINARY_DIR}/mpg123-build
    EXCLUDE_FROM_ALL
)

# =========================================================
# NUESTRO MOD
# =========================================================

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(
    ${PROJECT_NAME}
    SHARED
    ${SOURCES}
)

# Includes del mod + MPG123
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src

    # MPG123 -> aquí está mpg123.h
    ${mpg123_SOURCE_DIR}/src/include
)

# =========================================================
# LINK MPG123
# =========================================================

# IMPORTANTE:
# NO usar PRIVATE/PUBLIC aquí.
# setup_geode_mod() utiliza la firma plain de CMake.
target_link_libraries(
    ${PROJECT_NAME}
    libmpg123
)

# =========================================================
# GEODE
# =========================================================

add_subdirectory(
    $ENV{GEODE_SDK}
    ${CMAKE_CURRENT_BINARY_DIR}/geode
)

setup_geode_mod(
    ${PROJECT_NAME}
)
