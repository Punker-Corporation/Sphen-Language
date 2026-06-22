cmake_minimum_required(VERSION 3.25)
project(Sphen LANGUAGES C)

set(CMAKE_C_STANDARD 99)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)
#set(CMAKE_BUILD_TYPE Debug)

file(GLOB_RECURSE SPHEN_SRC CONFIGURE_DEPENDS
	# "${CMAKE_SOURCE_DIR}/src/*.c"
	"${CMAKE_SOURCE_DIR}/src/main.c"
	"${CMAKE_SOURCE_DIR}/src/*/*.c"
	"${CMAKE_SOURCE_DIR}/src/*/*/*.c"

)
add_executable(Sphen
	${SPHEN_SRC}
)
