# CMakeLists for tools
project(tools)

# Set up
cmake_minimum_required (VERSION 3.0)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
find_package(ROOT REQUIRED)
find_package(LZ4 REQUIRED)
add_definitions(-D__LZ4__)

SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH "${CMAKE_SOURCE_DIR}/urwelllib")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_SOURCE_DIR}/urwelllib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_SOURCE_DIR}/urwelllib")
ENDIF("${isSystemDir}" STREQUAL "-1")

# Add directories
add_subdirectory(hipo/hipo4)
add_subdirectory(uRWell)
