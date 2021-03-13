cmake_minimum_required(VERSION 3.8)

project("bcc")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTS "enable testing for bcc" ON)
option(BUILD_CLI "enable cli executable construction" ON)

add_subdirectory("src")

if(BUILD_TESTS)
	enable_testing()
	add_subdirectory("tests")
endif()

if(BUILD_CLI)
	add_subdirectory("cli")
endif()
