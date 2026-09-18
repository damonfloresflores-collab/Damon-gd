cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(AILevelGenerator VERSION 0.2.0)

# ============================================================
# GEODE SDK
# ============================================================

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR
        "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode"
    )
endif()


# ============================================================
# MPG123 1.33.7
# ============================================================

include(FetchContent)

# No necesitamos programas de MPG123.
set(BUILD_PROGRAMS OFF CACHE BOOL "" FORCE)

# No necesitamos libout123.
set(BUILD_LIBOUT123 OFF CACHE BOOL "" FORCE)

# Queremos la biblioteca integrada en nuestro mod.
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)


FetchContent_Declare(
    mpg123
    URL https://downloads.sourceforge.net/project/mpg123/mpg123/1.33.7/mpg123-1.33.7.tar.bz2
)


FetchContent_GetProperties(mpg123)

if (NOT mpg123_POPULATED)

    FetchContent_Populate(mpg123)

endif()


# ============================================================
# CONSTRUIR MPG123
# ============================================================

add_subdirectory(
    ${mpg123_SOURCE_DIR}/ports/cmake
    ${CMAKE_CURRENT_BINARY_DIR}/mpg123-build
    EXCLUDE_FROM_ALL
)


# ============================================================
# CODIGO DEL MOD
# ============================================================

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)


add_library(
    ${PROJECT_NAME}
    SHARED
    ${SOURCES}
)


target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)


# ============================================================
# MPG123
# ============================================================

# IMPORTANTE:
# No usar PRIVATE/PUBLIC aqui.
# Geode setup_geode_mod() utiliza la firma antigua.
target_link_libraries(
    ${PROJECT_NAME}
    libmpg123
)


# ============================================================
# GEODE
# ============================================================

add_subdirectory(
    $ENV{GEODE_SDK}
    ${CMAKE_CURRENT_BINARY_DIR}/geode
)


setup_geode_mod(
    ${PROJECT_NAME}
)
