#pragma once

#include "glm.hpp"
#include <vector>
#include "PhysicsObject.h"

class PhysicsObject;

using CollisionFunction = std::function<glm::vec2(PhysicsObject*, PhysicsObject*)>;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void Update(float deltaTime);

	void AddObject(PhysicsObject* pObject);
	void RemoveObject(PhysicsObject* pObject);

	void SetTimeStep(float timeStep);
	float GetTimeStep();
	void SetGravity(glm::vec2 gravity);
	glm::vec2 GetGravity();

	static glm::vec2 CircleVsCircle(PhysicsObject* pCircleA, PhysicsObject* pCircleB);
	static glm::vec2 CircleVsBox(PhysicsObject* pCircle, PhysicsObject* pBox);
	static glm::vec2 CircleVsPlane(PhysicsObject* pCircle, PhysicsObject* pPlane);
	static glm::vec2 BoxVsCircle(PhysicsObject* pBox, PhysicsObject* pCircle);
	static glm::vec2 BoxVsBox(PhysicsObject* pBoxA, PhysicsObject* pBoxB);
	static glm::vec2 BoxVsPlane(PhysicsObject* pBox, PhysicsObject* pPlane);
	static glm::vec2 PlaneVsCircle(PhysicsObject* pPlane, PhysicsObject* pCircle);
	static glm::vec2 PlaneVsBox(PhysicsObject* pPlane, PhysicsObject* pBox);
	static glm::vec2 PlaneVsPlane(PhysicsObject* pPlaneA, PhysicsObject* pPlaneB);

protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_pObjects;

	static CollisionFunction m_collisionFunctions[];
};

