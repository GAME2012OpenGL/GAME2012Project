#pragma once
#ifndef __CUBE__
#define __CUBE__

#include "Mesh.h"

class Cube : public Mesh
{
public:
	Cube();
	~Cube();

	void CreateMesh();

	//void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
	//void RenderMesh();
	//void ClearMesh();
};

#endif