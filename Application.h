#pragma once
#include "glm.hpp"
#include "ext.hpp"
#include "..\glcore\gl_core_4_5.h"
#include "glfw3.h"

#include "Camera.h"
#include "Shape.h"
#include "Sprite.h"
#include "ShaderProgram.h"
#include "Dependencies\OBJMesh.h"
#include "Light.h"

class Application
{
public:

	Application();
	~Application();

	void Run();

	glxy::ShaderProgram* pShapeShader = nullptr;
	glxy::ShaderProgram* pSpriteShader = nullptr;
	glxy::ShaderProgram* pLitShader = nullptr;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float totalTime = 0.0f;

private:

	int InitialiseOGL();

	bool m_running;
	GLFWwindow* m_window;

	glm::mat4 m_model;
	glxy::Camera* m_pCamera;
	glxy::Sprite* m_pSprite;
	glxy::Cube* m_pCube;
	aie::OBJMesh m_soldierModel;

	std::vector<glxy::DirectionalLight> m_directionalLights;

};