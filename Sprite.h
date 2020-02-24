#pragma once

#include "glm.hpp"

namespace glxy
{
	class Sprite
	{
	public:
		struct Vertex
		{
			glm::vec3 position;
			glm::vec2 UV;
		};

		Sprite(glm::vec3 position = glm::vec3(0), float scale = 1.0f);
		~Sprite();
		void LoadTexture(const char* texturePath);
		void Draw();	//Override to draw the texture, if present

		glm::vec3 GetPosition();
		void SetPosition(glm::vec3 position);

		float GetScale();
		void SetScale(float scale);

		unsigned int GetVAO();

	private:
		void CreateMesh();
		void LoadMesh();	//Assign memory and create the array/buffer objects

		unsigned int m_VAO;	//Vertex Array Object, to contain the VBO and IBO
		unsigned int m_VBO;	//Vertex Buffer Object, to store the array of points for the geometry
		unsigned int m_IBO;	//Index Buffer Object, to store the indexes of the verts we want to draw

		Vertex* m_vertices = nullptr;
		int* m_indexBuffer = nullptr;
		int m_vertexAmount;
		int m_indexAmount;

		unsigned int m_texture = 0;

		glm::mat4 m_localTransform;
		float m_scale = 1.0f;
	};
}