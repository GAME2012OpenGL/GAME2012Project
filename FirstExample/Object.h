#ifndef __OBJECT__
#define __OBJECT__

#include "GL/glew.h"
#include "glm\glm.hpp"

class Mesh;

class Object
{
protected :
	Mesh* m_pMesh;
	GLuint m_iTexture;

	glm::mat4 m_WorldMatrix;
	glm::vec3 m_Position;
	glm::vec3 m_RotationAxis;
	glm::vec3 m_Scale;

	float m_fAngle;
	float m_fShininess;

	GLuint m_uniformWorld;
	GLuint m_uniformShininess;

public :
	Object(GLuint uniformWorld, GLuint uniformShininess, float fShininess);
	~Object();

	void Update();
	virtual void Render();
	void Clear();






public :
	void SetMesh(Mesh* pMesh) { m_pMesh = pMesh; }
	void SetTexture(GLuint iTexture) { m_iTexture = iTexture; }
	void SetPosition(float X, float Y, float Z) { m_Position = glm::vec3(X, Y, Z); }
	void SetRotation(float X, float Y, float Z, float fAngle) { m_RotationAxis = glm::vec3(X, Y, Z); m_fAngle = fAngle; }
	void SetScale(float X, float Y, float Z) { m_Scale = glm::vec3(X, Y, Z); }

};

#endif

