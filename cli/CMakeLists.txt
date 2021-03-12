cmake_minimum_required(VERSION 3.8)

set(CLI_SRCS
	"main.cpp")

add_executable(tcc ${CLI_SRCS})

target_link_libraries(tcc PUBLIC libtcc)
