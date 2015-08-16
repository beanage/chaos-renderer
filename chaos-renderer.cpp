#include <SDL2/SDL.h>
#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <exception>
#include <map>
#include <string>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

#include "chaos-renderer.hpp"
#include "chaos-renderer-program.hpp"

using namespace std;
using namespace chaos;

static const char* const default_vert_shader = R"X(
	#version 330
	layout(location=0)in vec2 vert;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main() {
		gl_Position = vec4(vert.x, vert.y, 0, 1);
		gl_Position *= model;
		gl_Position *= view;
		gl_Position *= projection;
	}
)X";

static const char* const default_frag_shader = R"X(
	#version 330
	out vec4 color;
	void main() {
		color = vec4(1.0, 1.0, 1.0, 1.0);
	}
)X";

/*-------------- Impls ----------------*/

struct renderer::impl
{
	impl() : window(nullptr), main(default_vert_shader, default_frag_shader) {}

	SDL_Window* window;
	SDL_GLContext context;
	program main;
	glm::mat4 view;
};

struct renderer::mesh2d::impl
{
	impl() : vbo(0), ibo(0), vao(0) {}

	GLuint vbo;
	GLuint ibo;
	GLuint vao;
	GLuint index_buffer_size;
};

/*-------------- Funcs ----------------*/

renderer::renderer(int width, int height) : _impl(new impl)
{
	//if(SDL_Init(SDL_INIT_VIDEO) < 0)
	//	throw runtime_error("Failed to video.");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	resize_window(width, height);
	_impl->main.compile();
	glm::mat4 ortho = glm::ortho(.0f, static_cast<float>(width), static_cast<float>(height), .0f);
	_impl->main["projection"] = ortho;
	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j)
			cout << ortho[j][i] << " | ";
		cout << endl;
	}
	_impl->main["view"] = _impl->view;
}

renderer::~renderer()
{}

void renderer::resize_window(int width, int height)
{
	SDL_DestroyWindow(_impl->window);
	_impl->window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if(!_impl->window)
		throw runtime_error("Failed to sdl.");
	_impl->context = SDL_GL_CreateContext(_impl->window);
    if(!_impl->context)
        throw runtime_error(SDL_GetError());
	
	if(SDL_GL_SetSwapInterval(1) < 0)
		throw runtime_error(SDL_GetError());
	
	if(_impl->main.program_id != 0)
		_impl->main["projection"] = glm::ortho(.0f, static_cast<float>(width), static_cast<float>(height), .0f);
}

void renderer::render(mesh2d const& to_draw, glm::mat4 model)
{
	_impl->main["model"] = model;
	to_draw.bind();
	
	//glDrawElements(GL_TRIANGLES, to_draw.size(), GL_UNSIGNED_INT, NULL);
	glDrawArrays(GL_TRIANGLES, 0, to_draw.size());
}

void renderer::flip()
{
	SDL_GL_SwapWindow(_impl->window);
}

renderer::mesh2d::mesh2d(std::initializer_list<float> const& vertices) : _impl(new impl())
{
	vector<float> buffer(vertices);
	if(buffer.size() % 2 != 0)
		throw runtime_error("Invalid data for 2d mesh.");
	
	_impl->index_buffer_size = buffer.size()/2;
	vector<GLuint> indices(_impl->index_buffer_size, 0);
	for(unsigned int i = 0; i < _impl->index_buffer_size; ++i) indices[i] = i;
	
	glGenBuffers(1, &_impl->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _impl->vbo);
	glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(GLfloat), &buffer[0], GL_STATIC_DRAW);
	
	//glGenBuffers(1, &_impl->ibo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _impl->ibo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, _impl->index_buffer_size * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	
	glGenVertexArrays(1, &_impl->vao);
	glBindVertexArray(_impl->vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
}

renderer::mesh2d::~mesh2d()
{
	glDeleteBuffers(1, &_impl->vbo);
	glDeleteBuffers(1, &_impl->ibo);
	glDeleteVertexArrays(1, &_impl->vao);
}

void renderer::mesh2d::bind() const
{
	glBindVertexArray(_impl->vao);
}

size_t renderer::mesh2d::size() const
{
	return _impl->index_buffer_size;
}

void renderer::begin()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	_impl->main();
	glEnableVertexAttribArray(0);
}

void renderer::end()
{
	glDisableVertexAttribArray(0);
	flip();
}

scoped_renderer::scoped_renderer(renderer& target) : target(target)
{
	target.begin();
}

scoped_renderer::~scoped_renderer()
{
	target.end();
}


