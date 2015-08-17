cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Debug)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

add_executable(demo
	src/main.cpp
	src/chaos-renderer.cpp
)

target_link_libraries(demo
	${OPENGL_LIBRARIES}
	GLEW
	SDL2
)

target_compile_features(demo PRIVATE cxx_range_for)
