#pragma once

namespace glxy
{
	class RenderTarget
	{
	public:

		RenderTarget(unsigned int targetCount, unsigned int width, unsigned int height);
		virtual ~RenderTarget();

		void Bind();
		void Unbind();

		unsigned int GetTarget(unsigned int index);


	private:

		unsigned int CreateTexture(unsigned int width, unsigned int height);

		unsigned int	m_width;
		unsigned int	m_height;

		unsigned int	m_fbo;	//Frame buffer object
		unsigned int	m_rbo;	//Render buffer object

		unsigned int	m_targetCount;
		unsigned int*	m_targets;		//Texture handles
	};
}