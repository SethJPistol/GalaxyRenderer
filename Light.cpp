#include "Light.h"

#include "..\glcore\gl_core_4_5.h"
#include "ext.hpp"

using namespace glxy;

Light::Light()
{
	direction = glm::vec3(-1.0f, 0.0f, 0.0f);
	diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	specular = glm::vec3(1.0f, 1.0f, 1.0f);
}

Light::~Light()
{

}

void Light::Update()
{
	//Get the shader ID to use
	int shaderProgramID = -1;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgramID);
	if (shaderProgramID == -1)	//If none was found,
	{
		printf("No shader program bound when trying to update light\n");
		return;
	}

	//Set the light property uniforms
	auto uniform_location = glGetUniformLocation(shaderProgramID, "light_direction");
	glUniform3fv(uniform_location, 1, glm::value_ptr(direction));
	uniform_location = glGetUniformLocation(shaderProgramID, "light_diffuse");
	glUniform3fv(uniform_location, 1, glm::value_ptr(diffuse));
	uniform_location = glGetUniformLocation(shaderProgramID, "light_specular");
	glUniform3fv(uniform_location, 1, glm::value_ptr(specular));
}