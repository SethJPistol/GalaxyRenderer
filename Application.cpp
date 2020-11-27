#include "Application.h"

#include "stb_image.h"

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


	//MESHES
	//Shapes
	m_pCube = new glxy::Cube(glm::vec3(-3.0f, 0.0f, 0.0f));
	m_pPoly = new glxy::Polygon(5, glm::vec3(-5.0f, -0.5f, 0.0f));
	m_pPrism = new glxy::Prism(6, 2, glm::vec3(-7.5f, 0.5f, 0.0f));
	m_pPyramid = new glxy::Pyramid(8, 2, glm::vec3(-10.0f, 0.5f, 0.0f));

	//Physics shapes
	m_pCircleLeft = new glxy::Circle(glm::vec3(-3.0f, 0.0f, 0.0f), 1.0f, glm::vec2(1.5f, 0.0f), 1.0f);
	m_pCircleRight = new glxy::Circle(glm::vec3(3.0f, 0.0f, 0.0f), 1.0f, glm::vec2(-1.6f, 0.0f), 1.0f);
	m_pCircleHeavy = new glxy::Circle(glm::vec3(0.0f, 6.0f, 0.0f), 2.0f, glm::vec2(0.0f, 0.0f), 3.0f);
	m_pBoxLeft = new glxy::Quad(glm::vec3(-3.0f, -3.0f, 0.0f), 1.0f, glm::vec2(1.6f, 0.0f));
	m_pBoxRight = new glxy::Quad(glm::vec3(3.0f, -3.0f, 0.0f), 1.0f, glm::vec2(-1.5f, 0.0f));
	
	m_pPlaneLeft = new glxy::Plane(glm::vec2(1.0f, 0.0f), -8.0f);
	m_pPlaneLeftDiagonal = new glxy::Plane(glm::vec2(1.0f, 1.0f), -8.0f);
	m_pPlaneGround = new glxy::Plane(glm::vec2(0.0f, 1.0f), -8.0f);
	m_pPlaneRightDiagonal = new glxy::Plane(glm::vec2(-1.0f, 1.0f), -8.0f);
	m_pPlaneRight = new glxy::Plane(glm::vec2(-1.0f, 0.0f), -8.0f);
	//m_pPlaneLeft->SetElasticity(0.0f);
	//m_pPlaneLeftDiagonal->SetElasticity(0.0f);
	//m_pPlaneGround->SetElasticity(0.0f);
	//m_pPlaneRightDiagonal->SetElasticity(0.0f);
	//m_pPlaneRight->SetElasticity(0.0f);

	//Sprites
	m_pSprite = new glxy::Sprite(glm::vec3(17.0f, -6.5f, 0.0f));

	//Models
	bool loaded = m_soldierModel.load("Assets\\WinterSoldier\\Model\\CharAS.obj");
	m_soldierModel.SetPosition(glm::vec3(12.0f, -7.0f, 0.0f));
	m_soldierModel.SetScale(0.1f);
	bool loaded2 = m_tentacleModel.load("Assets\\Tentacle\\Model\\Tentacle.obj");
	m_tentacleModel.SetPosition(glm::vec3(15.0f, -7.0f, 0.0f));
	m_tentacleModel.SetScale(0.01f);


	//TEXTURES
	stbi_set_flip_vertically_on_load(true);	//Make sure that all the textures' UVs are flipped vertically to work with OGL
	m_pSprite->LoadTexture("Assets/test.png");
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
	m_pCamera = new glxy::Camera(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0));


	//SHADERS
	pShapeShader = new glxy::ShaderProgram("Shaders\\VertexShaderBasic.glsl", "Shaders\\FragmentShaderBasic.glsl");
	pSpriteShader = new glxy::ShaderProgram("Shaders\\VertexShaderSprite.glsl", "Shaders\\FragmentShaderSprite.glsl");
	pLitShader = new glxy::ShaderProgram("Shaders\\VertexShaderLit.glsl", "Shaders\\FragmentShaderLit.glsl");
	if (m_usePostProcessing)
		pPostProcShader = new glxy::ShaderProgram("Shaders\\VertexShaderPostProc.glsl", "Shaders\\FragmentShaderPostProc.glsl");


	//LIGHTING
	//When adding/subtracting lights, change the #define in the lit fragment shader too
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
	m_directionalLights.push_back(glxy::DirectionalLight());
	m_directionalLights[2].direction = glm::vec3(1.0f, 0.0f, 0.0f);
	m_directionalLights[2].ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	m_directionalLights[2].diffuse = glm::vec3(0.6f, 0.6f, 0.9f);
	m_directionalLights[2].specular = glm::vec3(0.2f, 0.2f, 0.2f);


	//POST-PROCESSING
	if (m_usePostProcessing)
	{
		int width;
		int height;
		glfwGetWindowSize(m_window, &width, &height);
		m_pRenderTarget = new glxy::RenderTarget(1, width, height);

		m_pScreen = new glxy::ScreenMesh(m_pRenderTarget->GetTarget(0));
	}


	//RENDER SETTINGS
	glClearColor(0.9f, 0.9f, 0.9f, 0.0f);	//Set the colour to clear the screen to
	glPolygonMode(GL_BACK, GL_LINE);		//Set to render back faces in wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//Set to render all faces in wireframe mode
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//PHYSICS
	m_pPhysicsScene = new PhysicsScene();
	m_pPhysicsScene->SetTimeStep(0.01f);
	m_pPhysicsScene->SetGravity(glm::vec2(0.0f, -9.87f));

	m_pPhysicsScene->AddObject(m_pCircleLeft);
	m_pPhysicsScene->AddObject(m_pCircleRight);
	m_pPhysicsScene->AddObject(m_pCircleHeavy);
	m_pPhysicsScene->AddObject(m_pBoxLeft);
	m_pPhysicsScene->AddObject(m_pBoxRight);
	m_pPhysicsScene->AddObject(m_pPlaneLeft);
	m_pPhysicsScene->AddObject(m_pPlaneLeftDiagonal);
	m_pPhysicsScene->AddObject(m_pPlaneGround);
	m_pPhysicsScene->AddObject(m_pPlaneRightDiagonal);
	m_pPhysicsScene->AddObject(m_pPlaneRight);

	m_running = true;
}

