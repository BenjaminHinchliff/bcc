cmake_minimum_required(VERSION 3.12)

set(HDRS_PREFIX
	"../include/")

set(HDRS
	"bcc/bcc.hpp"
	"bcc/lexer.hpp"
	"bcc/parser.hpp"
	"bcc/ast.hpp"
	"bcc/codegen.hpp"
	"bcc/tokens.hpp")

list(TRANSFORM HDRS PREPEND ${HDRS_PREFIX})

set(SRCS
	"lexer.cpp"
	"parser.cpp"
	"ast.cpp"
	"codegen.cpp"
	"tokens.cpp")

set(LIBRARY_NAME "lib${PROJECT_NAME}")
add_library(${LIBRARY_NAME} ${HDRS} ${SRCS})
target_include_directories(${LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/${HDRS_PREFIX}")
