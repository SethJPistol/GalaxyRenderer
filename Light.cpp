#include "Light.h"

#include "..\glcore\gl_core_4_5.h"
#include "ext.hpp"

#include <fstream>
#include <sstream>

using namespace glxy;

Light::Light()
{
	ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	specular = glm::vec3(1.0f, 1.0f, 1.0f);
}

Light::~Light()
{

}



DirectionalLight::DirectionalLight()
{
	direction = glm::vec3(-1.0f, 0.0f, 0.0f);
}

void DirectionalLight::Update(int lightIndex)
{
	//Get the shader ID to use
	int shaderProgramID = -1;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgramID);
	if (shaderProgramID == -1)	//If none was found,
	{
		printf("No shader program bound when trying to update directional light\n");
		return;
	}

	//Set the light property uniforms
	std::string uniformName = "directional_lights[" + std::to_string(lightIndex) + "].ambient";
	auto uniform_location = glGetUniformLocation(shaderProgramID, uniformName.c_str());
	glUniform3fv(uniform_location, 1, glm::value_ptr(ambient));

	uniformName = "directional_lights[" + std::to_string(lightIndex) + "].diffuse";
	uniform_location = glGetUniformLocation(shaderProgramID, uniformName.c_str());
	glUniform3fv(uniform_location, 1, glm::value_ptr(diffuse));

	uniformName = "directional_lights[" + std::to_string(lightIndex) + "].specular";
	uniform_location = glGetUniformLocation(shaderProgramID, uniformName.c_str());
	glUniform3fv(uniform_location, 1, glm::value_ptr(specular));

	uniformName = "directional_lights[" + std::to_string(lightIndex) + "].direction";
	uniform_location = glGetUniformLocation(shaderProgramID, uniformName.c_str());
	glUniform3fv(uniform_location, 1, glm::value_ptr(direction));
}