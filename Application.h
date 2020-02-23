#pragma once
#include "glm.hpp"
#include "ext.hpp"
#include "..\glcore\gl_core_4_5.h"
#include "glfw3.h"

#include "Camera.h"
#include "Shape.h"
#include "Dependencies\OBJMesh.h"

class Application
{
public:

	Application();
	~Application();

	void Run();

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

private:

	int InitialiseOGL();
	void ShaderCompileCheck(unsigned int shaderID, const char* errorMessage);
	void ShaderProgramLinkCheck(unsigned int programID, const char* errorMessage);

	bool m_running;
	GLFWwindow* m_window;

	unsigned int m_vertexShaderID;
	unsigned int m_fragmentShaderID;
	unsigned int m_shaderProgramID;

	glm::mat4 m_model;
	Camera* m_pCamera;
	glxy::Quad* m_pQuad;
	glxy::Cube* m_pCube;

};