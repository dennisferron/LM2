# @author  Dennis Ferron
set(LIB_HEADERS
  "${CMAKE_CURRENT_BINARY_DIR}/../include/TestSharedTables/Version.h"
  #../include/TestSharedTables/Foo.hpp
)

#set(LIB_SOURCES
#)

set(APP_HEADERS
    ../include/TestSharedTables/ExampleDTOs.hpp
    ../include/TestSharedTables/MockTable.hpp
)

set(APP_SOURCES
    ExampleDTOs.cpp
    MockTable.cpp
    TestExampleDTOs.cpp
    TestTable.cpp
    TestView.cpp
    UnitTestMain.cpp
)

# Define two variables in order not to repeat ourselves.
#set(LIBRARY_NAME ProjB)

# Define the library
#add_library(${LIBRARY_NAME} SHARED ${LIB_SOURCES})

# Set the build version. It will be used in the name of the lib, with corresponding
# symlinks created. SOVERSION could also be specified for api version. 
#set_target_properties(${LIBRARY_NAME} PROPERTIES
#  VERSION ${VERSION}  # ${VERSION} was defined in the main CMakeLists.
#  FRAMEWORK FALSE
#  PUBLIC_HEADER "${LIB_HEADERS}" # specify the public headers
#)

# Links the target library against the library (libprojBA)
#target_link_libraries(${LIBRARY_NAME} ProjA)

# This line finds the boost lib and headers.
set(Boost_NO_BOOST_CMAKE ON) #  Don't do a find_package in config mode before searching for a regular boost install.
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# This is a make target, so you can do a "make projBrunner"
set(APPLICATION_NAME TestSharedTables)
add_executable(${APPLICATION_NAME} ${APP_SOURCES})

set(PROJECT_LIBRARIES SharedTables)

# Links the target exe (projBrunner) against the library (libprojB) and other required libraries
target_link_libraries(${APPLICATION_NAME} ${PROJECT_LIBRARIES}
        ${Boost_INTERPROCESS_LIBRARY}
        gtest
        gtest_main)

# Says how and where to install software
install(TARGETS ${LIBRARY_NAME} ${APPLICATION_NAME}
  EXPORT ${PROJECT_NAME}Targets            # for downstream dependencies
  ARCHIVE DESTINATION lib COMPONENT libs   # static lib
  RUNTIME DESTINATION bin COMPONENT libs   # binaries
  LIBRARY DESTINATION lib COMPONENT libs   # shared lib
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME} COMPONENT devel   # headers (note the different component -> different package)
  FRAMEWORK DESTINATION bin COMPONENT libs # for mac
)

