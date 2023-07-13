#include "Terrain.h"

Terrain::Terrain() {

}

Terrain::Terrain(Camera* camera, int screenWidth, int screenHeight, int terrainWidth, int terrainHeight, int chunkSize)
	: camera(camera),ScreenWidth(screenWidth),ScreenHeight(screenHeight),TerrainWidth(terrainWidth),TerrainHeight(terrainHeight),ChunkSize(chunkSize)
{
	shader=Shader("shaders/default.vert", "shaders/default.frag");
    shader.use();
    shader.setInt("Snow", 1);
    shader.setInt("SnowNor", 2);
    shader.setInt("Grass", 3);
    shader.setInt("GrassNor", 4);
    shader.setInt("Dirt", 5);
    shader.setInt("DirtNor", 6);
    shader.setInt("Sand", 7);
    shader.setInt("SandNor", 8);

    waterShader = Shader("shaders/water.vert", "shaders/water.frag");
    waterShader.use();
    waterShader.setInt("waterTexture", 9);

    chunkManager=ChunkManager(camera->Position, ChunkSize, TerrainWidth, TerrainHeight);
    chunkManager.SetupTextures();
}

void Terrain::Update(glm::vec3 pos) {
    chunkManager.Update(pos);
}

void Terrain::Resize(int terrainWidth, int terrainHeight) {
    TerrainWidth = terrainWidth;
    TerrainHeight = terrainHeight;
    chunkManager.Resize(TerrainWidth, TerrainHeight);
}

void Terrain::Generate() {
    chunkManager.Generate();
}

void Terrain::Draw(glm::mat4 proj,glm::mat4 view) {
    shader.use();
    chunkManager.BindTextures();
    shader.setVec3("viewPos", camera->Position);

    shader.setMat4("projection", proj);
    shader.setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1, 0, 0));
    shader.setMat4("model", model);
    glm::mat4 rot = glm::mat4(1.0f);

    shader.setBool("useLighting", useLighting);
    shader.setBool("useColoring", useColoring);
    shader.setBool("useTextures", useTextures);
    shader.setBool("useNormalMapping", useNormalMapping);

    shader.setVec3("dirLight", lightDir.x, lightDir.y, lightDir.z);
    shader.setFloat("shininess", 0.0f);
    shader.setBool("blinn", true);

    shader.setFloat("height", heightScale);

    shader.setFloat("snow", heightScale * snowLevel);
    shader.setFloat("grass", heightScale * grassLevel);
    shader.setFloat("water", heightScale * waterLevel);

    chunkManager.Draw(shader);

    if (drawWater) {
        waterShader.use();

        waterShader.setMat4("projection", proj);
        waterShader.setMat4("view", view);
        waterShader.setBool("useTextures", useTextures);
        waterShader.setFloat("height", heightScale*waterLevel);
        chunkManager.DrawWater();
    }
}

int Terrain::getTerrainWidth() { return TerrainWidth; }
int Terrain::getTerrainHeight() { return TerrainHeight; }