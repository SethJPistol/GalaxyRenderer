#include "PhysicsScene.h"
#include "RigidBody.h"

#include "Shape.h"
#include "Plane.h"

#include <algorithm>
#include <list>

using namespace glxy;

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

	int objectCount = m_pObjects.size();

	//Update all physics objects
	while (accumulatedTime >= m_timeStep)
	{
		for (int i = 0; i < objectCount; ++i)
			m_pObjects[i]->FixedUpdate(m_gravity, m_timeStep);
		accumulatedTime -= m_timeStep;
	}

	//Collision check all physics objects
	//static std::list<PhysicsObject*> pDirty;	//Tracks which objects have already collided
	//for (auto pObject : m_pObjects)
	//{
	//	for (auto pOther : m_pObjects)
	//	{
	//		if (pObject == pOther)
	//			continue;

	//		if (std::find(pDirty.begin(), pDirty.end(), pObject) != pDirty.end()
	//			&& std::find(pDirty.begin(), pDirty.end(), pOther) != pDirty.end())
	//			continue;

	//		RigidBody* pRb = dynamic_cast<RigidBody*>(pObject);
	//		if (pRb->CheckCollision(pOther) == true)
	//		{
	//			glm::vec2 force = pRb->GetVelocity() * pRb->GetMass();
	//			pRb->ApplyForceToObject(dynamic_cast<RigidBody*>(pOther), force);
	//			pDirty.push_back(pObject);
	//			pDirty.push_back(pOther);
	//		}
	//	}
	//}
	//pDirty.clear();

	for (int outer = 0; outer < objectCount - 1; ++outer)
	{
		for (int inner = outer + 1; inner < objectCount; ++inner)
		{
			PhysicsObject* object1 = m_pObjects[outer];
			PhysicsObject* object2 = m_pObjects[inner];
			int shapeType1 = (int)object1->GetShapeType();
			int shapeType2 = (int)object2->GetShapeType();

			int functionIndex = (shapeType1 * SHAPE_AMOUNT) + shapeType2;
			CollisionFunction functionPointer = m_collisionFunctions[functionIndex];
			if (functionPointer != nullptr)
			{
				//Check for collision
				bool collision = functionPointer(object1, object2);
				if (collision)
				{
					RigidBody* pRb1 = dynamic_cast<RigidBody*>(object1);
					RigidBody* pRb2 = dynamic_cast<RigidBody*>(object2);
					if (pRb1 != nullptr && pRb2 != nullptr)
					{
						glm::vec2 force = pRb1->GetVelocity() * pRb1->GetMass();
						pRb1->ApplyForceToObject(pRb2, force);
					}
				}
			}
		}
	}
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
	Circle* c1 = dynamic_cast<Circle*>(pCircleA);
	Circle* c2 = dynamic_cast<Circle*>(pCircleB);

	if (c1 != nullptr && c2 != nullptr)
	{
		float distance = glm::distance(c1->RigidBody::GetPosition(), c2->RigidBody::GetPosition());
		if (distance < (c1->GetRadius() + c2->GetRadius()))
			return true;	//return glm::normalize(distance) * (m_radius + pCircle->GetRadius());
	}
	return false;
}
bool PhysicsScene::CircleVsBox(PhysicsObject* pCircle, PhysicsObject* pBox)
{
	return false;
}
bool PhysicsScene::CircleVsPlane(PhysicsObject* pCircle, PhysicsObject* pPlane)
{
	Circle* c = dynamic_cast<Circle*>(pCircle);
	Plane* p = dynamic_cast<Plane*>(pPlane);

	if (c != nullptr && p != nullptr)
	{
		glm::vec2 normal = p->GetNormal();
		//Calculate the distance between the circle center and the nearest point on the plane
		float displacement = glm::dot(c->RigidBody::GetPosition(), normal) - p->GetDistance();

		//Enable collisions on both sides of the plane
		//Flip the normals if the circle is behind the plane
		if (displacement < 0)
		{
			normal *= -1.0f;
			displacement *= 1.0f;
		}

		//Calculate the distance between the nearest points on the plane and circle
		float overlap = c->GetRadius() - displacement;
		if (overlap > 0.0f)	//If they are overlapping,
		{
			//Stop the circle
			glm::vec2 force = c->GetVelocity() * c->GetMass();
			c->ApplyForce(-force);

			return true;
		}
	}
	return false;
}
bool PhysicsScene::BoxVsCircle(PhysicsObject* pBox, PhysicsObject* pCircle)
{
	return CircleVsBox(pCircle, pBox);
}
bool PhysicsScene::BoxVsBox(PhysicsObject* pBoxA, PhysicsObject* pBoxB)
{
	Quad* b1 = dynamic_cast<Quad*>(pBoxA);
	Quad* b2 = dynamic_cast<Quad*>(pBoxB);


	if (b1 != nullptr && b2 != nullptr)
	{
		return !(b1->GetMax().x < b2->GetMin().x ||
				b1->GetMax().x > b2->GetMax().x ||
				b1->GetMax().y < b2->GetMin().y ||
				b1->GetMax().y > b2->GetMax().y);
	}
	return false;
}
bool PhysicsScene::BoxVsPlane(PhysicsObject* pBox, PhysicsObject* pPlane)
{
	return false;
}
bool PhysicsScene::PlaneVsCircle(PhysicsObject* pPlane, PhysicsObject* pCircle)
{
	return CircleVsPlane(pCircle, pPlane);
}
bool PhysicsScene::PlaneVsBox(PhysicsObject* pPlane, PhysicsObject* pBox)
{
	return BoxVsPlane(pBox, pPlane);
}
bool PhysicsScene::PlaneVsPlane(PhysicsObject* pPlaneA, PhysicsObject* pPlaneB)
{
	return false;
}