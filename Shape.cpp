#include "Shape.h"
#include "ext.hpp"


using namespace glxy;

Shape::Shape(glm::vec3 position, float scale)
{
	m_localTransform = glm::mat4(1);
	m_scale = 1;
	SetPosition(position);
	SetScale(scale);
}

Shape::~Shape()
{
	//Clearing memory
	glDeleteBuffers(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);

	delete[] m_vertices;
	delete[] m_indexBuffer;
}

void Shape::Draw()
{
	//Get the shader ID to use
	int shaderProgramID = -1;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgramID);
	if (shaderProgramID == -1)	//If none was found,
	{
		printf("No shader program bound when trying to draw shape\n");
		return;
	}

	//Feed through the global transform to the vertex shader
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

void Shape::SetPosition(glm::vec3 position)
{
	m_localTransform[3] = glm::vec4(position, 1.0f);
}

float Shape::GetScale()
{
	return m_scale;
}

void Shape::SetScale(float scale)
{
	m_localTransform[0][0] /= m_scale;
	m_localTransform[0][0] *= scale;
	m_localTransform[1][1] /= m_scale;
	m_localTransform[1][1] *= scale;
	m_localTransform[2][2] /= m_scale;
	m_localTransform[2][2] *= scale;
}

unsigned int Shape::GetVAO()
{
	return 0;
}

void Shape::LoadMesh()
{
	//Generate the buffers
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	//Bind vertex array
	glBindVertexArray(m_VAO);
	//Bind buffers
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	//Fill buffers
	glBufferData(GL_ARRAY_BUFFER, m_vertexAmount * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexAmount * sizeof(int), m_indexBuffer, GL_STATIC_DRAW);

	//Set the attributes of each vertex
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	//Unbind the array and buffers for safety
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}



