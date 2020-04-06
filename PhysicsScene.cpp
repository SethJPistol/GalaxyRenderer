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
	for (int i = 0; i < m_pObjects.size(); ++i)
	{
		delete m_pObjects[i];
		m_pObjects[i] = nullptr;
	}
	m_pObjects.clear();
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
				glm::vec2 resultNormal = functionPointer(object1, object2);
				if (glm::length(resultNormal) > 0.001f)
				{
					RigidBody* pRb1 = dynamic_cast<RigidBody*>(object1);
					RigidBody* pRb2 = dynamic_cast<RigidBody*>(object2);

					//DEBUG
					if (shapeType1 == 1 || shapeType2 == 1)
					{
						int a = 0;
					}

					if (pRb1 != nullptr && pRb2 != nullptr)
					{
						//Collision restitution
						pRb1->RigidBody::SetPosition(pRb1->GetPosition() + 0.5f * resultNormal);
						pRb2->RigidBody::SetPosition(pRb2->GetPosition() - 0.5f * resultNormal);
						//Collision resolution
						pRb1->ResolveCollision(pRb2, resultNormal);
					}
					else if (pRb1 != nullptr)
					{
						pRb1->RigidBody::SetPosition(pRb1->GetPosition() + resultNormal);
						Plane* p = dynamic_cast<Plane*>(object2);
						p->ResolveCollision(pRb1);
					}
					else if (pRb2)
					{
						pRb2->RigidBody::SetPosition(pRb2->GetPosition() + resultNormal);
						Plane* p = dynamic_cast<Plane*>(object1);
						p->ResolveCollision(pRb2);
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
void PhysicsScene::SetGravity(glm::vec2 gravity)
{
	m_gravity = gravity;
}
glm::vec2 PhysicsScene::GetGravity()
{
	return m_gravity;
}

glm::vec2 PhysicsScene::CircleVsCircle(PhysicsObject* pCircleA, PhysicsObject* pCircleB)
{
	Circle* c1 = dynamic_cast<Circle*>(pCircleA);
	Circle* c2 = dynamic_cast<Circle*>(pCircleB);

	if (c1 != nullptr && c2 != nullptr)
	{
		glm::vec2 displacement = c1->RigidBody::GetPosition() - c2->RigidBody::GetPosition();
		float radiiSum = c1->GetRadius() + c2->GetRadius();
		float overlap = radiiSum - glm::length(displacement);
		if (overlap > 0.0f)
			return glm::normalize(displacement) * overlap;
	}
	return glm::vec2(0.0f);
}
glm::vec2 PhysicsScene::CircleVsBox(PhysicsObject* pCircle, PhysicsObject* pBox)
{
	Circle* c = dynamic_cast<Circle*>(pCircle);
	Quad* b = dynamic_cast<Quad*>(pBox);

	if (c != nullptr && b != nullptr)
	{
		glm::vec2 clampedCentre = glm::clamp(c->RigidBody::GetPosition(), b->GetMin(), b->GetMax());
		glm::vec2 displacement = clampedCentre - c->RigidBody::GetPosition();

		float overlap = c->GetRadius() - glm::length(displacement);

		if (overlap > 0.0f)
		{
			return -glm::normalize(displacement) * overlap;
		}
	}
	return glm::vec2(0.0f);
}
glm::vec2 PhysicsScene::CircleVsPlane(PhysicsObject* pCircle, PhysicsObject* pPlane)
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
			return p->GetNormal() * overlap;
	}
	return glm::vec2(0.0f);
}
glm::vec2 PhysicsScene::BoxVsCircle(PhysicsObject* pBox, PhysicsObject* pCircle)
{
	return CircleVsBox(pCircle, pBox);
}
glm::vec2 PhysicsScene::BoxVsBox(PhysicsObject* pBoxA, PhysicsObject* pBoxB)
{
	Quad* b1 = dynamic_cast<Quad*>(pBoxA);
	Quad* b2 = dynamic_cast<Quad*>(pBoxB);


	if (b1 != nullptr && b2 != nullptr)
	{
		float maxOverlap = 10000000.0f;	//Float equivalent of infinity
		float overlap = maxOverlap;

		glm::vec2 overlapVector = glm::vec2(0.0f);

		glm::vec2 b1Max = b1->GetMax();
		glm::vec2 b1Min = b1->GetMin();
		glm::vec2 b2Max = b2->GetMax();
		glm::vec2 b2Min = b2->GetMin();

		if (b1Max.x > b2Min.x)
		{
			float localOverlap = abs(b1Max.x - b2Min.x);
			if (localOverlap < overlap)
			{
				overlap = localOverlap;
				overlapVector = glm::vec2(-overlap, 0.0f);
			}
		}
		else
			return glm::vec2(0.0f);
		if (b1Min.x < b2Max.x)
		{
			float localOverlap = abs(b1Min.x - b2Max.x);
			if (localOverlap < overlap)
			{
				overlap = localOverlap;
				overlapVector = glm::vec2(overlap, 0.0f);
			}
		}
		else
			return glm::vec2(0.0f);
		if (b1Max.y > b2Min.y)
		{
			float localOverlap = abs(b1Max.y - b2Min.y);
			if (localOverlap < overlap)
			{
				overlap = localOverlap;
				overlapVector = glm::vec2(0.0f, -overlap);
			}
		}
		else
			return glm::vec2(0.0f);
		if (b1Min.y < b2Max.y)
		{
			float localOverlap = abs(b1Min.y - b2Max.y);
			if (localOverlap < overlap)
			{
				overlap = localOverlap;
				overlapVector = glm::vec2(0.0f, overlap);
			}
		}
		else
			return glm::vec2(0.0f);

		/*return !(b1->GetMax().x < b2->GetMin().x ||
				b1->GetMax().x > b2->GetMax().x ||
				b1->GetMax().y < b2->GetMin().y ||
				b1->GetMax().y > b2->GetMax().y);*/

		return overlapVector;
	}
	return glm::vec2(0.0f);
}
glm::vec2 PhysicsScene::BoxVsPlane(PhysicsObject* pBox, PhysicsObject* pPlane)
{
	Quad* b = dynamic_cast<Quad*>(pBox);
	Plane* p = dynamic_cast<Plane*>(pPlane);

	if (b != nullptr && p != nullptr)
	{
		//Convert the AABB into a circle with an appropriate radius instead
		glm::vec2 positiveExtents = b->GetMax() - b->RigidBody::GetPosition();
		glm::vec2 negativeXExtents = glm::vec2(-positiveExtents.x, positiveExtents.y);

		float dotPositiveExtents = glm::dot(positiveExtents, p->GetNormal());
		float dotNegativeXExtents = glm::dot(negativeXExtents, p->GetNormal());

		//The radius is the larger of the two dot products
		float radius = fmaxf(fabsf(dotPositiveExtents), fabsf(dotNegativeXExtents));

		Circle projection(glm::vec3(b->RigidBody::GetPosition(), 0.0f), radius);
		return CircleVsPlane(&projection, pPlane);
	}
	return glm::vec2(0.0f);
}
glm::vec2 PhysicsScene::PlaneVsCircle(PhysicsObject* pPlane, PhysicsObject* pCircle)
{
	return CircleVsPlane(pCircle, pPlane);
}
glm::vec2 PhysicsScene::PlaneVsBox(PhysicsObject* pPlane, PhysicsObject* pBox)
{
	return BoxVsPlane(pBox, pPlane);
}
glm::vec2 PhysicsScene::PlaneVsPlane(PhysicsObject* pPlaneA, PhysicsObject* pPlaneB)
{
	return glm::vec2(0.0f);
}