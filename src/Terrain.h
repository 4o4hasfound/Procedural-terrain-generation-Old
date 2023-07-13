#pragma once
#ifndef _TERRAIN_H_
#define _TERRAIN_H_
#include <iostream>

#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Chunk.h"

class Terrain {
public:
	Terrain();
	Terrain(Camera* camera, int screenWidth, int screenHeight, int terrainWidth, int terrainHeight, int chunkSize);
	void Update(glm::vec3 pos);
	void Resize(int terrainWidth, int terrainHeight);
	void Generate();
	void Draw(glm::mat4 proj, glm::mat4 view);
	int getTerrainWidth();
	int getTerrainHeight();
	bool useTextures = true;
	bool useSkyBox = true;
	bool useNormalMapping = true;
	bool useLighting = true;
	bool useColoring = true;
	bool drawWater = true;
	float heightScale = 450.0f;
	float snowLevel = 0.2f;
	float grassLevel = 0.05f;
	float waterLevel = -0.1f;
private:
	Camera* camera;
	int ScreenWidth, ScreenHeight, TerrainWidth, TerrainHeight, ChunkSize;
	
	Shader shader, waterShader;
	ChunkManager chunkManager;
	glm::vec3 lightDir = { 0.0f, 1.0f, 0.0f };
};

#endif