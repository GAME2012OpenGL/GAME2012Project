#pragma once
#ifndef __CONE__
#define __CONE__

#include "Mesh.h"

class Cone : public Mesh
{
public:
	Cone();
	~Cone();

	void CreateMesh();
};

#endif