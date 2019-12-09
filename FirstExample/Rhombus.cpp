#include "Rhombus.h"

Rhombus::Rhombus()
{
}

Rhombus::~Rhombus()
{
}

void Rhombus::CreateMesh()
{
	GLfloat Rhombus_vertices[] =
	{
		//Front
		//x,y,z				u,v			nx,ny,nz
		-2.f,0.f,1.f,		0,0.5,		0,0,0,
		0.f,-1.f,1.f,		0.5,0.25,	0,0,0,
		2.f,0.f,1.f,		1,0.5,		0,0,0,
		0.f,1.f,1.f,		0.5,0.75,	0,0,0,

		//Top Left
		-2.f,0.f,-1.f,		0,0,		0,0,0,
		-2.f,0.f,1.f,		1,0,		0,0,0,
		0.f,1.f,1.f,		1,1,		0,0,0,
		0.f,1.f,-1.f,		0,1,		0,0,0,

		//Bottom Left
		0.f,1.f,-1.f,		0,0,	0,0,0,
		0.f,-1.f,1.f,		1,0,	0,0,0,
		-2.f,0.f,1.f,		1,1,	0,0,0,
		-2.f,0.f,-1.f,		0,1,	0,0,0,
		

		//Top Right
		2.f,0.f,1.f,		0,0,	0,0,0,
		2.f,0.f,-1.f,		1,0,	0,0,0,
		0.f,1.f,-1.f,		1,1,	0,0,0,
		0.f,1.f,1.f,		0,1,	0,0,0,

		//Bottom Right
		0.f,-1.f,1.f,		0,0,	0,0,0,
		2.f,0.f,1.f,		1,0,	0,0,0,
		2.f,0.f,-1.f,		1,1,	0,0,0,
		0.f,-1.f,-1.f,		0,1,	0,0,0,

		//Back
		-2.f,0.f,-1.f,		0,0.5,		0,0,0,
		0.f,-1.f,-1.f,		0.5,0.25,	0,0,0,
		2.f,0.f,-1.f,		1,0.5,		0,0,0,
		0.f,1.f,-1.f,		0.5,0.75,	0,0,0
	};

	GLshort Rhombus_indices[] =
	{
		//front
		0,1,3,
		1,2,3,

		//TL
		4,5,6,
		4,6,7,

		//BL
		8,9,10,
		8,10,11,

		//TR
		12,13,14,
		12,14,15,

		//BR
		16,17,18,
		16,18,19,

		//back
		23,21,20,
		23,22,21
	};

	m_indexCount = sizeof(Rhombus_indices) / sizeof(GLshort);
	m_vertexCount = sizeof(Rhombus_vertices) / (sizeof(GLfloat) * m_vertexLength);
	Mesh::calcAverageNormals(Rhombus_indices, m_indexCount, Rhombus_vertices, m_vertexCount, m_vertexLength, 5);

	Mesh::CreateMesh(Rhombus_vertices, Rhombus_indices, m_vertexCount, m_indexCount);
}
