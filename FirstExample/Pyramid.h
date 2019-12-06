#pragma once
#ifndef __CUBE__
#define __CUBE__

#include "Mesh.h"

class Pyramid : public Mesh
{
public:
	Pyramid();
	~Pyramid();

	void CreateMesh();

	//void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
	//void RenderMesh();
	//void ClearMesh();
};

#endif