#pragma once
#ifndef __Trapezoid__
#define __Trapezoid__

#include "Mesh.h"
class Trapezoid : public Mesh
{
public:
	Trapezoid();
	~Trapezoid();

	void CreateMesh();
};

#endif // !__Trapezoid__