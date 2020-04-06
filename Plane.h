#pragma once
#include "PhysicsObject.h"

class RigidBody;

namespace glxy
{
	class Plane : public PhysicsObject
	{
	public:

		struct Vertex
		{
			glm::vec3 position;
		};

		Plane(glm::vec2 normal = glm::vec2(0.0f, 1.0f), float distance = -1.0f);
		virtual ~Plane() override;

		void CreateMesh();
		void LoadMesh();

		virtual void FixedUpdate(glm::vec2 gravity, float timeStep);

		void Draw();

		void ResolveCollision(RigidBody* pOther);

		glm::vec2 GetNormal();
		float GetDistance();

	protected:
		glm::vec2 m_normal;
		float m_distance;

		unsigned int m_VAO;	//Vertex Array Object, to contain the VBO and IBO
		unsigned int m_VBO;	//Vertex Buffer Object, to store the array of points for the geometry
		unsigned int m_IBO;	//Index Buffer Object, to store the indexes of the verts we want to draw

		Vertex* m_vertices = nullptr;
		int* m_indexBuffer = nullptr;
		int m_vertexAmount;
		int m_indexAmount;
	};
}

