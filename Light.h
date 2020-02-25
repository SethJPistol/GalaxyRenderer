#pragma once
#include "glm.hpp"

namespace glxy
{
	class Light
	{
	public:

		Light();
		~Light();

		void Update();

		glm::vec3 direction;
		glm::vec3 diffuse;
		glm::vec3 specular;


	private:

	};
}