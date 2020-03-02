#include "Application.h"

#include <fstream>	//For strings and other stuff
#include <sstream>

Application::Application()
{
	//WINDOW
	m_window = nullptr;
	int result = InitialiseOGL();
	if (result != 0 || m_window == nullptr)		//If an error was encountered,
	{
		m_running = false;	//Don't run the program
		return;
	}


	//RENDER TARGETS
	int width;
	int height;
	glfwGetWindowSize(m_window, &width, &height);
	m_pRenderTarget = new glxy::RenderTarget(1, width, height);


	//MESHES
	m_pCube = new glxy::Cube(glm::vec3(-3.0f, 0.0f, 0.0f));
	m_pPoly = new glxy::Polygon(5, glm::vec3(-5.0f, 0.0f, 0.0f));
	m_pPrism = new glxy::Prism(6, 2, glm::vec3(-7.5f, 0.0f, 0.0f));
	m_pPyramid = new glxy::Pyramid(8, 2, glm::vec3(-10.0f, 0.0f, 0.0f));

	m_pSprite = new glxy::Sprite(glm::vec3(5.0f, 0.0f, 0.0f));
	m_pRTSprite = new glxy::Sprite(glm::vec3(7.0f, 0.0f, 0.0f));

	bool loaded = m_soldierModel.load("Assets\\WinterSoldier\\Model\\CharAS.obj", false, true);	//Final value true so the UVs are flipped vertically
	m_soldierModel.SetPosition(glm::vec3(0.0f, -2.0f, 0.0f));
	m_soldierModel.SetScale(0.1f);
	bool loaded2 = m_tentacleModel.load("Assets\\Tentacle\\Model\\Tentacle.obj", false, true);
	m_tentacleModel.SetPosition(glm::vec3(3.0f, -2.0f, 0.0f));
	m_tentacleModel.SetScale(0.01f);


	//TEXTURES
	m_pSprite->LoadTexture("Assets/test.png");
	m_pRTSprite->LoadTexture(m_pRenderTarget->GetTarget(0));
	m_soldierModel.LoadDiffuse("Assets\\WinterSoldier\\Textures\\Char_AS_Albedo.png");
	m_soldierModel.LoadNormal("Assets\\WinterSoldier\\Textures\\Char_AS_Normal_DirectX.PNG");
	m_tentacleModel.LoadDiffuse("Assets\\Tentacle\\Textures\\Tentacle_Albedo.png");
	m_tentacleModel.LoadNormal("Assets\\Tentacle\\Textures\\Tentacle_Normal.png");


	//MATERIALS
	m_soldierModel.m_materials[0].ambient = glm::vec3(0.5f, 0.5f, 0.5f);
	m_soldierModel.m_materials[0].diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	m_soldierModel.m_materials[0].specular = glm::vec3(1.0f, 1.0f, 1.0f);
	m_soldierModel.m_materials[0].specularPower = 16.0f;

	m_tentacleModel.m_materials[0].ambient = glm::vec3(0.5f, 0.5f, 0.5f);
	m_tentacleModel.m_materials[0].diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	m_tentacleModel.m_materials[0].specular = glm::vec3(1.0f, 1.0f, 1.0f);
	m_tentacleModel.m_materials[0].specularPower = 16.0f;


	//CAMERA
	m_model = glm::mat4(1);
	m_pCamera = new glxy::Camera();


	//SHADERS
	pShapeShader = new glxy::ShaderProgram("Shaders\\VertexShaderBasic.glsl", "Shaders\\FragmentShaderBasic.glsl");
	pSpriteShader = new glxy::ShaderProgram("Shaders\\VertexShaderSprite.glsl", "Shaders\\FragmentShaderSprite.glsl");
	pLitShader = new glxy::ShaderProgram("Shaders\\VertexShaderLit.glsl", "Shaders\\FragmentShaderLit.glsl");


	//LIGHTING
	m_directionalLights.push_back(glxy::DirectionalLight());
	m_directionalLights[0].direction = glm::vec3(-1.0f, 0.0f, 0.0f);
	m_directionalLights[0].ambient = glm::vec3(0.0f, 0.2f, 0.0f);
	m_directionalLights[0].diffuse = glm::vec3(0.5f, 1.0f, 0.5f);
	m_directionalLights[0].specular = glm::vec3(0.2f, 0.7f, 0.2f);
	m_directionalLights.push_back(glxy::DirectionalLight());
	m_directionalLights[1].direction = glm::vec3(-1.0f, 0.0f, 0.0f);
	m_directionalLights[1].ambient = glm::vec3(0.2f, 0.0f, 0.0f);
	m_directionalLights[1].diffuse = glm::vec3(1.0f, 0.7f, 0.7f);
	m_directionalLights[1].specular = glm::vec3(0.7f, 0.2f, 0.2f);


	//RENDER SETTINGS
	glClearColor(0.9f, 0.9f, 0.9f, 0.0f);	//Set the colour to clear the screen to
	glPolygonMode(GL_BACK, GL_LINE);		//Set to render back faces in wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//Set to render all faces in wireframe mode
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_running = true;
}

