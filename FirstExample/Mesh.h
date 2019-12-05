#pragma once
#ifndef __MESH__
#define __MESH__

#include "GL/glew.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void CreateMesh(GLfloat* vertices, GLshort* indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void calcAverageNormals(GLshort* indices, unsigned int indiceCount,
		GLfloat* vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset);
	void RenderMesh();
	void ClearMesh();

protected:
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_IBO;
	GLuint m_indexCount;
	GLuint m_vertexCount;
	GLuint m_vertexLength;
};

#endif