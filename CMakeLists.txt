cmake_minimum_required(VERSION 3.8)

project("tcc")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(TCC_TESTS "enable testing for tcc" ON)

add_subdirectory("src")

if(TCC_TESTS)
	enable_testing()
	add_subdirectory("test")
endif()
