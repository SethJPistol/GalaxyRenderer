#pragma once
#include "glm.hpp"
#include "..\glcore\gl_core_4_5.h"

namespace gal
{
	class Shape
	{
	public:

		Shape(glm::vec3 position = glm::vec3(0), float scale = 1.0f);
		~Shape();

		void Draw();

		glm::vec3 GetPosition();
		void SetPosition(glm::vec3 position);

		float GetScale();
		void SetScale(float scale);

		unsigned int GetVAO();


	protected:

		virtual void CreateMesh() = 0;	//Pure virtual function to set the verts and indexes
		void LoadMesh();	//Assign memory and create the array/buffer objects

		unsigned int m_VAO;	//Vertex Array Object, to contain the VBO and IBO
		unsigned int m_VBO;	//Vertex Buffer Object, to store the array of points for the geometry
		unsigned int m_IBO;	//Index Buffer Object, to store the indexes of the verts we want to draw

		glm::vec3* m_vertices = nullptr;
		int* m_indexBuffer = nullptr;
		int m_vertexAmount;
		int m_indexAmount;

		glm::mat4 m_localTransform;
		float m_scale = 1.0f;
	};


	class Cube : public Shape
	{
	public:

		Cube(glm::vec3 position = glm::vec3(0), float scale = 1.0f);

		void CreateMesh();
	};


	class Polygon : public Shape
	{
	public:

		Polygon(int sides = 3, glm::vec3 position = glm::vec3(0), float scale = 1.0f);

		void CreateMesh();

	private:

		int m_sides;
	};


	class Prism : public Shape
	{
	public:

		Prism(int sides = 3, float height = 1.0f, glm::vec3 position = glm::vec3(0), float scale = 1.0f);

		void CreateMesh();

	private:

		int m_sides;
		float m_height;
	};
}