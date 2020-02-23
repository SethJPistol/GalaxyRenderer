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
	//glxy::Cube* pCube2 = new glxy::Cube(glm::vec3(-3.0f, 0.0f, 0.0f), 2.0f);
	//glxy::Polygon* pPoly = new glxy::Polygon(7, glm::vec3(3.0f, 0.0f, 0.0f));
	//glxy::Prism* pPrism = new glxy::Prism(3, 2.0f, glm::vec3(5.0f, 0.0f, 0.0f));
	//glxy::Pyramid* pPyramid = new glxy::Pyramid();
	//aie::OBJMesh soldierModel;
	//bool loaded = soldierModel.load("Assets\\WinterSoldier\\Model\\ASOBJ.obj", false);


	//TEXTURE
	m_pQuad->LoadTexture("Assets/test.png");


	//CAMERA
	m_model = glm::mat4(1);
	m_pCamera = new Camera();


	//SHADERS
	m_vertexShaderID = 0;
	m_fragmentShaderID = 0;
	m_shaderProgramID = 0;

	//VERTEX SHADER
	//Load shader into string
	std::string shaderData;
	std::ifstream inFileStream("Shaders\\simple_vertex_shader.glsl", std::ifstream::in);

	//Load source into string for compilation
	std::stringstream stringStream;
	if (inFileStream.is_open())
	{
		stringStream << inFileStream.rdbuf();
		shaderData = stringStream.str();
		inFileStream.close();
	}

	m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);	//Allocate space for a vertex shader
	const char* data = shaderData.c_str();	//Convert to raw c string
	glShaderSource(m_vertexShaderID, 1, (const GLchar**)&data, 0);	//Send it to the shader location
	glCompileShader(m_vertexShaderID);	//Build the shader

	//Check it worked
	ShaderCompileCheck(m_vertexShaderID, "Vertex shader failed to compile");


	//FRAGMENT SHADER
	//Load shader into string
	std::ifstream inFileStreamFrag("Shaders\\simple_fragment_shader.glsl", std::ifstream::in);

	//Load source into string for compilation
	std::stringstream stringStream_frag;
	if (inFileStreamFrag.is_open())
	{
		stringStream_frag << inFileStreamFrag.rdbuf();
		shaderData = stringStream_frag.str();
		inFileStreamFrag.close();
	}

	m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);	//Allocate space for a vertex shader
	data = shaderData.c_str();	//Convert to raw c string
	glShaderSource(m_fragmentShaderID, 1, (const GLchar**)&data, 0);	//Send it to the shader location
	glCompileShader(m_fragmentShaderID);	//Build the shader

	//Check it worked
	ShaderCompileCheck(m_fragmentShaderID, "Fragment shader failed to compile");

	//SHADER PROGRAM
	//Now the IDs are validated, they can be linked
	m_shaderProgramID = glCreateProgram();	//Create the new shader program
	glAttachShader(m_shaderProgramID, m_vertexShaderID);	//Attach shader by ID and type
	glAttachShader(m_shaderProgramID, m_fragmentShaderID);	//Attach shader by ID and type
	glLinkProgram(m_shaderProgramID);	//Link the two programs

	//Check it worked
	ShaderProgramLinkCheck(m_shaderProgramID, "Shader program failed to link");


	//COLOR
	/*int colorDirection = 1;
	float r = COLOR_SPEED;
	float g = COLOR_SPEED;
	float b = COLOR_SPEED;*/

	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);	//Set the colour to clear the screen to
	glPolygonMode(GL_BACK, GL_LINE);	//Set to render in wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//Set to render in wireframe mode

	m_running = true;
}

Application::~Application()
{
	delete m_pCamera;
	delete m_pQuad;
	delete m_pCube;
	//delete pCube2;
	//delete pPoly;
	//delete pPrism;
	//delete pPyramid;

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

		//model = glm::rotate(model, 0.016f, glm::vec3(0, 1, 0));	//Rotate the model

		//CAMERA MOVEMENT
		m_pCamera->Update(deltaTime);

		glm::mat4 pv = m_pCamera->GetPV();

		////Calculate color this frame
		//if (r > 0.0f && r < 1.0f)
		//	r += COLOR_SPEED * colorDirection;
		//else if (b > 0.0f && b < 1.0f)
		//	b += COLOR_SPEED * colorDirection;
		//else if (g > 0.0f && g < 1.0f)
		//	g += COLOR_SPEED * colorDirection;
		//else
		//{
		//	colorDirection = -colorDirection;
		//	r = b = g += (COLOR_SPEED * colorDirection);
		//}
		////glm::vec4 color = glm::vec4(r, g, b, 0.0f);
		//glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);

		//Use the shaders
		glUseProgram(m_shaderProgramID);	//Use the shader program, now it is bound
		//Set the transforms, must be done before drawing the arrays
		auto uniform_location = glGetUniformLocation(m_shaderProgramID, "projection_view_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(pv));
		uniform_location = glGetUniformLocation(m_shaderProgramID, "model_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(m_model));
		//uniform_location = glGetUniformLocation(m_shaderProgramID, "color");
		//glUniform4fv(uniform_location, 1, glm::value_ptr(color));


		//Draw the meshes
		m_pQuad->Draw();
		m_pCube->Draw();
		//pCube2->Draw();
		//pPoly->Draw();
		//pPrism->Draw();
		//pPyramid->Draw();
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

void Application::ShaderCompileCheck(unsigned int shaderID, const char* errorMessage)
{
	//Check it worked
	GLint success = GL_FALSE;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		//Get the length of the OpenGL error message
		GLint logLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		//Create the error buffer
		char* log = new char[logLength];
		//Copy the error from the buffer
		glGetShaderInfoLog(shaderID, logLength, 0, log);

		//Create the error message
		std::string full_errorMessage(log);
		full_errorMessage += errorMessage;
		full_errorMessage += "\n";
		printf(full_errorMessage.c_str());
		//Clean up anyways
		delete[] log;
	}
}
void Application::ShaderProgramLinkCheck(unsigned int programID, const char* errorMessage)
{
	GLint success = GL_FALSE;
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		//Get the length of the OpenGL error message
		GLint logLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
		//Create the error buffer
		char* log = new char[logLength];
		//Copy the error from the buffer
		glGetProgramInfoLog(programID, logLength, 0, log);

		//Create the error message
		std::string full_errorMessage(log);
		full_errorMessage += errorMessage;
		full_errorMessage += "\n";
		printf(full_errorMessage.c_str());
		//Clean up anyways
		delete[] log;
	}
}