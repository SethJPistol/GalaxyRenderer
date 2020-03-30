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
protected:
	PhysicsObject(ShapeType shape) : m_shape(shape) {}

	ShapeType m_shape;

public:
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	ShapeType GetShapeType() { return m_shape; }
};

