#include "PhysicsScene.h"
#include "RigidBody.h"
#include <algorithm>
#include <list>

CollisionFunction PhysicsScene::m_collisionFunctions[] =
{
	PhysicsScene::CircleVsCircle, PhysicsScene::CircleVsBox, PhysicsScene::CircleVsPlane,
	PhysicsScene::BoxVsCircle, PhysicsScene::BoxVsBox, PhysicsScene::BoxVsPlane,
	PhysicsScene::PlaneVsCircle, PhysicsScene::PlaneVsBox, PhysicsScene::PlaneVsPlane
};

PhysicsScene::PhysicsScene()
{
	m_timeStep = 0.01f;
	m_gravity = glm::vec2(0.0f, 0.0f);
}
PhysicsScene::~PhysicsScene()
{
	while (!m_pObjects.empty())
	{
		m_pObjects.erase(m_pObjects.end() - 1);
	}
}

void PhysicsScene::Update(float deltaTime)
{
	static float accumulatedTime = 0.0f;
	accumulatedTime += deltaTime;

	//Update all physics objects
	while (accumulatedTime >= m_timeStep)
	{
		for (int i = 0; i < m_pObjects.size(); ++i)
			m_pObjects[i]->FixedUpdate(m_gravity, m_timeStep);
		accumulatedTime -= m_timeStep;
	}

	//Collision check all physics objects
	static std::list<PhysicsObject*> pDirty;	//Tracks which objects have already collided
	for (auto pObject : m_pObjects)
	{
		for (auto pOther : m_pObjects)
		{
			if (pObject == pOther)
				continue;

			if (std::find(pDirty.begin(), pDirty.end(), pObject) != pDirty.end()
				&& std::find(pDirty.begin(), pDirty.end(), pOther) != pDirty.end())
				continue;

			RigidBody* pRb = dynamic_cast<RigidBody*>(pObject);
			if (pRb->CheckCollision(pOther) == true)
			{
				glm::vec2 force = pRb->GetVelocity() * pRb->GetMass();
				pRb->ApplyForceToObject(dynamic_cast<RigidBody*>(pOther), force);
				pDirty.push_back(pObject);
				pDirty.push_back(pOther);
			}
		}
	}
	pDirty.clear();
}

void PhysicsScene::AddObject(PhysicsObject* pObject)
{
	if (pObject != nullptr)
		m_pObjects.push_back(pObject);
}
void PhysicsScene::RemoveObject(PhysicsObject* pObject)
{
	if (pObject != nullptr)
	{
		std::vector<PhysicsObject*>::iterator index = std::find(m_pObjects.begin(), m_pObjects.end(), pObject);
		if (index != m_pObjects.end())	//If it was found,
			m_pObjects.erase(index);
		delete pObject;
		pObject = nullptr;
	}
}

void PhysicsScene::SetTimeStep(float timeStep)
{
	m_timeStep = timeStep;
}
float PhysicsScene::GetTimeStep()
{
	return m_timeStep;
}

bool PhysicsScene::CircleVsCircle(PhysicsObject* pCircleA, PhysicsObject* pCircleB)
{
	return false;
}
bool PhysicsScene::CircleVsBox(PhysicsObject* pCircle, PhysicsObject* pBox)
{
	return false;
}
bool PhysicsScene::CircleVsPlane(PhysicsObject* pCircle, PhysicsObject* pPlane)
{
	return false;
}
bool PhysicsScene::BoxVsCircle(PhysicsObject* pBox, PhysicsObject* pCircle)
{
	return false;
}
bool PhysicsScene::BoxVsBox(PhysicsObject* pBoxA, PhysicsObject* pBoxB)
{
	return false;
}
bool PhysicsScene::BoxVsPlane(PhysicsObject* pBox, PhysicsObject* pPlane)
{
	return false;
}
bool PhysicsScene::PlaneVsCircle(PhysicsObject* pPlane, PhysicsObject* pCircle)
{
	return false;
}
bool PhysicsScene::PlaneVsBox(PhysicsObject* pPlane, PhysicsObject* pBox)
{
	return false;
}
bool PhysicsScene::PlaneVsPlane(PhysicsObject* pPlaneA, PhysicsObject* pPlaneB)
{
	return false;
}