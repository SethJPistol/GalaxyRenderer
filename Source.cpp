#include "glm.hpp"
#include "ext.hpp"
#include "..\glcore\gl_core_4_5.h"
#include "glfw3.h"

#include <fstream>	//For strings and other stuff
#include <sstream>

#include "Cube.h"

#define COLOR_SPEED 0.025f


void ShaderCompileCheck(unsigned int shader_ID, const char* error_message)
{
	//Check it worked
	GLint success = GL_FALSE;
	glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		//Get the length of the OpenGL error message
		GLint log_length = 0;
		glGetShaderiv(shader_ID, GL_INFO_LOG_LENGTH, &log_length);
		//Create the error buffer
		char* log = new char[log_length];
		//Copy the error from the buffer
		glGetShaderInfoLog(shader_ID, log_length, 0, log);

		//Create the error message
		std::string full_error_message(log);
		full_error_message += error_message;
		full_error_message += "\n";
		printf(full_error_message.c_str());
		//Clean up anyways
		delete[] log;
	}
}
void ShaderProgramLinkCheck(unsigned int program_ID, const char* error_message)
{
	GLint success = GL_FALSE;
	glGetProgramiv(program_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		//Get the length of the OpenGL error message
		GLint log_length = 0;
		glGetProgramiv(program_ID, GL_INFO_LOG_LENGTH, &log_length);
		//Create the error buffer
		char* log = new char[log_length];
		//Copy the error from the buffer
		glGetProgramInfoLog(program_ID, log_length, 0, log);

		//Create the error message
		std::string full_error_message(log);
		full_error_message += error_message;
		full_error_message += "\n";
		printf(full_error_message.c_str());
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
	Cube* pCube = new Cube();


	//CAMERA
	//The projection matrix, representing the lens of the camera, created with FOV, screen aspect ratio, and near and far of the frustum
	glm::mat4 projection = glm::perspective(1.507f, 16.0f / 9.0f, 0.1f, 5.0f);
	//View matrix, the inverse of the camera's world transform
	glm::mat4 view = glm::lookAt(glm::vec3(0, 1, 2), glm::vec3(0), glm::vec3(0, 1, 0));
	//The model's local transform, set to the identity matrix
	glm::mat4 model = glm::mat4(1);


	//SHADERS
	unsigned int vertex_shader_ID = 0;
	unsigned int fragment_shader_ID = 0;
	unsigned int shader_program_ID = 0;

	//VERTEX SHADER
	//Load shader into string
	std::string shader_data;
	std::ifstream in_file_stream("Shaders\\simple_vertex_shader.glsl", std::ifstream::in);

	//Load source into string for compilation
	std::stringstream string_stream;
	if (in_file_stream.is_open())
	{
		string_stream << in_file_stream.rdbuf();
		shader_data = string_stream.str();
		in_file_stream.close();
	}

	vertex_shader_ID = glCreateShader(GL_VERTEX_SHADER);	//Allocate space for a vertex shader
	const char* data = shader_data.c_str();	//Convert to raw c string
	glShaderSource(vertex_shader_ID, 1, (const GLchar**)&data, 0);	//Send it to the shader location
	glCompileShader(vertex_shader_ID);	//Build the shader

	//Check it worked
	ShaderCompileCheck(vertex_shader_ID, "VERTEX SHADER FAILED TO COMPILE");


	//FRAGMENT SHADER
	//Load shader into string
	std::ifstream in_file_stream_frag("Shaders\\simple_fragment_shader.glsl", std::ifstream::in);
	
	//Load source into string for compilation
	std::stringstream string_stream_frag;
	if (in_file_stream_frag.is_open())
	{
		string_stream_frag << in_file_stream_frag.rdbuf();
		shader_data = string_stream_frag.str();
		in_file_stream_frag.close();
	}

	fragment_shader_ID = glCreateShader(GL_FRAGMENT_SHADER);	//Allocate space for a vertex shader
	data = shader_data.c_str();	//Convert to raw c string
	glShaderSource(fragment_shader_ID, 1, (const GLchar**)&data, 0);	//Send it to the shader location
	glCompileShader(fragment_shader_ID);	//Build the shader

	//Check it worked
	ShaderCompileCheck(fragment_shader_ID, "FRAGMENT SHADER FAILED TO COMPILE");

	//SHADER PROGRAM
	//Now the IDs are validated, they can be linked
	shader_program_ID = glCreateProgram();	//Create the new shader program
	glAttachShader(shader_program_ID, vertex_shader_ID);	//Attach shader by ID and type
	glAttachShader(shader_program_ID, fragment_shader_ID);	//Attach shader by ID and type
	glLinkProgram(shader_program_ID);	//Link the two programs

	//Check it worked
	ShaderProgramLinkCheck(shader_program_ID, "SHADER PROGRAM FAILED TO LINK");


	//COLOR
	int color_direction = 1;
	float r = COLOR_SPEED;
	float g = COLOR_SPEED;
	float b = COLOR_SPEED;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	//Set the colour to clear the screen to
	glPolygonMode(GL_BACK, GL_LINE);	//Set to render in wireframe mode


	//GAME LOOP
	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//Clear the back buffer every frame, to avoid leftover visuals

		model = glm::rotate(model, 0.016f, glm::vec3(0, 1, 0));	//Rotate the model

		glm::mat4 pv = projection * view;

		//Calculate color this frame
		if (r  > 0.0f && r < 1.0f)
			r += COLOR_SPEED * color_direction;
		else if (b > 0.0f && b < 1.0f)
			b += COLOR_SPEED * color_direction;
		else if (g > 0.0f && g < 1.0f)
			g += COLOR_SPEED * color_direction;
		else
		{
			color_direction = -color_direction;
			r = b = g += (COLOR_SPEED * color_direction);
		}
		glm::vec4 color = glm::vec4(r, g, b, 0.0f);

		//Use the shaders
		glUseProgram(shader_program_ID);	//Use the shader program, now it is bound
		//Set the transforms, must be done before drawing the arrays
		auto uniform_location = glGetUniformLocation(shader_program_ID, "projection_view_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(pv));
		uniform_location = glGetUniformLocation(shader_program_ID, "model_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(model));
		uniform_location = glGetUniformLocation(shader_program_ID, "color");
		glUniform4fv(uniform_location, 1, glm::value_ptr(color));


		//Draw the meshes
		pCube->Draw();


		glfwSwapBuffers(window);	//Update the monitor display, swapping with the rendered back buffer
		glfwPollEvents();	//Polls for any events or system messages to make sure everything is handled correctly. Also updates keyboard input
	}


	//Clearing memory
	delete pCube;

	glfwDestroyWindow(window);
	glfwTerminate();	//Terminate GLFW

	return 0;
}