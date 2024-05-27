cmake_minimum_required(VERSION 3.28)
project(clib C)
set(PROJECT_VERSION 1.0.0)
set(CMAKE_C_STANDARD 23)
enable_language(ASM_NASM) # enable the assembly language

# Defined cmake functions !!!

function(comp_nasm asm_obj asm_src c_obj c_src)
    message(STATUS "asm_obj out file path: ${asm_obj}")
    message(STATUS "asm_src in file path: ${asm_src}")
    message(STATUS "c_obj out file path: ${c_obj}")
    message(STATUS "c_src in file path: ${c_src}")

    # Add a custom command to compile the NASM source file
    add_custom_command(
        OUTPUT ${asm_obj}
        COMMAND nasm -f elf64 -o ${asm_obj} ${asm_src}
        DEPENDS ${asm_src}
        COMMENT "Compiling NASM source file ${asm_src}"
    )

    # Add a custom command to compile the C source file
    add_custom_command(
        OUTPUT ${c_obj}
        COMMAND gcc -c ${c_src} -o ${c_obj}
        DEPENDS ${c_src}
        COMMENT "Compiling C source file ${c_src}"
    )

    # Create a custom target that depends on the compiled object files
    add_custom_target(
        compile_${asm_obj}_${c_obj} ALL
        DEPENDS ${asm_obj} ${c_obj}
    )
endfunction()
function(comp_nasm_by_name file_prefix_name)
    message(STATUS "file name without and .extention: ${file_prefix_name}")

    set(asm_src src/asm/src/${file_prefix_name}.s)
    set(asm_obj src/asm/obj/${file_prefix_name}.o)
    set(c_src src/asm/src/${file_prefix_name}.c)
    set(c_obj src/asm/obj/${file_prefix_name}_c.o)

    # Add a custom command to compile the NASM source file
    add_custom_command(
        OUTPUT ${asm_obj}
        COMMAND nasm -f elf64 -o ${asm_obj} ${asm_src}
        DEPENDS ${asm_src}
        COMMENT "Compiling NASM source file: ${asm_src}"
    )

    # Add a custom command to compile the C source file
    add_custom_command(
        OUTPUT ${c_obj}
        COMMAND gcc -c -o ${c_obj} ${c_src} 
        DEPENDS ${c_src}
        COMMENT "Compiling C source file ${c_src}"
    )

    # Create a custom target that depends on the compiled object files
    add_custom_target(
        compile_${asm_obj}_${c_obj} ALL
        DEPENDS ${asm_obj} ${c_obj}
    )
endfunction()

# set(__ASM__OBJECT_FILES__
#     comp_nasm_by_name("io")
# )
set(__ASM__OBJECT_FILES__
    src/asm/obj/io.o
    src/asm/obj/io_c.o
    src/asm/obj/strlen.o
    src/asm/obj/strlen_c.o
)
set(CLIB_SOURCES
    src/clib.c
)
set(CLIB_HEADERS
    src/clib.h
    src/asm/__asm__.h
)
add_library(clib_shared SHARED # Create the shared library
    ${CLIB_SOURCES}
)
add_library(clib_static STATIC # Create the static library
    ${CLIB_SOURCES}
    ${CLIB_HEADERS}
    ${__ASM__OBJECT_FILES__}
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