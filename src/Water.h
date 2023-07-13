#pragma once
#ifndef _WATER_H_
#define _WATER_H_

#include <GLM/glm.hpp>
#include <vector>
#include <glad/glad.h>

#include <iostream>

class Water {
public:
	Water(){}
	Water(glm::vec2 pos, int size);
	void Draw();
private:
	std::vector<GLfloat>       vertices;
	std::vector<unsigned int> indices;

	unsigned int VAO, VBO, EBO;
	glm::vec3 Position;
	float Height;
	int Size;

	void SetupWater();
};

#endif