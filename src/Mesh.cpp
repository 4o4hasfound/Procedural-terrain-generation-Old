//#include "Mesh.h"
//
//Mesh::Mesh(glm::vec3 pos, float width, float height) {
//    Position = pos;
//    setupIndices(width, height);
//    generateTerrain(pos, width, height);
//    setupMesh();
//}
//
////Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
////{
////    this->vertices = vertices;
////    this->indices = indices;
////    this->textures = textures;
////
////    // now that we have all the required data, set the vertex buffers and its attribute pointers.
////    setupMesh();
////}
//
//Mesh::Mesh(std::vector<GLfloat> vertices, std::vector<unsigned int> indices) {
//    this->vertices = vertices;
//    this->indices = indices;
//    setupMesh();
//}
//
//void Mesh::generateTerrain(glm::vec3 pos, float width, float height) {
//    vertices.clear();
//    for (int row = 0; row < width + 1; row++) {
//        for (int col = 0; col < height + 1; col++) {
//            glm::vec3 p = getHeight(row * scl + pos.x, col * scl + pos.z);
//            glm::vec3 normal;
//            glm::vec3 color;
//            glm::vec3 U = getHeight((row + 1) * scl + pos.x, col * scl + pos.z) - p;
//            glm::vec3 V = getHeight(row * scl + pos.x, (col + 1) * scl + pos.z) - p;
//            normal = glm::cross(U, V);
//            normal = glm::normalize(normal);
//            //glm::vec3 left = getHeight(pos, (row - 1) * scl + pos.x, col * -scl + pos.z);
//            //glm::vec3 right = getHeight(pos, (row + 1) * scl + pos.x, col * -scl + pos.z);
//            //glm::vec3 up = getHeight(pos, row * scl + pos.x, (col + 1) * -scl + pos.z);
//            //glm::vec3 down = getHeight(pos, row * scl + pos.x, (col - 1) * -scl + pos.z);
//    
//            //float hL = left.y;
//            //float hR = right.y;
//            //float hD = down.y;
//            //float hU = up.y;
//
//            //glm::vec3 N;
//
//            //N.x = hL - hR;
//            //N.y = hD - hU;
//            //N.z = 2.0;
//            //N = normalize(N);
//            vertices.push_back(p.x);
//            vertices.push_back(p.y);
//            vertices.push_back(p.z);
//
//            //std::cout << normal.x << " " << normal.y << " " << normal.z << "\n";
//
//            vertices.push_back(normal.x);
//            vertices.push_back(normal.y);
//            vertices.push_back(normal.z);
//
//        }
//    }
//}
//
//void Mesh::Draw(Shader& shader)
//{
//    // bind appropriate textures
//    //unsigned int diffuseNr = 1;
//    //unsigned int specularNr = 1;
//    //unsigned int normalNr = 1;
//    //unsigned int heightNr = 1;
//    //for (unsigned int i = 0; i < textures.size(); i++)
//    //{
//    //    glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
//    //    // retrieve texture number (the N in diffuse_textureN)
//    //    std::string number;
//    //    std::string name = textures[i].type;
//    //    if (name == "texture_diffuse")
//    //        number = std::to_string(diffuseNr++);
//    //    else if (name == "texture_specular")
//    //        number = std::to_string(specularNr++); // transfer unsigned int to string
//    //    else if (name == "texture_normal")
//    //        number = std::to_string(normalNr++); // transfer unsigned int to string
//    //    else if (name == "texture_height")
//    //        number = std::to_string(heightNr++); // transfer unsigned int to string
//
//    //    // now set the sampler to the correct texture unit
//    //    glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
//    //    // and finally bind the texture
//    //    glBindTexture(GL_TEXTURE_2D, textures[i].id);
//    //}
//
//    // draw mesh
//    glBindVertexArray(VAO);
//    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
//    glBindVertexArray(0);
//
//    // always good practice to set everything back to defaults once configured.
//    //glActiveTexture(GL_TEXTURE0);
//}
//
//void Mesh::setupMesh()
//{
//    // create buffers/arrays
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    glBindVertexArray(VAO);
//    // load data into vertex buffers
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    // A great thing about structs is that their memory layout is sequential for all its items.
//    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
//    // again translates to 3/2 floats which translates to a byte array.
//    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
//
//    // set the vertex attribute pointers
//    // vertex Positions
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
//
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
//
//    glEnableVertexAttribArray(2);
//    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
//}
//
//void Mesh::setupIndices(int width, int height) {
//    for (int row = 0; row < width; row++) {
//        for (int col = 0; col < height; col++) {
//            int index = row * (width + 1) + col;
//            
//            indices.push_back(index);
//            indices.push_back(index + (width + 1) + 1);
//            indices.push_back(index + (width + 1));
//
//            indices.push_back(index);
//            indices.push_back(index + 1);
//            indices.push_back(index + (width + 1) + 1);
//        }
//    }
//}
//
//glm::vec3 Mesh::getHeight(float x, float y) {
//    glm::vec3 p;
//    auto iter = heightMap.find(std::make_pair(x, y));
//    if (iter != heightMap.end())
//        p = glm::vec3(x, iter->second, y);
//    else {
//        heightMap[std::make_pair(x, y)] = noise(glm::abs(x), glm::abs(y));
//        //heightMap[std::make_pair(row * scl + pos.x, col * -scl + pos.z)] = 0.0f;
//        p = glm::vec3(x, heightMap[std::make_pair(x, y)], y);
//    }
//    return p;
//
//}