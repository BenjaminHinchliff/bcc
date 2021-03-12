cmake_minimum_required(VERSION 3.8)

project("tcc")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTS "enable testing for tcc" ON)
option(BUILD_CLI "enable cli executable construction" ON)

add_subdirectory("src")

if(BUILD_TESTS)
	enable_testing()
	add_subdirectory("test")
endif()

if(BUILD_CLI)
	add_subdirectory("cli")
endif()
