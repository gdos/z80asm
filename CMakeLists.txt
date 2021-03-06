#-----------------------------------------------------------------------------
# z80asm
# Copyright (c) Paulo Custodio, 2015-2016
# License: http://www.perlfoundation.org/artistic_license_2_0
#-----------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8.6)
project(z80asm CXX C)

# Force C++ 98
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98 -U__STRICT_ANSI__ -D_CRT_SECURE_NO_WARNINGS")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS")

#-----------------------------------------------------------------------------
# The version number.
#-----------------------------------------------------------------------------
set(Z80ASM_PROG         "z80asm")
set(Z80ASM_NAME         "Z80 Module Assembler")
set(Z80ASM_VERSION 		"1.0.0")
set(Z80ASM_COPYRIGHT    "(c) Paulo Custodio 2015-2016")

configure_file(
	${PROJECT_SOURCE_DIR}/z80asm_config.h.in
	${PROJECT_BINARY_DIR}/z80asm_config.h
	@ONLY
)
configure_file(
	${PROJECT_SOURCE_DIR}/t/z80asm_config.pm.in
	${PROJECT_BINARY_DIR}/z80asm_config.pm
	@ONLY
)
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES 
	${PROJECT_BINARY_DIR}/z80asm_config.h
	${PROJECT_BINARY_DIR}/z80asm_config.pm
)

#-----------------------------------------------------------------------------
# make lemon
#-----------------------------------------------------------------------------
add_executable(lemon "${PROJECT_SOURCE_DIR}/lemon/lemon.c")
set(LEMON_EXECUTABLE ${PROJECT_BINARY_DIR}/lemon${CMAKE_EXECUTABLE_SUFFIX})
set(LEMON_TEMPLATE 	 ${PROJECT_SOURCE_DIR}/lemon/lempar.c)
add_custom_command(
  OUTPUT	${LEMON_EXECUTABLE}
  DEPENDS	lemon
  COMMAND 	${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:lemon> ${LEMON_EXECUTABLE}
  COMMENT	"Create ${LEMON_EXECUTABLE}"
)

#-----------------------------------------------------------------------------
# make re2c
#-----------------------------------------------------------------------------
set(RE2C_DISABLE_TESTS ON)
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/re2c/re2c")
add_subdirectory("${PROJECT_SOURCE_DIR}/re2c/re2c")
set(RE2C_EXECUTABLE ${PROJECT_BINARY_DIR}/re2c/re2c/re2c${CMAKE_EXECUTABLE_SUFFIX})
add_custom_command(
  OUTPUT	${RE2C_EXECUTABLE}
  DEPENDS	re2c
  COMMAND 	${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:re2c> ${RE2C_EXECUTABLE}
  COMMENT	"Create ${RE2C_EXECUTABLE}"
)

#-----------------------------------------------------------------------------
# directories and configurated files
#-----------------------------------------------------------------------------
include_directories(
	${PROJECT_BINARY_DIR}
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/filesystem
	${PROJECT_SOURCE_DIR}/t
)

#-----------------------------------------------------------------------------
# make parser
#-----------------------------------------------------------------------------
add_custom_command(
	OUTPUT 	${PROJECT_SOURCE_DIR}/gram.cpp ${PROJECT_SOURCE_DIR}/gram.h
	COMMAND ${LEMON_EXECUTABLE}
	ARGS	-s 
			-T${PROJECT_SOURCE_DIR}/lemon/lempar.c
			gram.y
	DEPENDS ${PROJECT_SOURCE_DIR}/gram.y 
			${LEMON_EXECUTABLE}
	COMMENT "[LEMON] Building parser with ${LEMON_EXECUTABLE}"
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

#-----------------------------------------------------------------------------
# make scanner
#-----------------------------------------------------------------------------
add_custom_command(
	OUTPUT 	${PROJECT_SOURCE_DIR}/scanner.cpp
	COMMAND ${RE2C_EXECUTABLE}
	ARGS	-i --no-generation-date -W 
			-oscanner.cpp
			scanner.re
	DEPENDS ${PROJECT_SOURCE_DIR}/scanner.re 
			${PROJECT_SOURCE_DIR}/gram.h 
			${RE2C_EXECUTABLE}
	COMMENT "[RE2C] Building scanner with ${RE2C_EXECUTABLE}"
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set_source_files_properties(${PROJECT_SOURCE_DIR}/parser.cpp
	PROPERTIES OBJECT_DEPENDS ${PROJECT_SOURCE_DIR}/gram.h 
)

set_source_files_properties(${PROJECT_SOURCE_DIR}/token.cpp
	PROPERTIES OBJECT_DEPENDS ${PROJECT_SOURCE_DIR}/gram.h 
)

#-----------------------------------------------------------------------------
# make program
#-----------------------------------------------------------------------------
file(GLOB SOURCES 
	*.cpp
	*.h
	*.re
	*.y
)

add_library(z80asmlib STATIC
	${PROJECT_SOURCE_DIR}/gram.cpp
	${PROJECT_SOURCE_DIR}/scanner.cpp
	${SOURCES}
	${PROJECT_BINARY_DIR}/z80asm_config.h
)

# add the executable
add_executable(z80asm
	z80asm.cpp
)
target_link_libraries(z80asm
	z80asmlib
)

# add the install targets
install(
	TARGETS 	z80asm 
	DESTINATION bin
)

#-----------------------------------------------------------------------------
# tests
#-----------------------------------------------------------------------------
enable_testing()

add_test(z80asm_version
	z80asm
)
set_tests_properties(z80asm_version 
	PROPERTIES 	PASS_REGULAR_EXPRESSION 
    "${Z80ASM_NAME} ${Z80ASM_VERSION}"    # copyright has (c) which is interpreted as regexp subexpression and fails match
)

file(GLOB EXE_TESTS 
	t/*_test.cpp
)
file(GLOB PERL_TESTS 
	t/*_test.t
)
foreach(TEST ${EXE_TESTS})
    get_filename_component(TEST_PROG ${TEST} NAME_WE)
	add_executable(${TEST_PROG}
		${TEST}
		t/taptest.h 
		t/test.h
	)
	target_link_libraries(${TEST_PROG}
		z80asmlib
	)
	add_test(
		NAME				${TEST_PROG}
		COMMAND				${TEST_PROG}
		WORKING_DIRECTORY	${PROJECT_BINARY_DIR}
	)
endforeach(TEST ${EXE_TESTS})

foreach(TEST ${PERL_TESTS})
    get_filename_component(TEST_PROG ${TEST} NAME)
	add_test(
		NAME				${TEST_PROG}
		COMMAND				perl ${PROJECT_SOURCE_DIR}/t/${TEST_PROG}
		WORKING_DIRECTORY	${PROJECT_BINARY_DIR}
	)
endforeach(TEST ${PERL_TESTS})
