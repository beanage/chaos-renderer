#include "chaos-texture.hpp"
#include "chaos-plattform-gl.hpp"
#include "stb/stb_image.h"

#include <iostream>

using namespace chaos;
using namespace std;

struct texture2d::impl
{
	GLuint id;
	unsigned int width;
	unsigned int height;
};

texture2d::texture2d(string const& filename) : _impl(new impl)
{
	int width = 0, height = 0, comp = 0;
	unsigned char* buffer = stbi_load(filename.c_str(), &width, &height, &comp, STBI_rgb_alpha);

	if (!buffer)
		throw runtime_error("Error loading texture.");

	_impl->width = static_cast<int>(width);
	_impl->height = static_cast<int>(height);

	cout << "Loaded image w=" << _impl->width << " h=" << _impl->height << endl;

	glGenTextures(1, &_impl->id);
	glBindTexture(GL_TEXTURE_2D, _impl->id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	glBindTexture(GL_TEXTURE_2D, 0);
	free(buffer);
}

texture2d::~texture2d()
{
	glDeleteTextures(1, &_impl->id);
}

unsigned int texture2d::width() const
{
	return _impl->width;
}

unsigned int texture2d::height() const
{
	return _impl->height;
}

void texture2d::bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _impl->id);
}
