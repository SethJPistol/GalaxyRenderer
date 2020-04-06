#pragma once
#include "PhysicsObject.h"

class RigidBody : public PhysicsObject
{
public:
	RigidBody(ShapeType shape, glm::vec2 position, glm::vec2 velocity, float mass, float drag = 0.01f);
	virtual ~RigidBody();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);

	void ApplyForce(glm::vec2 force);
	void ApplyForceToObject(RigidBody* pObject, glm::vec2 force);

	void ResolveCollision(RigidBody* pOther, glm::vec2 collisionNormal);

	glm::vec2 GetPosition();
	void SetPosition(glm::vec2 position);
	glm::vec2 GetVelocity();
	void SetVelocity(glm::vec2 velocity);
	float GetMass();
	void SetDrag(float drag);
	float GetDrag();


protected:

	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_drag;
};

