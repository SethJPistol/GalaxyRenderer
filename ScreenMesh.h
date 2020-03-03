#pragma once

#include "glm.hpp"

namespace glxy
{
	class ScreenMesh
	{
	public:

		struct Vertex
		{
			glm::vec2 position;
		};

		ScreenMesh();
		ScreenMesh(unsigned int textureHandle);
		~ScreenMesh();

		void LoadTexture(unsigned int textureHandle);

		void Draw();

	private:

		void CreateMesh();
		void LoadMesh();

		unsigned int m_VAO;	//Vertex Array Object, to contain the VBO and IBO
		unsigned int m_VBO;	//Vertex Buffer Object, to store the array of points for the geometry
		unsigned int m_IBO;	//Index Buffer Object, to store the indexes of the verts we want to draw

		Vertex* m_vertices = nullptr;
		int* m_indexBuffer = nullptr;
		int m_vertexAmount;
		int m_indexAmount;

		unsigned int m_screenTexture = 0;
	};
}