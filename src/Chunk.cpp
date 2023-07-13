#include "Chunk.h"
Chunk::Chunk(glm::vec3 pos, float size,std::vector<unsigned>* indices):Position(pos),indices(indices) {
    generateTerrain(pos, size);
    setupMesh();
    water = Water(glm::vec2(pos.x, pos.z), size);
}

Chunk::Chunk(std::vector<GLfloat> vertices, std::vector<unsigned int> *indices) {
    this->vertices = vertices;
    this->indices = indices;
    setupMesh();
}

void Chunk::generateTerrain(glm::vec3 pos, float size) {
    vertices.clear();
    for (int row = 0; row < size + 1; row++) {
        for (int col = 0; col < size + 1; col++) {
            glm::vec3 p = getHeightVec3(row * scl + pos.x, col * scl + pos.z);
            glm::vec3 normal;
            //glm::vec3 U = getHeightVec3((row + 1) * scl + pos.x, col * scl + pos.z);
            //U.x = 105; U.z = 104;
            //glm::vec3 V = getHeightVec3(row * scl  + pos.x, (col + 1) * scl + pos.z);
            //V.x = 104; V.z = 105;
            //normal = glm::cross(U, V);
            //normal = glm::normalize(normal);ee
            float st = 1.0;
            float dhdu = (getHeight((p.x + st), p.z)*HEIGHT - getHeight((p.x - st), p.z)* HEIGHT)  / (2.0 * st);
            float dhdv = (getHeight(p.x, (p.z + st)) * HEIGHT - getHeight(p.x, (p.z - st)) * HEIGHT) / (2.0 * st);
            glm::vec3 X(1.0, dhdu, 1.0);

            glm::vec3 Z(0.0, dhdv, 1.0);

            normal = normalize(cross(Z, X));
            //std::cout << normal.x << " " << normal.y << " " << normal.z << "\n";
            glm::vec3 pos1 = p;
            glm::vec3 pos2=getHeightVec3((row + 1) * scl + pos.x, col * scl + pos.z);
            glm::vec3 pos3=getHeightVec3(row * scl + pos.x, (col + 1) * scl + pos.z);
            glm::vec3 pos4=getHeightVec3((row + 1) * scl + pos.x, (col + 1) * scl + pos.z);

            glm::vec2 uv1(p.x / 20.0f, p.z / 20.0f);
            glm::vec2 uv2(pos2.x / 20.0f, pos2.z / 20.0f);
            glm::vec2 uv3(pos3.x / 20.0f, pos3.z / 20.0f);
            glm::vec2 uv4(pos4.x / 20.0f, pos4.z / 20.0f);

            glm::vec3 edge1 = pos2 - pos1;
            glm::vec3 edge2 = pos3 - pos1;
            glm::vec2 deltaUV1 = uv2 - uv1;
            glm::vec2 deltaUV2 = uv3 - uv1;

            glm::vec3 tangent1, bitangent1, tangent2, bitangent2;

            float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

            bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
            bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
            bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

            glm::vec3 edge3 = pos4 - pos1;
            glm::vec2 deltaUV3 = uv4 - uv1;

            f = 1.0f / (deltaUV1.x * deltaUV3.y - deltaUV3.x * deltaUV1.y);

            tangent2.x = f * (deltaUV3.y * edge1.x - deltaUV1.y * edge3.x);
            tangent2.y = f * (deltaUV3.y * edge1.y - deltaUV1.y * edge3.y);
            tangent2.z = f * (deltaUV3.y * edge1.z - deltaUV1.y * edge3.z);

            bitangent2.x = f * (-deltaUV3.x * edge1.x + deltaUV1.x * edge3.x);
            bitangent2.y = f * (-deltaUV3.x * edge1.y + deltaUV1.x * edge3.y);
            bitangent2.z = f * (-deltaUV3.x * edge1.z + deltaUV1.x * edge3.z);

            glm::vec3 tangent = tangent1 + tangent2;
            glm::vec3 bitangent = bitangent1 + bitangent2;

            vertices.emplace_back(p.x);
            vertices.emplace_back(p.y);
            vertices.emplace_back(p.z);

            vertices.emplace_back(normal.x);
            vertices.emplace_back(normal.y);
            vertices.emplace_back(normal.z);

            vertices.emplace_back(p.x / 20.0f);
            vertices.emplace_back(p.z / 20.0f);

            vertices.emplace_back(tangent.x / 2);
            vertices.emplace_back(tangent.y / 2);
            vertices.emplace_back(tangent.z / 2);

            vertices.emplace_back(bitangent.x / 2);
            vertices.emplace_back(bitangent.y / 2);
            vertices.emplace_back(bitangent.z / 2);
        }
    }
}

void Chunk::Draw(Shader& shader) {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices->size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Chunk::DrawWater() {
    water.Draw();
}

void Chunk::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(unsigned int), indices->data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
}


float Chunk::getHeight(float x, float y) {
    if (table.count(std::make_pair(x, y)))
        return table[std::make_pair(x, y)];
    table[std::make_pair(x, y)] = noise(x, y);
    return table[std::make_pair(x, y)];
}

