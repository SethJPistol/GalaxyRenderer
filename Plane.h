#pragma once
#include "PhysicsObject.h"

class Plane : public PhysicsObject
{
public:
	Plane(glm::vec2 normal = glm::vec2(0.0f, 1.0f), float distance = 1.0f);
	~Plane();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);

	void Draw();

	glm::vec2 GetNormal();
	float GetDistance();

protected:
	glm::vec2 m_normal;
	float m_distance;
};

