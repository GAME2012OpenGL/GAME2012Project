#pragma once
#ifndef __Octahedron__
#define __Octahedron__

#include "Mesh.h"

class Octahedron : public Mesh
{
public:
	Octahedron();
	~Octahedron();

	void CreateMesh();
};

#endif // !__Octahedron__