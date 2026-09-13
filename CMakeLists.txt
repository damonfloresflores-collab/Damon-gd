cmake_minimum_required(VERSION 3.21)
project(AILevelGenerator VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Geode REQUIRED)

setup_geode_mod(${PROJECT_NAME})
