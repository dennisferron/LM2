# @author  Dennis Ferron
set(LIB_HEADERS
  "${CMAKE_CURRENT_BINARY_DIR}/../include/SharedTables/Version.h" # Generated header
  ../include/SharedTables/DTOXMacros.hpp # Normal header
  ../include/SharedTables/Exceptions.hpp
  ../include/SharedTables/IModelBinder.hpp
  ../include/SharedTables/ITable.hpp
  ../include/SharedTables/ITableDataAllocator.hpp
  ../include/SharedTables/IView.hpp
  ../include/SharedTables/Schema.hpp
  ../include/SharedTables/SchemaXMacros.hpp
  ../include/SharedTables/ShmemAllocator.hpp
  ../include/SharedTables/Table.hpp
  ../include/SharedTables/TableHeader.hpp
  ../include/SharedTables/TableRowSet.hpp
  ../include/SharedTables/View.hpp
)

set(LIB_SOURCES
  InterfaceDestructors.cpp
  Schema.cpp
  Table.cpp
  TableRowSet.cpp
)

#set(APP_HEADERS
#)

#set(APP_SOURCES
#)

# Define two variables in order not to repeat ourselves.
set(LIBRARY_NAME SharedTables)

# Define the library
add_library(${LIBRARY_NAME} SHARED ${LIB_SOURCES})

# Set the build version. It will be used in the name of the lib, with corresponding
# symlinks created. SOVERSION could also be specified for api version. 
set_target_properties(${LIBRARY_NAME} PROPERTIES
  VERSION ${VERSION}  # ${VERSION} was defined in the main CMakeLists.
  FRAMEWORK FALSE
  PUBLIC_HEADER "${LIB_HEADERS}" # specify the public headers
)

#set(APPLICATION_NAME TestSharedTables)
#add_executable(${APPLICATION_NAME} ${APP_SOURCES})

# Links the target exe (projArunner) against the library (libprojA) and other required libraries
target_link_libraries(${APPLICATION_NAME} ${LIBRARY_NAME} ${Boost_PROGRAM_OPTIONS_LIBRARY})

# Says how and where to install software
install(TARGETS ${LIBRARY_NAME} #${APPLICATION_NAME}
  EXPORT ${PROJECT_NAME}Targets            # for downstream dependencies
  ARCHIVE DESTINATION lib COMPONENT libs   # static lib
  RUNTIME DESTINATION bin COMPONENT libs   # binaries
  LIBRARY DESTINATION lib COMPONENT libs   # shared lib
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME} COMPONENT devel   # headers (note the different component -> different package)
  FRAMEWORK DESTINATION bin COMPONENT libs # for mac
)


