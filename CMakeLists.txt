#-----------------------------------------------------------------------------
# z80asm
# Copyright (c) Paulo Custodio, 2015-2016
# License: http://www.perlfoundation.org/artistic_license_2_0
#-----------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8.4)
project(z80asm CXX)

# Force C++ 98
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98 -U__STRICT_ANSI__")

# The version number.
set(Z80ASM_VERSION_MAJOR 1)
set(Z80ASM_VERSION_MINOR 0)
 
# configure a header file to pass some of the CMake settings
# to the source code
include_directories("${PROJECT_BINARY_DIR}")
configure_file(
	"${PROJECT_SOURCE_DIR}/z80asm_config.h.in"
	"${PROJECT_BINARY_DIR}/z80asm_config.h"
)
 
# create a library with all z80asm code, used by test programs
add_library(z80asmlib STATIC
	dummy.cpp
)

# add the executable
add_executable(
	z80asm 
	z80asm.cpp
)
target_link_libraries(
	z80asm
	z80asmlib
)

# add the install targets
install(
	TARGETS z80asm 
	DESTINATION bin
)

# enable testing
enable_testing()

add_test(
	z80asm_version 
	z80asm
)
set_tests_properties(
	z80asm_version 
	PROPERTIES 
	PASS_REGULAR_EXPRESSION "z80asm version ${Z80ASM_VERSION_MAJOR}.${Z80ASM_VERSION_MINOR}"
)
