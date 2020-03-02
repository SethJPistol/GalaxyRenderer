#include "Sprite.h"

#include "..\glcore\gl_core_4_5.h"
#include "ext.hpp"

//For textures
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace glxy;

Sprite::Sprite(glm::vec3 position, float scale)
{
	CreateMesh();
	LoadMesh();

	m_localTransform = glm::mat4(1);
	m_scale = 1;
	SetPosition(position);
	SetScale(scale);
}

Sprite::~Sprite()
{
	//Clearing memory
	glDeleteBuffers(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteTextures(1, &m_texture);

	delete[] m_vertices;
	delete[] m_indexBuffer;
}

void Sprite::LoadTexture(const char* texturePath)
{
	int x, y, n;
	unsigned char* texData = stbi_load(texturePath, &x, &y, &n, 0);

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	if (n == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	if (n == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);

	//Generate the mipmaps so the texture can be sampled
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // GL_LINEAR SAMPLES texels
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // GL_NEARESTS RETURNS just closest pixel

	stbi_image_free(texData);
}
void Sprite::LoadTexture(unsigned int textureHandle)
{
	m_texture = textureHandle;
}

void Sprite::Draw()
{
	glActiveTexture(GL_TEXTURE0);
	if (m_texture != 0)
		glBindTexture(GL_TEXTURE_2D, m_texture);

	//Get the shader ID to use
	int shaderProgramID = -1;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgramID);
	if (shaderProgramID == -1)	//If none was found,
	{
		printf("No shader program bound when trying to draw Sprite\n");
		return;
	}

	//Feed through the global transform to the vertex shader
	auto uniform_location = glGetUniformLocation(shaderProgramID, "model_matrix");
	glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(m_localTransform));

	//Binding and drawing arrays
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indexAmount, GL_UNSIGNED_INT, 0);	//Used for drawing using the IBO

	if (m_texture != 0)
		glBindTexture(GL_TEXTURE_2D, 0);	//Unbind the texture so that other things aren't rendered with it
}

glm::vec3 Sprite::GetPosition()
{
	return (glm::vec3)m_localTransform[3];
}

void Sprite::SetPosition(glm::vec3 position)
{
	m_localTransform[3] = glm::vec4(position, 1.0f);
}

float Sprite::GetScale()
{
	return m_scale;
}

void Sprite::SetScale(float scale)
{
	m_localTransform[0][0] /= m_scale;
	m_localTransform[0][0] *= scale;
	m_localTransform[1][1] /= m_scale;
	m_localTransform[1][1] *= scale;
	m_localTransform[2][2] /= m_scale;
	m_localTransform[2][2] *= scale;
}

unsigned int Sprite::GetVAO()
{
	return 0;
}

void Sprite::CreateMesh()
{
	m_vertexAmount = 4;
	m_vertices = new Vertex[m_vertexAmount];

	//Set the vertex positions
	m_vertices[0].position = glm::vec3(-0.5f, 0.5f, 0.0f);
	m_vertices[1].position = glm::vec3(0.5f, 0.5f, 0.0f);
	m_vertices[2].position = glm::vec3(-0.5f, -0.5f, 0.0f);
	m_vertices[3].position = glm::vec3(0.5f, -0.5f, 0.0f);

	//Set the vertex UVs
	m_vertices[0].UV = glm::vec2(0.0f, 1.0f);
	m_vertices[1].UV = glm::vec2(1.0f, 1.0f);
	m_vertices[2].UV = glm::vec2(0.0f, 0.0f);
	m_vertices[3].UV = glm::vec2(1.0f, 0.0f);

	//Create the tris
	m_indexAmount = 6;
	m_indexBuffer = new int[m_indexAmount]
	{
		0, 2, 1, 1, 2, 3
	};
}

void Sprite::LoadMesh()
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
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));

	//Unbind the array and buffers for safety
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}