#include "Camera.h"

#define MOVE_SPEED_DEFAULT 40.0f
#define ROTATE_SPEED_DEFAULT 0.1f

Camera::Camera()
{
	//The projection matrix, representing the lens of the camera, created with FOV, screen aspect ratio, and near and far of the frustum
	m_projectionTransform = glm::perspective(1.507f, 16.0f / 9.0f, 0.1f, 10000.0f);
	//View matrix, the inverse of the camera's world transform
	m_viewTransform = glm::lookAt(glm::vec3(0, 1, 2), glm::vec3(0), glm::vec3(0, 1, 0));
	//The global transform, set to the identity matrix
	m_worldTransform = glm::inverse(m_viewTransform);

	m_PVTransform = m_projectionTransform * m_viewTransform;

	m_pWindow = glfwGetCurrentContext();

	m_moveSpeed = MOVE_SPEED_DEFAULT;
	m_rotateSpeed = ROTATE_SPEED_DEFAULT;

	//Stop the camera from immediately rotating
	double mouseX = 0.0f;
	double mouseY = 0.0f;
	glfwGetCursorPos(m_pWindow, &mouseX, &mouseY);
	m_prevMouseX = mouseX;
	m_prevMouseY = mouseY;
}

Camera::Camera(glm::vec3 position, glm::vec3 pointToLookAt)
{
	//The projection matrix, representing the lens of the camera, created with FOV, screen aspect ratio, and near and far of the frustum
	m_projectionTransform = glm::perspective(1.507f, 16.0f / 9.0f, 0.1f, 100.0f);
	//View matrix, the inverse of the camera's world transform
	m_viewTransform = glm::lookAt(position, pointToLookAt, glm::vec3(0, 1, 0));
	//The global transform, set to the identity matrix
	m_worldTransform = glm::inverse(m_viewTransform);

	m_PVTransform = m_projectionTransform * m_viewTransform;

	m_pWindow = glfwGetCurrentContext();

	m_moveSpeed = MOVE_SPEED_DEFAULT;
	m_rotateSpeed = ROTATE_SPEED_DEFAULT;

	//Stop the camera from immediately rotating
	double mouseX = 0.0f;
	double mouseY = 0.0f;
	glfwGetCursorPos(m_pWindow, &mouseX, &mouseY);
	m_prevMouseX = mouseX;
	m_prevMouseY = mouseY;
}

void Camera::Update(float deltaTime)
{
	//MOVEMENT
	bool inputFlag = false;

	glm::vec4 displacement = glm::vec4(0);
	if (glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS)			//Forward
	{
		displacement -= m_worldTransform[2];
		inputFlag = true;
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS)			//Backward
	{
		displacement += m_worldTransform[2];
		inputFlag = true;
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS)			//Left
	{
		displacement -= m_worldTransform[0];
		inputFlag = true;
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS)			//Right
	{
		displacement += m_worldTransform[0];
		inputFlag = true;
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_SPACE) == GLFW_PRESS)		//Up
	{
		displacement += glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		inputFlag = true;
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)	//Down
	{
		displacement -= glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		inputFlag = true;
	}

	if (inputFlag)		//If keyboard input was detected,
	{
		//Move the transform of the camera
		if (glfwGetKey(m_pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)									//If holding shift,
			m_worldTransform[3] = m_worldTransform[3] + displacement * m_moveSpeed * deltaTime * 0.4f;	//Slow movement
		else																							//Otherwise,
			m_worldTransform[3] = m_worldTransform[3] + displacement * m_moveSpeed * deltaTime;			//Normal speed
		
		//Update the other transforms
		m_viewTransform = glm::inverse(m_worldTransform);
		m_PVTransform = m_projectionTransform * m_viewTransform;
	}


	//ROTATION
	if (glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && m_mouseHeld == false)
	{
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	//Hide the mouse cursor in the window so we can look around with the camera
		glfwSetInputMode(m_pWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

		//Stop the camera from immediately rotating
		double mouseX = 0.0f;
		double mouseY = 0.0f;
		glfwGetCursorPos(m_pWindow, &mouseX, &mouseY);
		m_prevMouseX = mouseX;
		m_prevMouseY = mouseY;

		m_mouseHeld = true;
	}
	else if (glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && m_mouseHeld == true)
	{
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);	//Make the cursor controllable again
		glfwSetInputMode(m_pWindow, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);

		m_mouseHeld = false;
	}

	if (glfwGetInputMode(m_pWindow, GLFW_RAW_MOUSE_MOTION))	//If the mouse is in camera control mode,
	{
		double mouseX = 0.0f;
		double mouseY = 0.0f;
		glfwGetCursorPos(m_pWindow, &mouseX, &mouseY);	//Make sure the mouse is set to disabled mode!
		//Calculate how far the mouse has travelled this frame
		double mouseDeltaX = mouseX - m_prevMouseX;
		double mouseDeltaY = mouseY - m_prevMouseY;
		m_prevMouseX = mouseX;
		m_prevMouseY = mouseY;

		if (mouseDeltaX || mouseDeltaY)		//If mouse input was detected,
		{
			glm::mat4 rotation = glm::mat4(1.0f);
			//Rotation around the y axis
			rotation *= glm::rotate(rotation, float(-mouseDeltaX * deltaTime * m_rotateSpeed), glm::vec3(m_viewTransform[1]));
			//Rotation around the x axis
			rotation *= glm::rotate(rotation, float(-mouseDeltaY * deltaTime * m_rotateSpeed), glm::vec3(1.0f, 0.0f, 0.0f));

			m_worldTransform = m_worldTransform * rotation;				//Set the camera rotation
			m_viewTransform = glm::inverse(m_worldTransform);			//Update the view matrix
			m_PVTransform = m_projectionTransform * m_viewTransform;	//Update the PV matrix
		}
	}
}

glm::mat4 Camera::GetPV()
{
	return m_PVTransform;
}

void Camera::SetMoveSpeed(float speed)
{
	m_moveSpeed = speed;
}

void Camera::SetRotateSpeed(float speed)
{
	m_rotateSpeed = speed;
}