Application::~Application()
{
	delete pShapeShader;
	delete pSpriteShader;
	delete pLitShader;

	delete m_pCamera;
	delete m_pCube;
	delete m_pPoly;
	delete m_pPrism;
	delete m_pPyramid;

	delete m_pSprite;
	delete m_pRTSprite;

	delete m_pRenderTarget;

	glfwDestroyWindow(m_window);
	glfwTerminate();	//Terminate GLFW
}

void Application::Run()
{
	//Game loop
	while (m_running)
	{
		if (!(glfwWindowShouldClose(m_window) == false && glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS))
			m_running = false;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//Clear the back buffer every frame, to avoid leftover visuals

		//Update time
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		totalTime += deltaTime;

		//Update the camera
		m_pCamera->Update(deltaTime);
		glm::mat4 pv = m_pCamera->GetPV();


		//Shape drawing
		glm::vec4 color = glm::vec4(0.3f, 0.3f, 0.85f, 1.0f);
		pShapeShader->UseProgram();	//Bind the shaders
		pShapeShader->SetUniform("projection_view_matrix", pv);
		pShapeShader->SetUniform("color", color);
		m_pCube->Draw();
		m_pPoly->Draw();
		m_pPrism->Draw();
		m_pPyramid->Draw();


		//Sprite drawing
		pSpriteShader->UseProgram();	//Bind the shaders
		pSpriteShader->SetUniform("projection_view_matrix", pv);
		m_pSprite->Draw();


		m_pRenderTarget->Bind();	//Render the following things to the render target
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//Clear the render target from last frame

		//Object drawing
		pLitShader->UseProgram();	//Bind the shaders
		pLitShader->SetUniform("projection_view_matrix", pv);
		pLitShader->SetUniform("model_matrix", m_model);
		pLitShader->SetUniform("normal_matrix", glm::inverseTranspose(glm::mat3(m_model)));	//Probably should be done separately on object itself
		pLitShader->SetUniform("camera_position", m_pCamera->GetPosition());
		m_directionalLights[0].direction = glm::vec3(glm::cos(totalTime), 0.0f, -glm::sin(totalTime));	//Make the first light spin

		for (int i = 0; i < m_directionalLights.size(); ++i)	//For all the directional lights in the scene,
			m_directionalLights[i].Update(i);					//Set the uniform values for the light

		m_soldierModel.draw();
		m_tentacleModel.draw();

		m_pRenderTarget->Unbind();	//Return to drawing to the back buffer


		//RTSprite drawing
		pSpriteShader->UseProgram();	//Bind the shaders
		pSpriteShader->SetUniform("projection_view_matrix", pv);
		m_pRTSprite->Draw();


		//Cube movement
		bool inputFlag = false;
		glm::vec3 displacement = glm::vec3(0);
		if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			displacement -= glm::vec3(0.0f, 0.0f, 1.0f);
			inputFlag = true;
		}
		if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			displacement += glm::vec3(0.0f, 0.0f, 1.0f);
			inputFlag = true;
		}
		if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			displacement -= glm::vec3(1.0f, 0.0f, 0.0f);
			inputFlag = true;
		}
		if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			displacement += glm::vec3(1.0f, 0.0f, 0.0f);
			inputFlag = true;
		}

		if (inputFlag)
		{
			m_pCube->SetPosition((m_pCube->GetPosition()) + displacement * 2.0f * deltaTime);
			glm::vec3 pos = m_pCube->GetPosition();
		}


		glfwSwapBuffers(m_window);	//Update the monitor display, swapping with the rendered back buffer
		glfwPollEvents();	//Polls for any events or system messages to make sure everything is handled correctly. Also updates keyboard input
	}
}


int Application::InitialiseOGL()
{
	//Initialise GLFW's hooks to windows
	if (glfwInit() == false)	//If they didn't initialise properly,
	{
		printf("Error - Initialising GLFW's hooks to Windows failed\n");
		return -1;				//Return an error code
	}

	m_window = glfwCreateWindow(1280, 720, "GalaxyGraphics", nullptr, nullptr);	//Create a new window to render to
	if (m_window == nullptr)	//If the window didn't get created successfully,
	{
		glfwTerminate();	//Terminate GLFW
		printf("Error - Window creation failed\n");
		return -2;			//Return an error code
	}

	glfwMakeContextCurrent(m_window);

	//Remap all of OpenGL's function calls to the correct features
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		printf("Error - Loading OGL functions failed\n");
		return -3;
	}

	//Get the version of OpenGL we are running
	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL Version: %i.%i\n", major, minor);

	return 0;	//No error encountered
}