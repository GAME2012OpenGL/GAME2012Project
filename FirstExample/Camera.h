#ifndef __CAMERA__
#define __CAMERA__

#include "GL/glew.h"
#include "glm\glm.hpp"

class Camera
{
public:
	Camera(glm::vec3 startPos, glm::vec3 WorldUp, GLfloat fYaw, GLfloat fPitch, GLfloat fMoveSpeed, GLfloat fTurnSpeed);
	~Camera();

	void KeyControl(unsigned char key, GLfloat fDeltaTime);
	void MouseControl(GLfloat fXChange, GLfloat fYChange);


	

	glm::mat4 CalculateViewMatrix();


	glm::vec3 GetCameraPosition() { return m_position; }
	void SetCameraPosition(glm::vec3 position) { m_position = position; }
	glm::vec3 GetFrontVector() const { return m_front; }
	bool GetMouseFirstMoved() const { return m_bMouseFirstMoved; }
	void SetMouseFirstMoved(bool bMouseFirstMoved) { m_bMouseFirstMoved = bMouseFirstMoved; }
	int GetLastMouseX() const { return m_iLastMouseX; }
	void SetLastMouseX(int iLastMouseX) { m_iLastMouseX = iLastMouseX; }
	int GetLastMouseY() const { return m_iLastMouseY; }
	void SetLastMouseY(int iLastMouseY) { m_iLastMouseY = iLastMouseY; }


private:
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;

	GLfloat m_fYaw;
	GLfloat m_fPitch;

	GLfloat m_fMovementSpeed;
	GLfloat m_fTurnSpeed;

	bool m_bMouseFirstMoved;

	int m_iLastMouseX;
	int m_iLastMouseY;
	int m_iMouseXChange;
	int m_iMouseYChange;

private:
	void update();
};

#endif