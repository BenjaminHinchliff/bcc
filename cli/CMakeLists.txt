cmake_minimum_required(VERSION 3.8)

set(CLI_SRCS
	"main.cpp")

set(CLI_NAME ${PROJECT_NAME})
add_executable(${CLI_NAME} ${CLI_SRCS})

target_link_libraries(${CLI_NAME} PUBLIC libtcc)
