#pragma once

#include "glm.hpp"
#include "glfw3.h"
#include "ext.hpp"


namespace glxy
{
	class Camera
	{
	public:

		Camera();
		Camera(glm::vec3 position, glm::vec3 pointToLookAt);

		void Update(float deltaTime);

		glm::mat4 GetPV();
		glm::vec3 GetPosition();
		void SetMoveSpeed(float speed);
		void SetRotateSpeed(float speed);


	private:

		glm::mat4 m_worldTransform;
		glm::mat4 m_viewTransform;
		glm::mat4 m_projectionTransform;
		glm::mat4 m_PVTransform;

		GLFWwindow* m_pWindow;

		float m_moveSpeed;
		float m_rotateSpeed;

		double m_prevMouseX, m_prevMouseY;

		bool m_mouseHeld = false;
	};
}