#pragma once

#include "glm.hpp"
#include <functional>

#define SHAPE_AMOUNT 3

enum class ShapeType : char
{
	CIRCLE = 0,
	BOX,
	PLANE
};

class PhysicsObject
{
public:
	virtual ~PhysicsObject() {}
protected:
	PhysicsObject(ShapeType shape) : m_shape(shape) {}

	ShapeType m_shape;

	float m_elasticity = 0.5f;

public:
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	ShapeType GetShapeType() { return m_shape; }
	void SetElasticity(float elasticity) { m_elasticity = elasticity; }
	float GetElasticity() { return m_elasticity; }
};

