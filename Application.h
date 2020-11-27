#pragma once
#include "glm.hpp"
#include "ext.hpp"
#include "..\glcore\gl_core_4_5.h"
#include "glfw3.h"

#include "PhysicsScene.h"
#include "Camera.h"
#include "Shape.h"
#include "Sprite.h"
#include "Plane.h"
#include "Dependencies\OBJMesh.h"
#include "Light.h"
#include "ShaderProgram.h"
#include "RenderTarget.h"
#include "ScreenMesh.h"

#define PP_EFFECT_COUNT 5

class Application
{
public:

	Application();
	~Application();

	void Run();

	glxy::ShaderProgram* pShapeShader = nullptr;
	glxy::ShaderProgram* pSpriteShader = nullptr;
	glxy::ShaderProgram* pLitShader = nullptr;
	glxy::ShaderProgram* pPostProcShader = nullptr;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float totalTime = 0.0f;

private:

	int InitialiseOGL();

	bool m_running;
	GLFWwindow* m_window;

	//Physics
	PhysicsScene* m_pPhysicsScene = nullptr;
	bool m_physicsRunning = false;

	//Post-processing
	glxy::RenderTarget* m_pRenderTarget = nullptr;
	glxy::ScreenMesh* m_pScreen = nullptr;
	bool m_usePostProcessing = true;
	int m_effectToUse = 0;
	bool m_EKeyHeld = false;

	glm::mat4 m_model;
	glxy::Camera* m_pCamera;

	//Shapes
	glxy::Cube* m_pCube;
	glxy::Polygon* m_pPoly;
	glxy::Prism* m_pPrism;
	glxy::Pyramid* m_pPyramid;

	//Physics shapes
	glxy::Circle* m_pCircleLeft;
	glxy::Circle* m_pCircleRight;
	glxy::Circle* m_pCircleHeavy;
	glxy::Quad* m_pBoxLeft;
	glxy::Quad* m_pBoxRight;
	glxy::Plane* m_pPlaneLeft;
	glxy::Plane* m_pPlaneLeftDiagonal;
	glxy::Plane* m_pPlaneGround;
	glxy::Plane* m_pPlaneRightDiagonal;
	glxy::Plane* m_pPlaneRight;

	//Sprites
	glxy::Sprite* m_pSprite;

	//Models
	aie::OBJMesh m_soldierModel;
	aie::OBJMesh m_tentacleModel;

	//Lights
	std::vector<glxy::DirectionalLight> m_directionalLights;
};