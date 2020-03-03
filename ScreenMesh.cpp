#include "ScreenMesh.h"

#include "..\glcore\gl_core_4_5.h"
#include "ext.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace glxy;

ScreenMesh::ScreenMesh()
{
	CreateMesh();
	LoadMesh();
}
ScreenMesh::ScreenMesh(unsigned int textureHandle)
{
	CreateMesh();
	LoadMesh();
	LoadTexture(textureHandle);
}
ScreenMesh::~ScreenMesh()
{
	//Clearing memory
	glDeleteBuffers(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteTextures(1, &m_screenTexture);

	delete[] m_vertices;
	delete[] m_indexBuffer;
}

void ScreenMesh::LoadTexture(unsigned int textureHandle)
{
	m_screenTexture = textureHandle;
}

void ScreenMesh::Draw()
{
	glActiveTexture(GL_TEXTURE0);
	if (m_screenTexture != 0)
		glBindTexture(GL_TEXTURE_2D, m_screenTexture);

	//Get the shader ID to use
	int shaderProgramID = -1;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgramID);
	if (shaderProgramID == -1)	//If none was found,
	{
		printf("No shader program bound when trying to draw ScreenMesh\n");
		return;
	}

	//Binding and drawing arrays
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indexAmount, GL_UNSIGNED_INT, 0);	//Used for drawing using the IBO

	if (m_screenTexture != 0)
		glBindTexture(GL_TEXTURE_2D, 0);	//Unbind the texture so that other things aren't rendered with it
}

void ScreenMesh::CreateMesh()
{
	m_vertexAmount = 4;
	m_vertices = new Vertex[m_vertexAmount];

	//Set the vertex positions
	m_vertices[0].position = glm::vec2(-1, 1);
	m_vertices[1].position = glm::vec2(1, 1);
	m_vertices[2].position = glm::vec2(-1, -1);
	m_vertices[3].position = glm::vec2(1, -1);

	//Create the tris
	m_indexAmount = 6;
	m_indexBuffer = new int[m_indexAmount]
	{
		0, 2, 1, 1, 2, 3
	};
}

void ScreenMesh::LoadMesh()
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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	//Unbind the array and buffers for safety
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}