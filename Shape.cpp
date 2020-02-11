#include "Shape.h"

Shape::Shape(Shape::DrawMode shape)
{
	switch (shape)
	{
	case eCube:
		MakeCube();
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
	m_vertexOffsets = new glm::vec3[m_vertexAmount];
	m_vertexOffsets[0] = glm::vec3(-0.5f, 0.5f, 0.5f);
	m_vertexOffsets[1] = glm::vec3(0.5f, 0.5f, 0.5f);
	m_vertexOffsets[2] = glm::vec3(-0.5f, -0.5f, 0.5f);
	m_vertexOffsets[3] = glm::vec3(0.5f, -0.5f, 0.5f);
	m_vertexOffsets[4] = glm::vec3(0.5f, 0.5f, -0.5f);
	m_vertexOffsets[5] = glm::vec3(-0.5f, 0.5f, -0.5f);
	m_vertexOffsets[6] = glm::vec3(0.5f, -0.5f, -0.5f);
	m_vertexOffsets[7] = glm::vec3(-0.5f, -0.5f, -0.5f);
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

	m_indexBuffer = new int[36]
	{
		0, 2, 1, 1, 2, 3,	//Front face
		4, 6, 5, 5, 6, 7, 	//Back face
		5, 7, 0, 0, 7, 2,	//Left face
		1, 3, 4, 4, 3, 6,	//Right face
		5, 0, 4, 4, 0, 1,	//Top face
		3, 2, 6, 2, 7, 6	//Bottom face
	};	//Winding order
	m_indexAmount = 36;
}

void Shape::Draw()
{
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
	for (int i = 0; i < m_vertexAmount; ++i)
	{
		m_vertices[i] = pos + m_vertexOffsets[i];
	}

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertexAmount * sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexAmount * sizeof(int), m_indexBuffer, GL_STATIC_DRAW);

	//Unbind the array and buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_localTransform[3] = glm::vec4(pos, 0.0f);
}

float Shape::GetScale()
{
	return m_scale;
}

void Shape::SetScale(float scale)
{
	m_scale = scale;
}

unsigned int Shape::GetVAO()
{
	return 0;
}