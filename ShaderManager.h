#pragma once

#include "glm.hpp"
#include "ext.hpp"
#include "..\glcore\gl_core_4_5.h"

class ShaderManager
{
public:

	ShaderManager(const char* vertShaderPath, const char* fragShaderPath);
	~ShaderManager();

	void UseProgram();
	void SetUniform(const char* uniformName, const GLfloat* value);

private:

	void ShaderCompileCheck(unsigned int shaderID, const char* errorMessage);
	void ShaderProgramLinkCheck(unsigned int programID, const char* errorMessage);

	unsigned int m_vertexShaderID;
	unsigned int m_fragmentShaderID;
	unsigned int m_shaderProgramID;

};