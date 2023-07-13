#pragma once
#ifndef _CHUNK_H_
#define _CHUNK_H_

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include "FastNoiseLite.h"

#include "Shader.h"
#include "Noise.h"
#include "Texture.h"
#include "Water.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <chrono>

#define scl 1
#define HEIGHT 550

#define SNOW_HEIGHT 0.2
#define GRASS_HEIGHT 0.05
#define WATER_HEIGHT -0.1

struct pair_hashes {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

class Chunk {
public:
    std::vector<GLfloat>       vertices;
    std::vector<unsigned int> *indices;
    unsigned int VAO;
    float scale = 10;
    glm::vec3 Position;

    Chunk() = default;

    Chunk(glm::vec3 pos, float size, std::vector<unsigned>* indices);

    Chunk(std::vector<GLfloat> vertices, std::vector<unsigned int>* indices);

    void generateTerrain(glm::vec3 pos, float size);

    void Draw(Shader & shader);

    void DrawWater();

private:

    unsigned int VBO, EBO;
    Water water;

    void setupMesh();

    float getHeight(float x, float y);
    glm::vec3 getHeightVec3(float x, float y);

    std::unordered_map<std::pair<int, int>, float, pair_hashes> table;
};

class ChunkManager {
public:
    ChunkManager();
    ChunkManager(glm::vec3 pos, int chunkSize, int chunkWidth, int chunkHeight);
    void Update(glm::vec3 pos);
    void Resize(int terrainWidth, int terrainHeight);
    void Generate();
    void Draw(Shader& shader);
    void SetupTextures();
    void BindTextures();

    void DrawWater();

    float snow_height = SNOW_HEIGHT;
    float grass_height = GRASS_HEIGHT;
    float water_height = WATER_HEIGHT;

private:
    void SetupIndices();
    void LoadTexture(std::string filename,unsigned *var);
    int ChunkSize;
    int ChunkWidth;
    int ChunkHeight;
    
    Texture snow, dirt, grass, sand,
        snowNor, dirtNor, grassNor, sandNor,
        water;
    std::vector<unsigned> indices;

    std::vector<glm::vec2> toGenerate;
    std::vector<Chunk*> chunks;
};
#endif