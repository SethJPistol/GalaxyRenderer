#include "RigidBody.h"

RigidBody::RigidBody(ShapeType shape, glm::vec2 position, glm::vec2 velocity, float mass) : PhysicsObject(shape)
{
	m_position = position;
	m_velocity = velocity;
	m_mass = mass;
}
RigidBody::~RigidBody()
{

}

void RigidBody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	ApplyForce(gravity * m_mass * timeStep);
	m_position += m_velocity * timeStep;
}

void RigidBody::ApplyForce(glm::vec2 force)
{
	//a = F/m
	m_velocity += (force / m_mass);
}
void RigidBody::ApplyForceToObject(RigidBody* pObject, glm::vec2 force)
{
	pObject->ApplyForce(force);
	ApplyForce(-force);
}

glm::vec2 RigidBody::GetPosition()
{
	return m_position;
}
glm::vec2 RigidBody::GetVelocity()
{
	return m_velocity;
}
float RigidBody::GetMass()
{
	return m_mass;
}