Application::~Application()
{
	delete m_pPhysicsScene;
	m_pPhysicsScene = nullptr;

	delete pShapeShader;
	pShapeShader = nullptr;
	delete pSpriteShader;
	pSpriteShader = nullptr;
	delete pLitShader;
	pLitShader = nullptr;
	delete pPostProcShader;
	pPostProcShader = nullptr;

	delete m_pCamera;
	m_pCamera = nullptr;

	delete m_pCube;
	m_pCube = nullptr;
	delete m_pPoly;
	m_pPoly = nullptr;
	delete m_pPrism;
	m_pPrism = nullptr;
	delete m_pPyramid;
	m_pPyramid = nullptr;

	delete m_pSprite;
	m_pSprite = nullptr;

	delete m_pRenderTarget;
	m_pRenderTarget = nullptr;
	delete m_pScreen;
	m_pScreen = nullptr;


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

		//Check to turn on physics
		if (glfwGetKey(m_window, GLFW_KEY_T) == GLFW_PRESS && !m_physicsRunning)
			m_physicsRunning = true;

		//Update physics
		if (m_physicsRunning)
			m_pPhysicsScene->Update(deltaTime);

		//Update the camera
		m_pCamera->Update(deltaTime);
		glm::mat4 pv = m_pCamera->GetPV();


		if (m_usePostProcessing)
		{
			m_pRenderTarget->Bind();	//Render the following things to the render target
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//Clear the render target from last frame
		}


		//Shape drawing
		pShapeShader->UseProgram();	//Bind the shaders
		pShapeShader->SetUniform("projection_view_matrix", pv);

		glm::vec4 colorMidnightBlue = glm::vec4(11, 27, 62, 255) / 255.0f;
		glm::vec4 colorDarkSlateGrey = glm::vec4(41, 101, 111, 255) / 255.0f;
		glm::vec4 colorDarkSeaGreen = glm::vec4(156, 201, 156, 255) / 255.0f;
		glm::vec4 colorDarkKhaki = glm::vec4(178, 205, 99, 255) / 255.0f;
		glm::vec4 colorKhaki = glm::vec4(206, 220, 153, 255) / 255.0f;

		pShapeShader->SetUniform("color", colorDarkSlateGrey);
		m_pCircleLeft->Draw();
		m_pCircleRight->Draw();

		pShapeShader->SetUniform("color", colorDarkSeaGreen);
		m_pCircleHeavy->Draw();

		pShapeShader->SetUniform("color", colorDarkKhaki);
		m_pBoxLeft->Draw();
		m_pBoxRight->Draw();

		pShapeShader->SetUniform("color", colorMidnightBlue);
		m_pPlaneLeft->Draw();
		m_pPlaneLeftDiagonal->Draw();
		m_pPlaneGround->Draw();
		m_pPlaneRightDiagonal->Draw();
		m_pPlaneRight->Draw();


		//Sprite drawing
		pSpriteShader->UseProgram();	//Bind the shaders
		pSpriteShader->SetUniform("projection_view_matrix", pv);
		//m_pSprite->Draw();


		//Object drawing
		pLitShader->UseProgram();	//Bind the shaders
		pLitShader->SetUniform("projection_view_matrix", pv);
		pLitShader->SetUniform("model_matrix", m_model);
		pLitShader->SetUniform("normal_matrix", glm::inverseTranspose(glm::mat3(m_model)));	//Probably should be done separately on object itself
		pLitShader->SetUniform("camera_position", m_pCamera->GetPosition());

		m_directionalLights[0].direction = glm::vec3(glm::cos(totalTime), 0.0f, -glm::sin(totalTime));	//Make the first light spin

		for (int i = 0; i < m_directionalLights.size(); ++i)	//For all the directional lights in the scene,
			m_directionalLights[i].Update(i);					//Set the uniform values for the light

		//Lighting doesn't work for these, since they don't have normals yet
		//m_pCube->Draw();
		//m_pPoly->Draw();
		//m_pPrism->Draw();
		//m_pPyramid->Draw();

		m_soldierModel.draw();
		m_tentacleModel.draw();


		if (m_usePostProcessing)
		{
			m_pRenderTarget->Unbind();	//Return to drawing to the back buffer

			//Let the user change the effect during runtime
			if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS && !m_EKeyHeld)
			{
				++m_effectToUse;
				if (m_effectToUse == PP_EFFECT_COUNT)
					m_effectToUse = 0;
				m_EKeyHeld = true;
			}
			else if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_RELEASE && m_EKeyHeld)
				m_EKeyHeld = false;

			//Draw the render target to the back buffer now
			pPostProcShader->UseProgram();
			pPostProcShader->SetUniform("effect_to_use", m_effectToUse);
			pPostProcShader->SetUniform("chromatic_aberration_amount", 1.0f);
			m_pScreen->Draw();
		}


		//Circle movement
		bool inputFlag = false;
		glm::vec3 direction = glm::vec3(0);
		if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			direction += glm::vec3(0.0f, 1.0f, 0.0f);
			inputFlag = true;
		}
		if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			direction -= glm::vec3(0.0f, 1.0f, 0.0f);
			inputFlag = true;
		}
		if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			direction -= glm::vec3(1.0f, 0.0f, 0.0f);
			inputFlag = true;
		}
		if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			direction += glm::vec3(1.0f, 0.0f, 0.0f);
			inputFlag = true;
		}
		if (inputFlag)
			m_pCircleHeavy->ApplyForce(direction);


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