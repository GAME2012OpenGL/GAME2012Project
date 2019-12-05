#include "Object.h"

#include "Mesh.h"
#include "glm\gtc\matrix_transform.hpp"

Object::Object(GLuint uniformWorld)
	:m_pMesh(nullptr), m_fAngle(0.f), m_uniformWorld(uniformWorld)
{
	m_WorldMatrix = glm::mat4(1.f);
	m_Position = glm::vec3(0.f, 0.f, 0.f);
	m_RotationAxis = glm::vec3(0.f, 1.f, 0.f);
	m_Scale = glm::vec3(1.f, 1.f, 1.f);

	
}

Object::~Object()
{

}

void Object::Update()
{
	m_WorldMatrix = glm::mat4(1.0f);
	m_WorldMatrix = glm::translate(m_WorldMatrix, m_Position);
	m_WorldMatrix = glm::rotate(m_WorldMatrix, glm::radians(m_fAngle), m_RotationAxis);
	m_WorldMatrix = glm::scale(m_WorldMatrix, m_Scale);
}

void Object::Render()
{
	glUniformMatrix4fv(m_uniformWorld, 1, GL_FALSE, &m_WorldMatrix[0][0]);
	m_pMesh->RenderMesh();
}

void Object::Clear()
{

}
