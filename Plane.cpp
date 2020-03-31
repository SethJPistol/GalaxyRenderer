#include "Plane.h"
#include "RigidBody.h"

#include "glm.hpp"
#include "..\glcore\gl_core_4_5.h"
#include "glfw3.h"
#include "ext.hpp"

using namespace glxy;

Plane::Plane(glm::vec2 normal, float distance) : PhysicsObject(ShapeType::PLANE)
{
	m_normal = glm::normalize(normal);
	m_distance = distance;

	CreateMesh();
	LoadMesh();
}
Plane::~Plane()
{
	//Clearing memory
	glDeleteBuffers(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);

	delete[] m_vertices;
	delete[] m_indexBuffer;
}

void Plane::CreateMesh()
{
	m_vertexAmount = 2;
	//Set the vertex positions
	m_vertices = new Vertex[m_vertexAmount];

	glm::vec2 centre = m_normal * m_distance;
	glm::vec2 drawDirection = glm::vec2(m_normal.y, -m_normal.x);
	m_vertices[0].position = glm::vec3(centre + (drawDirection * 500.0f), 0.0f);
	m_vertices[1].position = glm::vec3(centre - (drawDirection * 500.0f), 0.0f);

	//Create the line
	m_indexAmount = 2;
	m_indexBuffer = new int[m_indexAmount]
	{
		0, 1
	};
}

void Plane::LoadMesh()
{
	//Generate the buffers
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	//Bind vertex array
	glBindVertexArray(m_VAO);
	//Bind buffers
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	//Fill buffers
	glBufferData(GL_ARRAY_BUFFER, m_vertexAmount * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexAmount * sizeof(int), m_indexBuffer, GL_STATIC_DRAW);

	//Set the attributes of each vertex
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	//Unbind the array and buffers for safety
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Plane::FixedUpdate(glm::vec2 gravity, float timeStep)
{

}

void Plane::Draw()
{
	//Get the shader ID to use
	int shaderProgramID = -1;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgramID);
	if (shaderProgramID == -1)	//If none was found,
	{
		printf("No shader program bound when trying to draw plane\n");
		return;
	}

	//Feed through the global transform to the vertex shader
	auto uniformLocation = glGetUniformLocation(shaderProgramID, "model_matrix");
	glUniformMatrix4fv(uniformLocation, 1, false, glm::value_ptr(glm::mat4(1)));

	//Binding and drawing arrays
	glBindVertexArray(m_VAO);
	glDrawElements(GL_LINES, m_indexAmount, GL_UNSIGNED_INT, 0);	//Used for drawing using the IBO
}

void Plane::ResolveCollision(RigidBody* pOther)
{
	glm::vec2 relativeVelocity = pOther->GetVelocity();
	float elasticity = 1.0f;

	float j = glm::dot(-(1 + elasticity) * relativeVelocity, m_normal)
		/ glm::dot(m_normal, m_normal * (1 / pOther->GetMass()));

	glm::vec2 force = m_normal * j;

	pOther->ApplyForce(force);
}

glm::vec2 Plane::GetNormal()
{
	return m_normal;
}
float Plane::GetDistance()
{
	return m_distance;
}