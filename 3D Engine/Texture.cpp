#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#endif

#ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#endif

void Texture::LoadFromFile(const char* _path)
{
	std::string path = _path;
	size_t lastindex = path.find_last_of(".");
	std::string extension = path.substr(lastindex);
	GLenum format = GL_RGB;
	if (extension == ".png")
	{
		format = GL_RGBA;
	}

	glGenTextures(1, &this->id);
	GLfloat maxAnisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);

	glBindTexture(GL_TEXTURE_2D, this->id);

	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);


	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(_path, &this->width, &this->height, &this->nrChannels, 0);
	if (data)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, format, this->width, this->height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else
	{
		this->path = _path;
		std::cout << "Unable to find texture at path : " << _path << std::endl;
	}
}

void Texture::Refresh()
{
	glBindTexture(GL_TEXTURE_2D, this->id);

	unsigned char* data = stbi_load(this->path.c_str(), &this->width, &this->height, &this->nrChannels, 0);
	if (data)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Unable to find texture at path : " << this->path << std::endl;
	}
}

Texture::Texture()
{
}

Texture::Texture(const char* _path)
{
	if (_path != nullptr)
	{
		LoadFromFile(_path);
	}
}
Texture::~Texture()
{
}