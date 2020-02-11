#pragma once
#include "glm.hpp"
#include "..\glcore\gl_core_4_5.h"


class Shape
{

public:
	enum DrawMode
	{
		eCube,

	};

	Shape(Shape::DrawMode shape);
	~Shape();

	void MakeCube();

	void Draw();

	glm::vec3 GetPosition();
	void SetPosition(glm::vec3 pos);

	float GetScale();
	void SetScale(float scale);

	unsigned int GetVAO();


private:

	unsigned int m_VAO;	//Vertex Array Object, to contain the VBO and IBO
	unsigned int m_VBO;	//Vertex Buffer Object, to store the array of points for the geometry
	unsigned int m_IBO;	//Index Buffer Object, to store the indexes of the verts we want to draw

	glm::vec3* m_vertices = nullptr;
	int* m_indexBuffer = nullptr;
	int m_vertexAmount;
	int m_indexAmount;

	glm::mat4 m_localTransform;
	glm::vec3* m_vertexOffsets = nullptr;
	float m_scale = 1.0f;
};