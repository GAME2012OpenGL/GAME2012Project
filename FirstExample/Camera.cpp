#include "Camera.h"

#include "glm\gtc\matrix_transform.hpp"

Camera::Camera(glm::vec3 startPos, glm::vec3 WorldUp, GLfloat fYaw, GLfloat fPitch, GLfloat fMoveSpeed, GLfloat fTurnSpeed)
{
	m_position = startPos;
	m_up = glm::vec3(0.f, 1.f, 0.f);
	m_worldUp = WorldUp;
	m_fYaw = fYaw;
	m_fPitch = fPitch;

	m_front = glm::vec3(0.f, 0.f, -1.f);

	m_fMovementSpeed = fMoveSpeed;
	m_fTurnSpeed = fTurnSpeed;

	m_bMouseFirstMoved = true;

	m_iLastMouseX = 0;
	m_iLastMouseY = 0;
	m_iMouseXChange = 0;
	m_iMouseYChange = 0;

	update();
}

Camera::~Camera()
{

}

void Camera::KeyControl(unsigned char key, GLfloat fDeltaTime)
{
	switch (key)
	{
	case 'w':
	case 'W':
		m_position += m_front * m_fMovementSpeed * fDeltaTime;
		break;
	case 's':
	case 'S':
		m_position -= m_front * m_fMovementSpeed * fDeltaTime;
		break;
	case 'a':
	case 'A':
		m_position -= m_right * m_fMovementSpeed * fDeltaTime;
		break;
	case 'd':
	case 'D':
		m_position += m_right * m_fMovementSpeed * fDeltaTime;
		break;
	}
}

void Camera::MouseControl(GLfloat fXChange, GLfloat fYChange)
{
	fXChange *= m_fTurnSpeed;
	fYChange *= m_fTurnSpeed;

	m_fYaw += fXChange;
	m_fPitch += fYChange;

	if (m_fPitch >= 89.f)
	{
		m_fPitch = 89.f;
	}
	if (m_fPitch <= -89.f)
	{
		m_fPitch = -89.f;
	}

	update();
}

glm::mat4 Camera::CalculateViewMatrix()
{
	return glm::lookAt(m_position, m_position + m_front, m_worldUp);
}

void Camera::update()
{
	m_front.x = -sin(glm::radians(m_fYaw)) * cos(glm::radians(m_fPitch));
	m_front.y = sin(glm::radians(m_fPitch));
	m_front.z = -cos(glm::radians(m_fYaw)) * cos(glm::radians(m_fPitch));

	m_front = glm::normalize(m_front);

	m_right = glm::normalize(glm::cross(m_front, m_worldUp));

	m_up = glm::normalize(glm::cross(m_right, m_front));
}