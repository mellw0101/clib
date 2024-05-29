cmake_minimum_required(VERSION 3.28)
project(clib C)
set(PROJECT_VERSION 1.0.0)
set(CMAKE_C_STANDARD 23)
enable_language(ASM_NASM) # enable the assembly language

# Defined cmake functions !!!
function(comp_nasm_by_name file_prefix_name)
    message(STATUS "file name without extension: ${file_prefix_name}")

    set(asm_src ${CMAKE_SOURCE_DIR}/src/${file_prefix_name}.asm)
    set(asm_obj ${CMAKE_SOURCE_DIR}/obj/${file_prefix_name}.o)

    add_custom_command(
        OUTPUT ${asm_obj}
        COMMAND nasm -f elf64 -o ${asm_obj} ${asm_src}
        DEPENDS ${asm_src}
        COMMENT "Compiling NASM source file: ${asm_src} to ${asm_obj}"
    )

    add_custom_target(
        compile_${file_prefix_name} ALL
        DEPENDS ${asm_obj}
    )
endfunction()
comp_nasm_by_name(lib)

# Specify the source files and headers
set(CLIB_SOURCES
    ${CMAKE_SOURCE_DIR}/src/clib.c
)
set(CLIB_HEADERS
    ${CMAKE_SOURCE_DIR}/src/clib.h
)

# Create the shared library
add_library(clib_shared SHARED
    ${CLIB_SOURCES}
)

add_library(clib_static STATIC
    ${CLIB_SOURCES}
    ${CMAKE_SOURCE_DIR}/obj/lib.o
)

target_include_directories(clib_shared PUBLIC # Specify the include directories for the library
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/clib>
)
target_include_directories(clib_static PUBLIC # Specify the include directories for the library
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/clib>
)
set_target_properties(clib_shared PROPERTIES OUTPUT_NAME "clib") # Set the properties for the shared library
set_target_properties(clib_static PROPERTIES OUTPUT_NAME "clib") # Set the propertbry
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