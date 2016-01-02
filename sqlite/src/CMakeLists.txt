
set(LIB_HEADERS
        "${CMAKE_CURRENT_BINARY_DIR}/../include/sqlite/Version.h" # Generated header
        ../include/sqlite/sqlite3.h
        ../include/sqlite/sqlite3ext.h
        )

set(LIB_SOURCES
        shell.c
        sqlite3.c)

set(LIBRARY_NAME sqlite)

add_library(${LIBRARY_NAME} SHARED ${LIB_SOURCES})

install(TARGETS ${LIBRARY_NAME} #${APPLICATION_NAME}
        EXPORT ${PROJECT_NAME}Targets            # for downstream dependencies
        ARCHIVE DESTINATION lib COMPONENT libs   # static lib
        RUNTIME DESTINATION bin COMPONENT libs   # binaries
        LIBRARY DESTINATION lib COMPONENT libs   # shared lib
        PUBLIC_HEADER DESTINATION include/${PROJECT_NAME} COMPONENT devel   # headers (note the different component -> different package)
        FRAMEWORK DESTINATION bin COMPONENT libs # for mac
        )
