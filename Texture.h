#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stb/stb_image.h>

#include"Shader.h"

class Texture
{
public:
	Texture(const char* image, const char* texType, GLenum slot, GLenum format, GLenum pixelType);
	~Texture();

	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();

	std::string getType() { return std::string(type); }
	GLuint getID()        { return ID; }

private:
	GLuint ID;
	const char* type;
	GLuint unit;
};
#endif
