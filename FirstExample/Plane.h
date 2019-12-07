#ifndef __PLANE__
#define __PLANE__

#include "Mesh.h"

class Plane : public Mesh
{
public:
	Plane();
	~Plane();

	void CreateMesh(GLuint iNumOfGrid);
};

#endif