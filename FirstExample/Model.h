#pragma once
#ifndef __MESH__
#define __MESH__

#include "GL/glew.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderMesh();
	void ClearMesh();

private:
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_IBO;
	GLsizei m_indexCount;
};

#endif