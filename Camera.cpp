#include "Camera.h"

Camera::Camera()
{
	//The projection matrix, representing the lens of the camera, created with FOV, screen aspect ratio, and near and far of the frustum
	m_projectionTransform = glm::perspective(1.507f, 16.0f / 9.0f, 0.1f, 100.0f);
	//View matrix, the inverse of the camera's world transform
	m_viewTransform = glm::lookAt(glm::vec3(0, 1, 2), glm::vec3(0), glm::vec3(0, 1, 0));
	//The global transform, set to the identity matrix
	m_worldTransform = glm::inverse(m_viewTransform);

	m_PVTransform = m_projectionTransform * m_viewTransform;

	m_pWindow = glfwGetCurrentContext();

	m_moveSpeed = 3.0f;
	m_rotateSpeed = 0.1f;

	//Stop the camera from immediately rotating
	double mouse_x = 0.0f;
	double mouse_y = 0.0f;
	glfwGetCursorPos(m_pWindow, &mouse_x, &mouse_y);
	m_prevMouseX = mouse_x;
	m_prevMouseY = mouse_y;
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

	m_moveSpeed = 3.0f;
	m_rotateSpeed = 0.1f;

	//Stop the camera from immediately rotating
	double mouse_x = 0.0f;
	double mouse_y = 0.0f;
	glfwGetCursorPos(m_pWindow, &mouse_x, &mouse_y);
	m_prevMouseX = mouse_x;
	m_prevMouseY = mouse_y;
}

void Camera::Update(float deltaTime)
{
	//MOVEMENT
	bool input_flag = false;

	glm::vec4 displacement = glm::vec4(0);
	if (glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS)		//Forward
	{
		displacement -= m_worldTransform[2];
		input_flag = true;
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS)		//Backward
	{
		displacement += m_worldTransform[2];
		input_flag = true;
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS)		//Left
	{
		displacement -= m_worldTransform[0];
		input_flag = true;
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS)		//Right
	{
		displacement += m_worldTransform[0];
		input_flag = true;
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_SPACE) == GLFW_PRESS)		//Up
	{
		displacement += glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		input_flag = true;
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)		//Down
	{
		displacement -= glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		input_flag = true;
	}

	if (input_flag)
	{
		m_worldTransform[3] = m_worldTransform[3] + displacement * m_moveSpeed * deltaTime;
		m_viewTransform = glm::inverse(m_worldTransform);
		m_PVTransform = m_projectionTransform * m_viewTransform;
	}


	//ROTATION
	double mouse_x = 0.0f;
	double mouse_y = 0.0f;
	glfwGetCursorPos(m_pWindow, &mouse_x, &mouse_y);	//Make sure the mouse is set to disabled mode!
	double mouse_delta_x = mouse_x - m_prevMouseX;
	double mouse_delta_y = mouse_y - m_prevMouseY;
	m_prevMouseX = mouse_x;
	m_prevMouseY = mouse_y;
	if (mouse_delta_x || mouse_delta_y)
	{
		glm::mat4 rotation = glm::mat4(1.0f);
		//Rotation around the y axis
		rotation *= glm::rotate(rotation, float(-mouse_delta_x * deltaTime * m_rotateSpeed), glm::vec3(m_viewTransform[1]));
		//Rotation around the x axis
		rotation *= glm::rotate(rotation, float(-mouse_delta_y * deltaTime * m_rotateSpeed), glm::vec3(1.0f, 0.0f, 0.0f));

		m_worldTransform = m_worldTransform * rotation;			//Set the camera rotation
		m_viewTransform = glm::inverse(m_worldTransform);			//Update the view matrix
		m_PVTransform = m_projectionTransform * m_viewTransform;	//Update the PV matrix
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