//#ifndef MESH_H
//#define MESH_H
//
//#include <glad/glad.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//
//#include "shaderClass.h"
//#include "Noise.h"
//
//#include <string>
//#include <vector>
//#include <unordered_map>
//#include <algorithm>
//
//#define scl 1
//
//struct pair_hash {
//    template <class T1, class T2>
//    std::size_t operator() (const std::pair<T1, T2>& pair) const {
//        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
//    }
//};
//
//struct Vertex {
//    // position
//    glm::vec3 Position;
//    // normal
//    glm::vec3 Normal;
//    // texCoords
//    glm::vec2 TexCoords;
//};
//
//class Mesh {
//public:
//    // mesh Data
//    std::vector<GLfloat>       vertices;
//    std::vector<unsigned int> indices;
//    std::vector<Texture>      textures;
//    unsigned int VAO;
//    float scale = 10;
//    glm::vec3 Position;
//
//    Mesh() = default;
//
//    Mesh(glm::vec3 pos, float width, float height);
//    // constructor
//    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
//
//    Mesh(std::vector<GLfloat> vertices, std::vector<unsigned int> indices);
//    
//    void generateTerrain(glm::vec3 pos, float width, float height);
//    // render the mesh
//
//    void Draw(Shader& shader);
//
//private:
//    // render data 
//    unsigned int VBO, EBO;
//    std::unordered_map<std::pair<float, float>, float, pair_hash> heightMap;
//
//    // initializes all the buffer objects/arrays
//    void setupMesh();
//
//    void setupIndices(int width, int height);
//
//    glm::vec3 getHeight(float x, float y);
//};
//#endif