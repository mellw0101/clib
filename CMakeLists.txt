# cmake_minimum_required(VERSION 3.28)
# project(clib C)

# set(CMAKE_C_STANDARD 23)

# set(CLIB_SOURCES
#     clib.c
# )

# set(CLIB_HEADERS
#     clib.h
# )

# # Create the shared library
# add_library(clib_shared SHARED ${CLIB_SOURCES})

# # Specify the include directories for the library
# target_include_directories(clib_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# # Set the properties for the shared library
# set_target_properties(clib_shared PROPERTIES OUTPUT_NAME "clib")

# # Add optimization flags
# target_compile_options(clib_shared PRIVATE -O3 -march=native)

# # Install the header files
# install(FILES ${CLIB_HEADERS}
#     DESTINATION include/clib
# )

# # Install the shared library
# install(TARGETS clib_shared
#     LIBRARY DESTINATION lib
#     RUNTIME DESTINATION bin
# )

cmake_minimum_required(VERSION 3.28)
project(clib C)

set(PROJECT_VERSION 1.0.0)

set(CMAKE_C_STANDARD 23)

set(CLIB_SOURCES
    clib.c
)

set(CLIB_HEADERS
        clib.h
        /usr/include/unistd.h
        /usr/include/errno.h
        /usr/include/pwd.h
        /usr/include/string.h
        /usr/include/stdlib.h
)

# Create the shared library
add_library(clib_shared SHARED ${CLIB_SOURCES})

# Create the static library
add_library(clib_static STATIC ${CLIB_SOURCES} ${CLIB_HEADERS})

# Specify the include directories for the library
target_include_directories(clib_shared PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/clib>
)

# Specify the include directories for the library
target_include_directories(clib_static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/clib>
)

# Set the properties for the shared library
set_target_properties(clib_shared PROPERTIES OUTPUT_NAME "clib")

# Set the properties for the static library
set_target_properties(clib_static PROPERTIES OUTPUT_NAME "clib")

# Add optimization flags
target_compile_options(clib_shared PRIVATE -O3 -march=native)

# Add optimization flags
target_compile_options(clib_static PRIVATE -O3 -march=native)

# Install the header files
install(FILES ${CLIB_HEADERS}
    DESTINATION include/clib
)

# Install the shared library
install(TARGETS clib_shared
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
)

install(TARGETS clib_static
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
)


# Create and install the package configuration files
include(CMakePackageConfigHelpers)
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