glm::vec3 Chunk::getHeightVec3(float x, float y) {
    return glm::vec3(x, getHeight(x,y), y);
}

ChunkManager::ChunkManager() {

}
ChunkManager::ChunkManager(glm::vec3 pos, int chunkSize, int chunkWidth, int chunkHeight):ChunkSize(chunkSize),ChunkWidth(chunkWidth),ChunkHeight(chunkHeight) {
    SetupIndices();
    Update(pos);
}

void ChunkManager::Update(glm::vec3 pos) {
    std::vector<glm::vec3> existPos;
    std::vector<Chunk*> chunkLeft;
    #pragma omp parallel for
    for (int i = 0; i < chunks.size(); i++) {
        if ((glm::abs(pos.x-chunks[i]->Position.x>ChunkWidth/2*ChunkSize)||(glm::abs(pos.z-chunks[i]->Position.z)>ChunkHeight/2*ChunkSize))) {
            delete chunks[i];
        }
        else {
            chunkLeft.emplace_back(chunks[i]);
            existPos.emplace_back(chunks[i]->Position);
        }
    }
    toGenerate.clear();
    #pragma omp parallel for
    for (int row = -ChunkWidth/2; row < ChunkWidth / 2; row++) {
        #pragma omp parallel for
        for (int col = -ChunkHeight/2; col < ChunkWidth / 2; col++) {
            glm::vec3 p(pos.x + row * ChunkSize, 0, pos.z +col * ChunkSize);
            auto itr = std::find(existPos.begin(), existPos.end(), p);
            if (itr == existPos.end()) {
                toGenerate.emplace_back(p.x,p.z);
            }
        }
    }
    std::sort(toGenerate.begin(), toGenerate.end(), [&, pos](const glm::vec2& a, const glm::vec2& b) {
        return glm::abs(pos.x - a.x) + glm::abs(pos.z - a.y) < glm::abs(pos.x - b.x) + glm::abs(pos.z - b.y);
        });
    chunks = chunkLeft;
}

void ChunkManager::Resize(int terrainWidth, int terrainHeight) {
    ChunkWidth = terrainWidth;
    ChunkHeight = terrainHeight;
}

void ChunkManager::Generate() {
    if (toGenerate.size()) {
        chunks.emplace_back(new Chunk(glm::vec3(toGenerate[0].x, 0, toGenerate[0].y), ChunkSize, &indices));
        toGenerate.erase(toGenerate.begin());
    }
}

void ChunkManager::Draw(Shader& shader) {
    for (Chunk*& chunk : chunks) {
        chunk->Draw(shader);
    }
}

void ChunkManager::SetupIndices() {
    #pragma omp parallel for
    for (int row = 0; row < ChunkSize; row++) {
        #pragma omp parallel for
        for (int col = 0; col < ChunkSize; col++) {
            int index = row * (ChunkSize + 1) + col;

            indices.emplace_back(index);
            indices.emplace_back(index + (ChunkSize + 1) + 1);
            indices.emplace_back(index + (ChunkSize + 1));

            indices.emplace_back(index);
            indices.emplace_back(index + 1);
            indices.emplace_back(index + (ChunkSize + 1) + 1);
        }
    }
}

void ChunkManager::SetupTextures() {
    snow = Texture("textures/snow/1k/Albedo.jpg");
    grass = Texture("textures/grass/1k/Albedo.jpg");
    dirt = Texture("textures/dirt/1k/Albedo.jpg");
    sand = Texture("textures/sand/1k/Albedo.jpg");

    snowNor = Texture("textures/snow/1k/Normal.jpg");
    grassNor = Texture("textures/grass/1k/Normal.jpg");
    dirtNor = Texture("textures/dirt/1k/Normal.jpg");
    sandNor = Texture("textures/sand/1k/Normal.jpg");

    water = Texture("textures/water/water.jpg");
}

void ChunkManager::LoadTexture(std::string filename,unsigned *var) {
    glGenTextures(1, var);
    glBindTexture(GL_TEXTURE_2D, *var);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, (nrChannels == 3 ? GL_RGB : GL_RGBA), GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Error when loading " << filename << "\n";
    }
    stbi_image_free(data);
}

void ChunkManager::BindTextures() {
    glActiveTexture(GL_TEXTURE1);
    snow.Bind();
    glActiveTexture(GL_TEXTURE2);
    snowNor.Bind();
    glActiveTexture(GL_TEXTURE3);
    grass.Bind();
    glActiveTexture(GL_TEXTURE4);
    grassNor.Bind();
    glActiveTexture(GL_TEXTURE5);
    dirt.Bind();
    glActiveTexture(GL_TEXTURE6);
    dirtNor.Bind();
    glActiveTexture(GL_TEXTURE7);
    sand.Bind();
    glActiveTexture(GL_TEXTURE8);
    sandNor.Bind();

    glActiveTexture(GL_TEXTURE9);
    water.Bind();
}

void ChunkManager::DrawWater() {
    for (Chunk*& chunk : chunks) {
        chunk->DrawWater();
    }
}
