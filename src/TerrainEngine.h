#pragma once
#ifndef _TerrainEngine_H_
#define _TerrainEngine_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <FreeImage.h>

#include<stb/stb_image.h>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <iomanip>
#include <ctime>
#include <cmath>

#include "Shader.h"
#include "Terrain.h"
#include "Player.h"
#include "Skybox.h"
#include "Camera.h"
#include "ComputeShader.h"

namespace TerrainEngine {
	void Init();
	void Run();
	void ComputeShaderTest();
	void RenderSettings();
	void RenderPlayerInf();
	void ResetGui();

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void processInput(GLFWwindow* window);
}

#endif