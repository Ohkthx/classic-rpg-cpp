cmake_minimum_required(VERSION 3.5)
project(ClassicRPG)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")


# Source files directory.
set(SRC_DIR src)
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")

add_executable(rpg ${SOURCES})
