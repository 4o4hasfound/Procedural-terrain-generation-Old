#pragma once
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <GLM/glm.hpp>
#include <glad/glad.h>
#include <stb/stb_image.h>
#include <iostream>

class Texture {
public:
	Texture() {}
	Texture(std::string filename, GLenum type = GL_RGB);
	void Bind();
private:
	unsigned int id;
};

#endif