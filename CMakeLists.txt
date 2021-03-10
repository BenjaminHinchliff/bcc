cmake_minimum_required(VERSION 3.8)

project("tcc")

option(TCC_TESTS "enable testing for tcc" ON)

add_subdirectory("src")

if(TCC_TESTS)
	enable_testing()
	add_subdirectory("test")
endif()
