#include "RenderTarget.h"

#include "..\glcore\gl_core_4_5.h"
#include <vector>

using namespace glxy;

RenderTarget::RenderTarget(unsigned int targetCount, unsigned int width, unsigned int height) 
{
	m_targetCount = targetCount;
	m_width = width;
	m_height = height;
	m_targets = nullptr;
	m_rbo = 0;
	

	//Generate and bind the frame buffer object
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	//Generate and bind a 24-bit depth buffer as a render buffer object
	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

	//Create and attach textures
	if (targetCount > 0) 
	{
		m_targets = new unsigned int[targetCount];

		std::vector<GLenum> drawBuffers = {};

		for (unsigned int i = 0; i < targetCount; ++i) 
		{
			m_targets[i] = CreateTexture(width, height);

			drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);

			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, m_targets[i], 0);
		}

		glDrawBuffers((GLsizei)drawBuffers.size(), drawBuffers.data());
	}

	//Check if it failed
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
	{
		//Cleanup
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		delete[] m_targets;
		m_targets = nullptr;

		glDeleteRenderbuffers(1, &m_rbo);
		glDeleteFramebuffers(1, &m_fbo);
		m_rbo = 0;
		m_fbo = 0;

		printf("Error - Framebuffer failed to compile\n");
		return;
	}

	//Success!
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_targetCount = targetCount;
	m_width = width;
	m_height = height;
}
RenderTarget::~RenderTarget() 
{
	delete[] m_targets;
	glDeleteRenderbuffers(1, &m_rbo);
	glDeleteFramebuffers(1, &m_fbo);
}

void RenderTarget::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}
void RenderTarget::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int RenderTarget::GetTarget(unsigned int index)
{
	return m_targets[index];
}


unsigned int RenderTarget::CreateTexture(unsigned int width, unsigned int height)
{
	unsigned int texture = 0;

	//Generate the texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//Generate the mipmaps so the texture can be sampled
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Create the texture, with no actual pixels yet
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	//Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}