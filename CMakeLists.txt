cmake_minimum_required(VERSION 3.28)
project(clib C)
set(PROJECT_VERSION 1.0.0)
set(CMAKE_C_STANDARD 23)
set(CLIB_SOURCES
    clib.c
)
set(CLIB_HEADERS
    clib.h
)
add_library(clib_shared SHARED ${CLIB_SOURCES} ${CLIB_HEADERS}) # Create the shared library
add_library(clib_static STATIC ${CLIB_SOURCES} ${CLIB_HEADERS}) # Create the static library
target_include_directories(clib_shared PUBLIC # Specify the include directories for the library
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/clib>
)
target_include_directories(clib_static PUBLIC # Specify the include directories for the library
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/clib>
)
set_target_properties(clib_shared PROPERTIES OUTPUT_NAME "clib") # Set the properties for the shared library
set_target_properties(clib_static PROPERTIES OUTPUT_NAME "clib") # Set the properties for the static library
target_compile_options(clib_shared PRIVATE -O3 -march=native) # Add optimization flags
target_compile_options(clib_static PRIVATE -O3 -march=native) # Add optimization flags
install(FILES ${CLIB_HEADERS} # Install the header files
    DESTINATION include/clib
)
install(TARGETS clib_shared # Install the shared library
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
)
install(TARGETS clib_static
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
)
include(CMakePackageConfigHelpers) # Create and install the package configuration files
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/clibConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/clibConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/clibConfig.cmake"
    INSTALL_DESTINATION lib/cmake/clib
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/clibConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/clibConfigVersion.cmake"
    DESTINATION lib/cmake/clib
)