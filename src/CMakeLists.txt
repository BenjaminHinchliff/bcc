cmake_minimum_required(VERSION 3.12)

set(TCC_HDRS_PREFIX
	"../include/")

set(TCC_HDRS
	"tcc/tcc.hpp"
	"tcc/lexer.hpp"
	"tcc/parser.hpp")

list(TRANSFORM TCC_HDRS PREPEND ${TCC_HDRS_PREFIX})

set(TCC_SRCS
	"lexer.cpp"
	"parser.cpp")

add_library(libtcc ${TCC_HDRS} ${TCC_SRCS})
target_include_directories(libtcc PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/${TCC_HDRS_PREFIX}")
