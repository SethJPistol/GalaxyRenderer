#pragma once

#include "glm.hpp"
#include "ext.hpp"
#include "..\glcore\gl_core_4_5.h"

namespace glxy
{
	class ShaderProgram
	{
	public:

		ShaderProgram(const char* vertShaderPath, const char* fragShaderPath);
		~ShaderProgram();

		void UseProgram();
		void SetUniform(const char* uniformName, glm::mat4 value);
		void SetUniform(const char* uniformName, glm::mat3 value);
		void SetUniform(const char* uniformName, glm::vec4 value);
		void SetUniform(const char* uniformName, glm::vec3 value);
		void SetUniform(const char* uniformName, float value);
		void SetUniform(const char* uniformName, int value);

	private:

		void ShaderCompileCheck(unsigned int shaderID, const char* errorMessage);
		void ShaderProgramLinkCheck(unsigned int programID, const char* errorMessage);

		unsigned int m_vertexShaderID;
		unsigned int m_fragmentShaderID;
		unsigned int m_shaderProgramID;

	};
}