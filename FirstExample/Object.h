#ifndef __OBJECT__
#define __OBJECT__

#include "GL/glew.h"
#include "glm\glm.hpp"

class Mesh;

class Object
{
private :
	Mesh* m_pMesh;
	GLuint m_iTexture;

	glm::mat4 m_WorldMatrix;
	glm::vec3 m_Position;
	glm::vec3 m_RotationAxis;
	glm::vec3 m_Scale;

	float m_fAngle;

	GLuint m_uniformWorld;

public :
	Object(GLuint uniformWorld);
	~Object();

	void Update();
	void Render();
	void Clear();






public :
	void SetMesh(Mesh* pMesh) { m_pMesh = pMesh; }
	void SetTexture(GLuint iTexture) { m_iTexture = iTexture; }
	void SetPosition(float X, float Y, float Z) { m_Position = glm::vec3(X, Y, Z); }

};

#endif

