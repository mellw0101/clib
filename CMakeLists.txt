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


# # Paths to LLVM assembly files
# set(MY_EXIT_LL ${CMAKE_CURRENT_SOURCE_DIR}/clib_exit.ll)
# set(MY_CLOSE_LL ${CMAKE_CURRENT_SOURCE_DIR}/clib_close.ll)
# set(MY_PERROR_LL ${CMAKE_CURRENT_SOURCE_DIR}/clib_perror.ll)
# set(MY_OPEN_LL ${CMAKE_CURRENT_SOURCE_DIR}/clib_open.ll)

# # Compile the LLVM assembly files to object files
# add_custom_command(
#     OUTPUT clib_exit.o
#     COMMAND clang -c -o clib_exit.o ${MY_EXIT_LL}
#     DEPENDS ${MY_EXIT_LL}
#     COMMENT "Compiling LLVM assembly for exit to object file"
# )

# add_custom_command(
#     OUTPUT clib_close.o
#     COMMAND clang -c -o clib_close.o ${MY_CLOSE_LL}
#     DEPENDS ${MY_CLOSE_LL}
#     COMMENT "Compiling LLVM assembly for close to object file"
# )

# add_custom_command(
#     OUTPUT clib_perror.o
#     COMMAND clang -c -o clib_perror.o ${MY_PERROR_LL}
#     DEPENDS ${MY_PERROR_LL}
#     COMMENT "Compiling LLVM assembly for perror to object file"
# )

# add_custom_command(
#     OUTPUT clib_open.o
#     COMMAND clang -c -o clib_open.o ${MY_OPEN_LL}
#     DEPENDS ${MY_OPEN_LL}
#     COMMENT "Compiling LLVM assembly for open to object file"
# )

# # Add custom targets to build the object files
# add_custom_target(
#     MyExitObj ALL
#     DEPENDS clib_exit.o
# )

# add_custom_target(
#     MyCloseObj ALL
#     DEPENDS clib_close.o
# )

# add_custom_target(
#     MyPerrorObj ALL
#     DEPENDS clib_perror.o
# )

# add_custom_target(
#     MyOpenObj ALL
#     DEPENDS clib_open.o
# )

set(PROJECT_VERSION 1.0.0)


set(CMAKE_C_STANDARD 23)


set(CLIB_SOURCES
        clib.c
)

set(CLIB_HEADERS
        clib.h
        def.h
        /usr/include/unistd.h
        /usr/include/errno.h
        /usr/include/pwd.h
        /usr/include/string.h
        /usr/include/stdlib.h
        /usr/include/stdio.h
)

# Create the shared library
add_library(clib_shared SHARED ${CLIB_SOURCES})

# Create the static library

add_library(clib_static STATIC ${CLIB_SOURCES} ${CLIB_HEADERS} )

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