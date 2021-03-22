cmake_minimum_required(VERSION 3.14)

include(FetchContent)
FetchContent_Declare(
	cli11	
	GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
	GIT_TAG v1.9.1
)
FetchContent_MakeAvailable(cli11)

set(CLI_SRCS
	"main.cpp")

set(CLI_NAME ${PROJECT_NAME})
add_executable(${CLI_NAME} ${CLI_SRCS})

target_link_libraries(${CLI_NAME} PUBLIC libbcc CLI11)
