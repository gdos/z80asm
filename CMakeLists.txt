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

include_directories(
	"${PROJECT_BINARY_DIR}"
	"${PROJECT_SOURCE_DIR}"
	"${PROJECT_SOURCE_DIR}/filesystem"
	"${PROJECT_SOURCE_DIR}/t"
)

# Need re2c
include("CMake/FindRE2C.cmake")
RE2C_TARGET(PreprocScanner preproc.re  "${CMAKE_CURRENT_BINARY_DIR}/preproc.cpp")

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
	"${PROJECT_SOURCE_DIR}/z80asm_config.h.in"
	"${PROJECT_BINARY_DIR}/z80asm_config.h"
)
 
# create a library with all z80asm code, used by test programs
add_library(z80asmlib STATIC
	dummy.cpp
	source.cpp
	util.cpp
	${RE2C_PreprocScanner_OUTPUTS}
)

# add the executable
add_executable(			z80asm	z80asm.cpp 	)
target_link_libraries(	z80asm	z80asmlib	)

# add the install targets
install(	TARGETS 	z80asm 
			DESTINATION bin
)

# enable testing
enable_testing()

add_test(				z80asm_version		z80asm	)
set_tests_properties(	z80asm_version 
	PROPERTIES 	PASS_REGULAR_EXPRESSION "z80asm version ${Z80ASM_VERSION_MAJOR}.${Z80ASM_VERSION_MINOR}"
)

foreach(TEST_PROG	
		t_filesystem
		t_source
		t_util
	)
	add_executable(			${TEST_PROG}	"t/${TEST_PROG}.cpp" 	)
	target_link_libraries(	${TEST_PROG}	z80asmlib				)
	add_test(				${TEST_PROG}	${TEST_PROG}			)
endforeach(TEST_PROG)
