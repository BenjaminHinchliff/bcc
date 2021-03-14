cmake_minimum_required(VERSION 3.8)

find_package(Catch2 CONFIG QUIET)
if(NOT Catch2_FOUND)
	message(STATUS "getting catch2 from source")
	include(FetchContent)
	FetchContent_Declare(
		catch2
		GIT_REPOSITORY https://github.com/catchorg/Catch2.git
		GIT_TAG v2.13.4
	)

	FetchContent_GetProperties(catch2)
	if(NOT catch2_POPULATED)
		FetchContent_Populate(catch2)
		set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${catch2_SOURCE_DIR}/contrib")
		add_subdirectory(${catch2_SOURCE_DIR} ${catch2_BINARY_DIR})
	endif()
endif()

set(TEST_SRCS
	"lexer_test.cpp"
	"parser_test.cpp"
	"codegen_test.cpp")

set(TESTS_NAME "${PROJECT_NAME}_tests")	
add_executable(${TESTS_NAME} ${TEST_SRCS})

target_link_libraries(${TESTS_NAME} PUBLIC libbcc)
target_link_libraries(${TESTS_NAME} PUBLIC Catch2::Catch2 Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(${TESTS_NAME})
