# REFERENCE PROJECT CMAKELISTS.TXT
#
# @author Dennis Ferron

cmake_minimum_required(VERSION 3.2)
project(sqlite)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../cmake) # main (top) cmake dir
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake) # project specific cmake dir

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(VERSION_MAJOR "0")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

include(Common)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/sqlite
        ${PROJECT_BINARY_DIR}/include   # add the binary tree to the search path so that we will find Version.h
        ${Boost_INCLUDE_DIRS}
)

enable_testing()
add_subdirectory(include)
add_subdirectory(src)

include(CPackConfig)
include(PackageConfigurator)
