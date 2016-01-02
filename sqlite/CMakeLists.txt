cmake_minimum_required(VERSION 3.2)
project(sqlite)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../cmake) # main (top) cmake dir
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake) # project specific cmake dir

set(VERSION_MAJOR "0")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(LIBRARY_NAME sqlite)

set(LIB_HEADERS
        "${CMAKE_CURRENT_BINARY_DIR}/../include/sqlite/Version.h" # Generated header
        sqlite3.h
        sqlite3ext.h
        )

set(LIB_SOURCES
        shell.c
        sqlite3.c)
add_library(${LIBRARY_NAME} SHARED ${LIB_SOURCES})

include(Common)
include(CPackConfig)
include(PackageConfigurator)

install(TARGETS ${LIBRARY_NAME} #${APPLICATION_NAME}
        EXPORT ${PROJECT_NAME}Targets            # for downstream dependencies
        ARCHIVE DESTINATION lib COMPONENT libs   # static lib
        RUNTIME DESTINATION bin COMPONENT libs   # binaries
        LIBRARY DESTINATION lib COMPONENT libs   # shared lib
        PUBLIC_HEADER DESTINATION include/${PROJECT_NAME} COMPONENT devel   # headers (note the different component -> different package)
        FRAMEWORK DESTINATION bin COMPONENT libs # for mac
        )
