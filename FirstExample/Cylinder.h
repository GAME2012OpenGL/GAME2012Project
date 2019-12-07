#pragma once
#ifndef __CYLINDER__
#define __CYLINDER__

#include "Mesh.h"

class Cylinder : public Mesh
{
public:
	Cylinder();
	~Cylinder();

	void CreateMesh();
};

#endif