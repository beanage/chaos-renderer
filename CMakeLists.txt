cmake_minimum_required(VERSION 2.8)


find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

add_executable(main
	main.cpp
	chaos-renderer.cpp
)

target_link_libraries(main
	${OPENGL_LIBRARIES}
	SDL2
)

target_compile_features(main PRIVATE cxx_range_for)