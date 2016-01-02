# Game Engine
#
# @author  Dennis Ferron

cmake_minimum_required(VERSION 3.2)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 11) # tODO move up to a general cmake config for all sub projects ?

#enable_testing() # needed on top-level CMakeLists.txt

# Target doc definition
# The challenge here is that the sub projects must all define a custom target but with different names to avoid
# conflicts. We also want to be able to issue "make doc" from the top dir while being able to issue "make doc"
# in the subdirs *when* we compile only a subproject.
# We define as well a function AddDocTarget in cmake/DoxygenTarget.cmake
#add_custom_target(doc) # DEPENDS docSharedTables docTestSharedTables) # Note: with CMake 3.X the DEPENDS would work
#ADD_DEPENDENCIES(doc sqlite SharedTables TestSharedTables)

add_subdirectory(sqlite)
add_subdirectory(SharedTables)
add_subdirectory(TestSharedTables)