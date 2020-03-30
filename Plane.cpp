#include "Plane.h"

#include "glm.hpp"
#include "..\glcore\gl_core_4_5.h"
#include "glfw3.h"

Plane::Plane(glm::vec2 normal, float distance) : PhysicsObject(ShapeType::PLANE)
{
	m_normal = normal;
	m_distance = distance;
}
Plane::~Plane()
{

}

void Plane::FixedUpdate(glm::vec2 gravity, float timeStep)
{

}

void Plane::Draw()
{
	glm::vec2 centre = m_normal * m_distance;
	glm::vec2 drawDirection = glm::vec2(m_normal.y, -m_normal.x);
	//Draw(centre + (drawDirection * 500.0f), centre - (drawDirection * 500.0f))
}

glm::vec2 Plane::GetNormal()
{
	return m_normal;
}
float Plane::GetDistance()
{
	return m_distance;
}