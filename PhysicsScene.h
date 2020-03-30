#pragma once

#include "glm.hpp"
#include <vector>
#include "PhysicsObject.h"

class PhysicsObject;

using CollisionFunction = std::function<bool(PhysicsObject*, PhysicsObject*)>;

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

	static bool CircleVsCircle(PhysicsObject* pCircleA, PhysicsObject* pCircleB);
	static bool CircleVsBox(PhysicsObject* pCircle, PhysicsObject* pBox);
	static bool CircleVsPlane(PhysicsObject* pCircle, PhysicsObject* pPlane);
	static bool BoxVsCircle(PhysicsObject* pBox, PhysicsObject* pCircle);
	static bool BoxVsBox(PhysicsObject* pBoxA, PhysicsObject* pBoxB);
	static bool BoxVsPlane(PhysicsObject* pBox, PhysicsObject* pPlane);
	static bool PlaneVsCircle(PhysicsObject* pPlane, PhysicsObject* pCircle);
	static bool PlaneVsBox(PhysicsObject* pPlane, PhysicsObject* pBox);
	static bool PlaneVsPlane(PhysicsObject* pPlaneA, PhysicsObject* pPlaneB);

protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_pObjects;

	static CollisionFunction m_collisionFunctions[];
};

