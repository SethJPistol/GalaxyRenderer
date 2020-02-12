#include "glm.hpp"
#include "ext.hpp"
#include "..\glcore\gl_core_4_5.h"
#include "glfw3.h"

#include <fstream>	//For strings and other stuff
#include <sstream>

#include "Camera.h"
#include "Cube.h"
#include "Shape.h"

#define COLOR_SPEED 0.025f
#define CAMERA_SPEED 0.03f


void ShaderCompileCheck(unsigned int shaderID, const char* errorMessage)
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
void ShaderProgramLinkCheck(unsigned int programID, const char* errorMessage)
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


int main()
{
	//Checks for memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//Initialise GLFW's hooks to windows
	if (glfwInit() == false)	//If they didn't initialise properly,
		return -1;				//Return an error code

	GLFWwindow* window = glfwCreateWindow(1280, 720, "GalaxyGraphics", nullptr, nullptr);	//Create a new window to render to
	if (window == nullptr)	//If the window didn't get created successfully,
	{
		glfwTerminate();	//Terminate GLFW
		return -2;			//Return an error code
	}

	glfwMakeContextCurrent(window);

	//Remap all of OpenGL's function calls to the correct features
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}

	//Get the version of OpenGL we are running
	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);


	//MAKE MESHES
	Shape* pCube = new Shape(Shape::DrawMode::eCube);
	Shape* pCube2 = new Shape(Shape::DrawMode::eCube);
	Shape* pPolygon = new Shape(Shape::DrawMode::ePolygon);
	Shape* pCylinder = new Shape(Shape::DrawMode::eCylinder);
	pCube->SetScale(2.0f);
	pCube->SetPosition(glm::vec3(-3.2f, 0.0f, 0.0f));
	pCube2->SetPosition(glm::vec3(-6.2f, 0.0f, 0.0f));
	pPolygon->SetPosition(glm::vec3(3.0f, 0.0f, 0.0f));

	//CAMERA
	glm::mat4 model = glm::mat4(1);
	Camera* pCamera = new Camera();


	//SHADERS
	unsigned int vertexShaderID = 0;
	unsigned int fragmentShaderID = 0;
	unsigned int shaderProgramID = 0;

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

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);	//Allocate space for a vertex shader
	const char* data = shaderData.c_str();	//Convert to raw c string
	glShaderSource(vertexShaderID, 1, (const GLchar**)&data, 0);	//Send it to the shader location
	glCompileShader(vertexShaderID);	//Build the shader

	//Check it worked
	ShaderCompileCheck(vertexShaderID, "VERTEX SHADER FAILED TO COMPILE");


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

	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);	//Allocate space for a vertex shader
	data = shaderData.c_str();	//Convert to raw c string
	glShaderSource(fragmentShaderID, 1, (const GLchar**)&data, 0);	//Send it to the shader location
	glCompileShader(fragmentShaderID);	//Build the shader

	//Check it worked
	ShaderCompileCheck(fragmentShaderID, "FRAGMENT SHADER FAILED TO COMPILE");

	//SHADER PROGRAM
	//Now the IDs are validated, they can be linked
	shaderProgramID = glCreateProgram();	//Create the new shader program
	glAttachShader(shaderProgramID, vertexShaderID);	//Attach shader by ID and type
	glAttachShader(shaderProgramID, fragmentShaderID);	//Attach shader by ID and type
	glLinkProgram(shaderProgramID);	//Link the two programs

	//Check it worked
	ShaderProgramLinkCheck(shaderProgramID, "SHADER PROGRAM FAILED TO LINK");


	//COLOR
	int colorDirection = 1;
	float r = COLOR_SPEED;
	float g = COLOR_SPEED;
	float b = COLOR_SPEED;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	//Set the colour to clear the screen to
	glPolygonMode(GL_BACK, GL_LINE);	//Set to render in wireframe mode


	//DELTATIME
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;


	//GAME LOOP
	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//Clear the back buffer every frame, to avoid leftover visuals

		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		//model = glm::rotate(model, 0.016f, glm::vec3(0, 1, 0));	//Rotate the model

		//CAMERA MOVEMENT
		pCamera->Update(deltaTime);

		glm::mat4 pv = pCamera->GetPV();

		//Calculate color this frame
		if (r  > 0.0f && r < 1.0f)
			r += COLOR_SPEED * colorDirection;
		else if (b > 0.0f && b < 1.0f)
			b += COLOR_SPEED * colorDirection;
		else if (g > 0.0f && g < 1.0f)
			g += COLOR_SPEED * colorDirection;
		else
		{
			colorDirection = -colorDirection;
			r = b = g += (COLOR_SPEED * colorDirection);
		}
		glm::vec4 color = glm::vec4(r, g, b, 0.0f);

		//Use the shaders
		glUseProgram(shaderProgramID);	//Use the shader program, now it is bound
		//Set the transforms, must be done before drawing the arrays
		auto uniform_location = glGetUniformLocation(shaderProgramID, "projection_view_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(pv));
		uniform_location = glGetUniformLocation(shaderProgramID, "model_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(model));
		uniform_location = glGetUniformLocation(shaderProgramID, "color");
		glUniform4fv(uniform_location, 1, glm::value_ptr(color));


		//Draw the meshes
		pCube->Draw();
		pCube2->Draw();
		pPolygon->Draw();
		pCylinder->Draw();

		bool inputFlag = false;
		glm::vec3 displacement = glm::vec3(0);
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			displacement -= glm::vec3(0.0f, 0.0f, 1.0f);
			inputFlag = true;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			displacement += glm::vec3(0.0f, 0.0f, 1.0f);
			inputFlag = true;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			displacement -= glm::vec3(1.0f, 0.0f, 0.0f);
			inputFlag = true;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			displacement += glm::vec3(1.0f, 0.0f, 0.0f);
			inputFlag = true;
		}

		if (inputFlag)
		{
			pCube->SetPosition((pCube->GetPosition()) + displacement * 2.0f * deltaTime);
			glm::vec3 pos = pCube->GetPosition();
		}


		glfwSwapBuffers(window);	//Update the monitor display, swapping with the rendered back buffer
		glfwPollEvents();	//Polls for any events or system messages to make sure everything is handled correctly. Also updates keyboard input
	}


	//Clearing memory
	delete pCamera;
	delete pCube;
	delete pCube2;
	delete pPolygon;
	delete pCylinder;

	glfwDestroyWindow(window);
	glfwTerminate();	//Terminate GLFW

	return 0;
}