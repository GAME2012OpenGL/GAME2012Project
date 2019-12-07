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
};

#endif