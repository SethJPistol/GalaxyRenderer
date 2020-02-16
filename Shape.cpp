#include "Shape.h"
#include "ext.hpp"

Shape::Shape(Shape::DrawMode shape)
{
	switch (shape)
	{
	case eCube:
		MakeCube();
		break;

	case ePolygon:
		MakePolygon();
		break;

	case eCylinder:
		MakeCylinder();
		break;

	default:
		MakeCube();
		break;
	}

	//CREATE AND 'LOAD' MESH
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertexAmount * sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexAmount * sizeof(int), m_indexBuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	//Unbind the array and buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_localTransform = glm::mat4(1);
}

Shape::~Shape()
{
	//Clearing memory
	glDeleteBuffers(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);

	delete[] m_vertices;
	delete[] m_indexBuffer;
	delete[] m_vertexOffsets;
}

void Shape::MakeCube()
{
	m_vertexAmount = 8;
	//Set the vertex offsets
	m_vertexOffsets = new glm::vec3[m_vertexAmount];
	m_vertexOffsets[0] = glm::vec3(-0.5f, 0.5f, 0.5f);
	m_vertexOffsets[1] = glm::vec3(0.5f, 0.5f, 0.5f);
	m_vertexOffsets[2] = glm::vec3(-0.5f, -0.5f, 0.5f);
	m_vertexOffsets[3] = glm::vec3(0.5f, -0.5f, 0.5f);
	m_vertexOffsets[4] = glm::vec3(0.5f, 0.5f, -0.5f);
	m_vertexOffsets[5] = glm::vec3(-0.5f, 0.5f, -0.5f);
	m_vertexOffsets[6] = glm::vec3(0.5f, -0.5f, -0.5f);
	m_vertexOffsets[7] = glm::vec3(-0.5f, -0.5f, -0.5f);

	//Set the vertex positions (same as offsets when at 0,0,0)
	m_vertices = new glm::vec3[m_vertexAmount];
	memcpy(m_vertices, m_vertexOffsets, sizeof(glm::vec3) * m_vertexAmount);
	//{
	//	//Front vertices
	//	glm::vec3(-0.5f, 0.5f, 0.5f),
	//	glm::vec3(0.5f, 0.5f, 0.5f),
	//	glm::vec3(-0.5f, -0.5f, 0.5f),
	//	glm::vec3(0.5f, -0.5f, 0.5f),

	//	//Back vertices
	//	glm::vec3(0.5f, 0.5f, -0.5f),
	//	glm::vec3(-0.5f, 0.5f, -0.5f),
	//	glm::vec3(0.5f, -0.5f, -0.5f),
	//	glm::vec3(-0.5f, -0.5f, -0.5f)
	//};

	//Create the tris
	m_indexAmount = 36;
	m_indexBuffer = new int[m_indexAmount]
	{
		0, 2, 1, 1, 2, 3,	//Front face
		4, 6, 5, 5, 6, 7, 	//Back face
		5, 7, 0, 0, 7, 2,	//Left face
		1, 3, 4, 4, 3, 6,	//Right face
		5, 0, 4, 4, 0, 1,	//Top face
		3, 2, 6, 2, 7, 6	//Bottom face
	};	//Winding order
}

void Shape::MakePolygon()
{
	m_sides = 6;
	m_vertexAmount = m_sides + 1;	//All verts on the outside and the centre vert
	//Set the vertex offsets
	m_vertexOffsets = new glm::vec3[m_vertexAmount];
	m_vertexOffsets[0] = glm::vec3(0);
	for (int i = 1; i < m_vertexAmount; ++i)
	{
		float currentAngle = glm::two_pi<float>() * ((float)(i - 1) / (float)m_sides);
		m_vertexOffsets[i] = glm::vec3(glm::cos(currentAngle), 0, -glm::sin(currentAngle));	//Sin negative to make face-up
	}

	//Set the vertex positions (same as offsets when at 0,0,0)
	m_vertices = new glm::vec3[m_vertexAmount];
	memcpy(m_vertices, m_vertexOffsets, sizeof(glm::vec3) * m_vertexAmount);

	//Create the tris
	m_indexAmount = 3 * m_sides;
	m_indexBuffer = new int[m_indexAmount];
	for (int i = 0; i < m_indexAmount; ++i)
	{
		m_indexBuffer[i] = 0;
		++i;
		m_indexBuffer[i] = i % 3 + (int)(i * 0.33333f);
		++i;
		m_indexBuffer[i] = i % 3 + (int)(i * 0.33333f);
	}
	m_indexBuffer[m_indexAmount - 1] = 1;
}

