#pragma once
#ifndef __PYRAMIDKINDOF__
#define __PYRAMIDKINDOF__

#include "Mesh.h"

class PyramidKindOf : public Mesh
{
public:
	PyramidKindOf();
	~PyramidKindOf();

	void CreateMesh();

	//void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
	//void RenderMesh();
	//void ClearMesh();
};

#endif