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

	//Shapes
	glxy::Cube* m_pCube;
	glxy::Polygon* m_pPoly;
	glxy::Prism* m_pPrism;
	glxy::Pyramid* m_pPyramid;

	//Sprites
	glxy::Sprite* m_pSprite;

	//Models
	aie::OBJMesh m_soldierModel;
	aie::OBJMesh m_tentacleModel;

	//Lights
	std::vector<glxy::DirectionalLight> m_directionalLights;
};