void Shape::MakeCylinder()
{
	m_sides = 3;
	m_vertexAmount = 2 * (m_sides + 1);	//Cylinder verts is same as two polygons
	//Set the vertex offsets
	m_vertexOffsets = new glm::vec3[m_vertexAmount];

	//Make the bottom polygon
	m_vertexOffsets[0] = glm::vec3(0.0f, -0.5f, 0.0f);
	for (int i = 1; i < (m_vertexAmount * 0.5f); ++i)
	{
		float currentAngle = glm::two_pi<float>() * ((float)(i - 1) / (float)m_sides);
		m_vertexOffsets[i] = glm::vec3(glm::cos(currentAngle), -0.5f, glm::sin(currentAngle));
	}
	//Make the top polygon
	m_vertexOffsets[(int)(m_vertexAmount * 0.5f)] = glm::vec3(0.0f, 0.5f, 0.0f);
	for (int i = (m_vertexAmount * 0.5f); i < m_vertexAmount; ++i)
	{
		float currentAngle = glm::two_pi<float>() * ((float)(i - 1) / (float)m_sides);
		m_vertexOffsets[i] = glm::vec3(glm::cos(currentAngle), 0.5f, -glm::sin(currentAngle));
	}

	//Set the vertex positions (same as offsets when at 0,0,0)
	m_vertices = new glm::vec3[m_vertexAmount];
	memcpy(m_vertices, m_vertexOffsets, sizeof(glm::vec3) * m_vertexAmount);

	//Create the tris
	m_indexAmount = 6 * m_sides;
	m_indexBuffer = new int[m_indexAmount];
	//Index the bottom polygon
	for (int i = 0; i < (m_indexAmount * 0.5f); ++i)
	{
		m_indexBuffer[i] = 0;
		++i;
		m_indexBuffer[i] = i % 3 + (int)(i * 0.33333f);
		++i;
		m_indexBuffer[i] = i % 3 + (int)(i * 0.33333f);
	}
	m_indexBuffer[(int)(m_indexAmount * 0.5f) - 1] = 1;
	//Index the top polygon
	for (int i = (m_indexAmount * 0.5f); i < m_indexAmount; ++i)
	{
		m_indexBuffer[i] = (m_indexAmount * 0.5f);
		++i;
		m_indexBuffer[i] = i % 3 + (i * 0.66666f);
		++i;
		m_indexBuffer[i] = i % 3 + (i * 0.66666f);
	}
	//9, 10, 11
	//9, 11, 12,
	//9, 12, 10
	m_indexBuffer[m_indexAmount - 1] = (m_indexAmount * 0.5f) + 1;
}

void Shape::Draw(unsigned int shaderProgramID)
{
	auto uniform_location = glGetUniformLocation(shaderProgramID, "model_matrix");
	glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(m_localTransform));

	//Binding and drawing arrays
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indexAmount, GL_UNSIGNED_INT, 0);	//Used for drawing using the IBO
}

glm::vec3 Shape::GetPosition()
{
	return (glm::vec3)m_localTransform[3];
}

void Shape::SetPosition(glm::vec3 pos)
{
	//for (int i = 0; i < m_vertexAmount; ++i)
	//{
	//	m_vertices[i] = pos + m_vertexOffsets[i];
	//}

	//glBindVertexArray(m_VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	//glBufferData(GL_ARRAY_BUFFER, m_vertexAmount * sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexAmount * sizeof(int), m_indexBuffer, GL_STATIC_DRAW);

	////Unbind the array and buffers
	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_localTransform[3] = glm::vec4(pos, 1.0f);
}

float Shape::GetScale()
{
	return m_scale;
}

void Shape::SetScale(float scale)
{
	for (int i = 0; i < m_vertexAmount; ++i)	//For all the vertices' offsets,
	{
		m_vertexOffsets[i] /= m_scale;	//Reset their scale to default
		m_vertexOffsets[i] *= scale;	//Set the new scale
	}
	m_scale = scale;
	SetPosition((glm::vec3)m_localTransform[3]);	//Apply the scale to the vertices
}

unsigned int Shape::GetVAO()
{
	return 0;
}