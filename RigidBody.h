#pragma once
#include "PhysicsObject.h"

class RigidBody : public PhysicsObject
{
public:
	RigidBody(ShapeType shape, glm::vec2 position, glm::vec2 velocity, float mass);
	~RigidBody();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);

	void ApplyForce(glm::vec2 force);
	void ApplyForceToObject(RigidBody* pObject, glm::vec2 force);

	virtual bool CheckCollision(PhysicsObject* pOther) = 0;

	glm::vec2 GetPosition();
	glm::vec2 GetVelocity();
	float GetMass();


protected:

	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
};