Cube::Cube(glm::vec3 position, float scale) : Shape(position, scale)
{
	CreateMesh();
	LoadMesh();
}
void Cube::CreateMesh()
{
	m_vertexAmount = 8;
	//Set the vertex positions
	m_vertices = new Vertex[m_vertexAmount];
	m_vertices[0].position = glm::vec3(-0.5f, 0.5f, 0.5f);
	m_vertices[1].position = glm::vec3(0.5f, 0.5f, 0.5f);
	m_vertices[2].position = glm::vec3(-0.5f, -0.5f, 0.5f);
	m_vertices[3].position = glm::vec3(0.5f, -0.5f, 0.5f);
	m_vertices[4].position = glm::vec3(0.5f, 0.5f, -0.5f);
	m_vertices[5].position = glm::vec3(-0.5f, 0.5f, -0.5f);
	m_vertices[6].position = glm::vec3(0.5f, -0.5f, -0.5f);
	m_vertices[7].position = glm::vec3(-0.5f, -0.5f, -0.5f);

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


Polygon::Polygon(int sides, glm::vec3 position, float scale) : Shape(position, scale)
{
	m_sides = sides;
	if (m_sides < 3)
		m_sides = 3;
	CreateMesh();
	LoadMesh();
}
void Polygon::CreateMesh()
{
	m_vertexAmount = m_sides + 1;	//All verts on the outside and the centre vert
	//Set the vertex positions
	m_vertices = new Vertex[m_vertexAmount];
	m_vertices[0].position = glm::vec3(0);	//Make the centre
	for (int i = 1; i < m_vertexAmount; ++i)
	{
		float currentAngle = glm::two_pi<float>() * ((float)(i - 1) / (float)m_sides);
		m_vertices[i].position = glm::vec3(glm::cos(currentAngle), 0, -glm::sin(currentAngle));	//Sin negative to make face-up
	}

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


Prism::Prism(int sides, float height, glm::vec3 position, float scale) : Shape(position, scale)
{
	m_sides = sides;
	if (m_sides < 3)
		m_sides = 3;
	m_height = height;
	CreateMesh();
	LoadMesh();
}
void Prism::CreateMesh()
{
	m_vertexAmount = 2 * (m_sides + 1);	//Cylinder verts is same as two polygons
	//Set the vertex positions
	m_vertices = new Vertex[m_vertexAmount];

	//Make the bottom polygon
	float heightOffset = m_height / 2;
	m_vertices[0].position = glm::vec3(0.0f, -heightOffset, 0.0f);
	for (int i = 1; i < (m_vertexAmount * 0.5f); ++i)
	{
		float currentAngle = glm::two_pi<float>() * ((float)(i - 1) / (float)m_sides);
		m_vertices[i].position = glm::vec3(glm::cos(currentAngle), -heightOffset, glm::sin(currentAngle));
	}
	//Make the top polygon
	m_vertices[(int)(m_vertexAmount * 0.5f)].position = glm::vec3(0.0f, heightOffset, 0.0f);
	for (int i = 1; i < (m_vertexAmount * 0.5f); ++i)
	{
		float currentAngle = glm::two_pi<float>() * ((float)(i - 1) / (float)m_sides);
		m_vertices[i + (int)(m_vertexAmount * 0.5f)].position = glm::vec3(glm::cos(currentAngle), heightOffset, glm::sin(currentAngle));
	}


	//Create the tris
	m_indexAmount = 12 * m_sides;
	m_indexBuffer = new int[m_indexAmount];

	//Index the bottom polygon
	int currentTri = 1;
	for (int i = 0; i < (m_sides * 3); ++i)
	{
		m_indexBuffer[i] = 0;
		++i;
		m_indexBuffer[i] = currentTri;
		++i;
		m_indexBuffer[i] = currentTri + 1;
		++currentTri;
	}
	m_indexBuffer[(int)(m_sides * 3) - 1] = 1;	//Link the final index along the circle back to the first

	//Index the top polygon
	int startingIndex = (int)(m_sides * 3);				//Offset the indexes we are writing
	int startingVertex = (int)(m_vertexAmount * 0.5f);	//Offset the vertices we are using
	currentTri = startingVertex + 1;
	for (int i = 0; i < (m_sides * 6); ++i)
	{
		m_indexBuffer[i + startingIndex] = startingVertex;
		++i;
		m_indexBuffer[i + startingIndex] = currentTri + 1;
		++i;
		m_indexBuffer[i + startingIndex] = currentTri;
		++currentTri;
	}
	m_indexBuffer[(m_sides * 6) - 2] = (int)(m_vertexAmount * 0.5f) + 1;	//Link the final index along the circle back to the first

	//Index the slice faces
	startingIndex = (int)(m_sides * 6);
	currentTri = 1;
	int sidesOffset = m_sides + 1;
	for (int i = startingIndex; i < m_indexAmount; ++i)
	{
		m_indexBuffer[i] = currentTri + 1;
		++i;
		m_indexBuffer[i] = currentTri;
		++i;
		m_indexBuffer[i] = currentTri + sidesOffset;
		++i;
		m_indexBuffer[i] = currentTri + sidesOffset;
		++i;
		m_indexBuffer[i] = currentTri + sidesOffset + 1;
		++i;
		m_indexBuffer[i] = currentTri + 1;
		++currentTri;
	}
	//Link the final face indexes back to the start
	m_indexBuffer[m_indexAmount - 6] = 1;
	m_indexBuffer[m_indexAmount - 2] = sidesOffset + 1;
	m_indexBuffer[m_indexAmount - 1] = 1;
}


Pyramid::Pyramid(int sides, float height, glm::vec3 position, float scale) : Shape(position, scale)
{
	m_sides = sides;
	if (m_sides < 3)
		m_sides = 3;
	m_height = height;
	CreateMesh();
	LoadMesh();
}
void Pyramid::CreateMesh()
{
	m_vertexAmount = m_sides + 2;	//Cylinder verts is same as two polygons
	//Set the vertex positions
	m_vertices = new Vertex[m_vertexAmount];

	//Make the bottom polygon
	float heightOffset = m_height / 2;
	m_vertices[0].position = glm::vec3(0.0f, -heightOffset, 0.0f);	//Centre of the base
	for (int i = 1; i < (m_vertexAmount - 1); ++i)
	{
		float currentAngle = glm::two_pi<float>() * ((float)(i - 1) / (float)m_sides);
		m_vertices[i].position = glm::vec3(glm::cos(currentAngle), -heightOffset, glm::sin(currentAngle));
	}
	m_vertices[m_vertexAmount - 1].position = glm::vec3(0.0f, heightOffset, 0.0f);	//Top of the cone


	//Create the tris
	m_indexAmount = 6 * m_sides;
	m_indexBuffer = new int[m_indexAmount];

	//Index the bottom polygon
	int currentTri = 1;
	for (int i = 0; i < (m_sides * 3); ++i)
	{
		m_indexBuffer[i] = 0;
		++i;
		m_indexBuffer[i] = currentTri;
		++i;
		m_indexBuffer[i] = currentTri + 1;
		++currentTri;
	}
	m_indexBuffer[(int)(m_sides * 3) - 1] = 1;	//Link the final index along the circle back to the first

	//Index the top polygon
	int startingIndex = (int)(m_sides * 3);				//Offset the indexes we are writing
	currentTri = 1;
	for (int i = 0; i < (m_sides * 6); ++i)
	{
		m_indexBuffer[i + startingIndex] = (m_vertexAmount - 1);
		++i;
		m_indexBuffer[i + startingIndex] = currentTri + 1;
		++i;
		m_indexBuffer[i + startingIndex] = currentTri;
		++currentTri;
	}
	m_indexBuffer[(m_sides * 6) - 2] = 1;	//Link the final index along the circle back to the first
}