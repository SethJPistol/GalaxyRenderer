#pragma once
#include "glm.hpp"
#include "..\glcore\gl_core_4_5.h"

class Cube
{
public:

	Cube();
	Cube(glm::vec3 position);
	Cube(float scale);

	~Cube();

	void Draw();

	unsigned int GetVAO();

private:

	unsigned int m_VAO;	//Vertex Array Object, to contain the VBO and IBO
	unsigned int m_VBO;	//Vertex Buffer Object, to store the array of points for the geometry
	unsigned int m_IBO;	//Index Buffer Object, to store the indexes of the verts we want to draw

	int m_vertex_amount;
	int m_index_amount;
};