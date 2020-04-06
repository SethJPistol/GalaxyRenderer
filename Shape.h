#pragma once
#include "glm.hpp"
#include "..\glcore\gl_core_4_5.h"
#include "RigidBody.h"

namespace glxy
{
	class Shape
	{
	public:

		struct Vertex
		{
			glm::vec3 position;
		};

		Shape(glm::vec3 position = glm::vec3(0), float scale = 1.0f);
		virtual ~Shape();

		virtual void Draw();

		glm::vec3 GetPosition();
		void SetPosition(glm::vec3 position);

		float GetScale();
		virtual void SetScale(float scale);

		unsigned int GetVAO();


	protected:

		virtual void CreateMesh() = 0;	//Pure virtual function to set the verts and indexes
		void LoadMesh();				//Assign memory and create the array/buffer objects

		unsigned int m_VAO;	//Vertex Array Object, to contain the VBO and IBO
		unsigned int m_VBO;	//Vertex Buffer Object, to store the array of points for the geometry
		unsigned int m_IBO;	//Index Buffer Object, to store the indexes of the verts we want to draw

		Vertex* m_vertices = nullptr;
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

	private:
		void CreateMesh();
	};


	class Quad : public Shape, public RigidBody
	{
	public:
		Quad(glm::vec3 position = glm::vec3(0), float scale = 1.0f,
			glm::vec2 velocity = glm::vec2(0), float mass = 1.0f);

		virtual bool CheckCollision(PhysicsObject* pOther);

		void Draw();

		glm::vec2 GetMin();
		glm::vec2 GetMax();

	private:
		void CreateMesh();
	};


	class Polygon : public Shape
	{
	public:
		Polygon(int sides = 3, glm::vec3 position = glm::vec3(0), float scale = 1.0f);
		virtual ~Polygon();

	protected:
		void CreateMesh();
		int m_sides;
	};

	class Circle : public Polygon, public RigidBody
	{
	public:
		Circle(glm::vec3 position = glm::vec3(0), 
				float scale = 1.0f,
				glm::vec2 velocity = glm::vec2(0),
				float mass = 1.0f);

		void SetScale(float scale);
		float GetRadius();

		void Draw();

	protected:
		float m_radius;
	};


	class Prism : public Shape
	{
	public:
		Prism(int sides = 3, float height = 1.0f, glm::vec3 position = glm::vec3(0), float scale = 1.0f);

	private:
		void CreateMesh();
		int m_sides;
		float m_height;
	};

	class Pyramid : public Shape
	{
	public:
		Pyramid(int sides = 3, float height = 1.0f, glm::vec3 position = glm::vec3(0), float scale = 1.0f);

	private:
		void CreateMesh();
		int m_sides;
		float m_height;
	};
}