#include "RigidBody.h"

RigidBody::RigidBody(ShapeType shape, glm::vec2 position, glm::vec2 velocity, float mass, float drag) : PhysicsObject(shape)
{
	m_position = position;
	m_velocity = velocity;
	m_mass = mass;
	m_drag = drag;
}
RigidBody::~RigidBody()
{

}

void RigidBody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	ApplyForce(gravity * m_mass * timeStep);
	m_velocity -= m_velocity * m_drag * timeStep;	//Apply drag
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

void RigidBody::ResolveCollision(RigidBody* pOther, glm::vec2 collisionNormal)
{
	glm::vec2 normal = glm::normalize(collisionNormal);
	glm::vec2 relativeVelocity = pOther->GetVelocity() - m_velocity;

	float j = glm::dot(-(1 + (m_elasticity * pOther->GetElasticity())) * relativeVelocity, normal) 
			/ glm::dot(normal, normal * ((1 / m_mass) + (1 / pOther->GetMass())));

	glm::vec2 force = normal * j;

	ApplyForceToObject(pOther, force);
}

glm::vec2 RigidBody::GetPosition()
{
	return m_position;
}
void RigidBody::SetPosition(glm::vec2 position)
{
	m_position = position;
}
glm::vec2 RigidBody::GetVelocity()
{
	return m_velocity;
}
void RigidBody::SetVelocity(glm::vec2 velocity)
{
	m_velocity = velocity;
}
float RigidBody::GetMass()
{
	return m_mass;
}
void RigidBody::SetDrag(float drag)
{
	m_drag = drag;
}
float RigidBody::GetDrag()
{
	return m_drag;
}