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

	//MESHES
	m_pSprite = new glxy::Sprite();
	m_pCube = new glxy::Cube(glm::vec3(-3.0f, 0.0f, 0.0f));
	bool loaded = m_pSoldierModel.load("Assets\\WinterSoldier\\Model\\ASOBJ.obj", false);


	//TEXTURE
	m_pSprite->LoadTexture("Assets/test.png");


	//CAMERA
	m_model = glm::mat4(1);
	m_pCamera = new Camera();


	//SHADERS
	pShapeShader = new ShaderManager("Shaders\\VertexShaderBasic.glsl", "Shaders\\FragmentShaderBasic.glsl");
	pSpriteShader = new ShaderManager("Shaders\\VertexShaderSprite.glsl", "Shaders\\FragmentShaderSprite.glsl");
	pLitShader = new ShaderManager("Shaders\\VertexShaderLit.glsl", "Shaders\\FragmentShaderLit.glsl");


	//RENDER SETTINGS
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);	//Set the colour to clear the screen to
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
	delete m_pSprite;
	delete m_pCube;

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

		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//CAMERA MOVEMENT
		m_pCamera->Update(deltaTime);

		glm::mat4 pv = m_pCamera->GetPV();

		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		//Shape drawing
		pShapeShader->UseProgram();	//Bind the shaders
		pShapeShader->SetUniform("projection_view_matrix", pv);
		pShapeShader->SetUniform("model_matrix", m_model);
		pShapeShader->SetUniform("color", color);
		m_pCube->Draw();


		//Sprite drawing
		pSpriteShader->UseProgram();	//Bind the shaders
		pSpriteShader->SetUniform("projection_view_matrix", pv);
		pSpriteShader->SetUniform("model_matrix", m_model);
		m_pSprite->Draw();


		//Object drawing
		pLitShader->UseProgram();	//Bind the shaders
		pLitShader->SetUniform("projection_view_matrix", pv);
		pLitShader->SetUniform("model_matrix", m_model);
		pLitShader->SetUniform("normal_matrix", glm::inverseTranspose(glm::mat3(m_model)));	//Probably should be done separately on object itself
		pLitShader->SetUniform("directional_light", glm::vec3(-1.0f, 0.0f, 0.0f));
		m_pSoldierModel.draw();


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
			//pCube->SetPosition((pCube->GetPosition()) + displacement * 2.0f * deltaTime);
			//glm::vec3 pos = pCube->GetPosition();
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