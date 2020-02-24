#include "Application.h"

#include <fstream>	//For strings and other stuff
#include <sstream>

#define COLOR_SPEED 0.025f

Application::Application()
{
	m_window = nullptr;
	int result = InitialiseOGL();
	if (result != 0 || m_window == nullptr)		//If an error was encountered,
	{
		m_running = false;	//Don't run the program
		return;
	}

	//MAKE MESHES
	m_pQuad = new glxy::Quad();
	m_pCube = new glxy::Cube(glm::vec3(-3.0f, 0.0f, 0.0f));
	//aie::OBJMesh soldierModel;
	//bool loaded = soldierModel.load("Assets\\WinterSoldier\\Model\\ASOBJ.obj", false);


	//TEXTURE
	m_pQuad->LoadTexture("Assets/test.png");


	//CAMERA
	m_model = glm::mat4(1);
	m_pCamera = new Camera();


	//SHADERS
	pShaderMan = new ShaderManager("Shaders\\simple_vertex_shader.glsl", "Shaders\\simple_fragment_shader.glsl");


	//RENDER SETTINGS
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);	//Set the colour to clear the screen to
	glPolygonMode(GL_BACK, GL_LINE);		//Set to render in wireframe mode
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//Set to render in wireframe mode

	m_running = true;
}

Application::~Application()
{
	delete pShaderMan;

	delete m_pCamera;
	delete m_pQuad;
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

		//SHADERS
		pShaderMan->UseProgram();	//Bind the shaders

		//Set the transforms, must be done before drawing the arrays
		pShaderMan->SetUniform("projection_view_matrix", glm::value_ptr(pv));
		pShaderMan->SetUniform("model_matrix", glm::value_ptr(m_model));
		//uniform_location = glGetUniformLocation(m_shaderProgramID, "color");
		//glUniform4fv(uniform_location, 1, glm::value_ptr(color));


		//Draw the meshes
		m_pQuad->Draw();
		m_pCube->Draw();
		//soldierModel.draw();

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