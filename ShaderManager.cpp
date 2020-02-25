#include "ShaderManager.h"

#include "..\glcore\gl_core_4_5.h"

#include <fstream>	//For strings and other stuff
#include <sstream>

using namespace glxy;

ShaderManager::ShaderManager(const char* vertShaderPath, const char* fragShaderPath)
{
	m_vertexShaderID = 0;
	m_fragmentShaderID = 0;
	m_shaderProgramID = 0;

	//Load shader into string
	std::string shaderData;
	std::ifstream inFileStream(vertShaderPath, std::ifstream::in);

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
	ShaderCompileCheck(m_vertexShaderID, "Error - Vertex shader failed to compile");


	//FRAGMENT SHADER
	//Load shader into string
	std::ifstream inFileStreamFrag(fragShaderPath, std::ifstream::in);

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
	ShaderCompileCheck(m_fragmentShaderID, "Error - Fragment shader failed to compile");

	//SHADER PROGRAM
	//Now the IDs are validated, they can be linked
	m_shaderProgramID = glCreateProgram();	//Create the new shader program
	glAttachShader(m_shaderProgramID, m_vertexShaderID);	//Attach shader by ID and type
	glAttachShader(m_shaderProgramID, m_fragmentShaderID);	//Attach shader by ID and type
	glLinkProgram(m_shaderProgramID);	//Link the two programs

	//Check it worked
	ShaderProgramLinkCheck(m_shaderProgramID, "Error -Shader program failed to link");
}

ShaderManager::~ShaderManager()
{

}

void ShaderManager::UseProgram()
{
	glUseProgram(m_shaderProgramID);	//Use the shader program, now it is bound
}

void ShaderManager::SetUniform(const char* uniformName, glm::mat4 value)
{
	auto uniformLocation = glGetUniformLocation(m_shaderProgramID, uniformName);
	glUniformMatrix4fv(uniformLocation, 1, false, glm::value_ptr(value));
}
void ShaderManager::SetUniform(const char* uniformName, glm::mat3 value)
{
	auto uniformLocation = glGetUniformLocation(m_shaderProgramID, uniformName);
	glUniformMatrix3fv(uniformLocation, 1, false, glm::value_ptr(value));
}
void ShaderManager::SetUniform(const char* uniformName, glm::vec4 value)
{
	auto uniformLocation = glGetUniformLocation(m_shaderProgramID, uniformName);
	glUniform4fv(uniformLocation, 1, glm::value_ptr(value));
}
void ShaderManager::SetUniform(const char* uniformName, glm::vec3 value)
{
	auto uniformLocation = glGetUniformLocation(m_shaderProgramID, uniformName);
	glUniform3fv(uniformLocation, 1, glm::value_ptr(value));
}

void ShaderManager::ShaderCompileCheck(unsigned int shaderID, const char* errorMessage)
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
void ShaderManager::ShaderProgramLinkCheck(unsigned int programID, const char* errorMessage)
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