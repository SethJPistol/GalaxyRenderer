#pragma once
#include "glm.hpp"

namespace glxy
{
	class Light
	{
	public:

		Light();
		~Light();

		virtual void Update(int lightIndex) = 0;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;


	private:

	};

	class DirectionalLight : public Light
	{
	public:

		DirectionalLight();

		void Update(int lightIndex);

		glm::vec3 direction;

	private:

	};
}