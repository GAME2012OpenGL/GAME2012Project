#include "Trapezoid.h"

Trapezoid::Trapezoid()
{
}

Trapezoid::~Trapezoid()
{
}

void Trapezoid::CreateMesh()
{
	GLfloat Trapezoid_vertices[] =
	{
		//Front
		//x,y,z				u,v			nx,ny,nz
		0.f,0.f,1.f,		0,0,		0,0,0,
		2.f,0.f,1.f,		1,0,		0,0,0,
		2.f,4.f,1.f,		1,1,		0,0,0,
		0.f,3.f,1.f,		1,0,		0,0,0,

		2.f,0.f,1.f,		0,0,		0,0,0,
		2.f,0.f,0.f,		1,0,		0,0,0,
		2.f,4.f,0.f,		1,1,		0,0,0,
		2.f,4.f,1.f,		0,1,		0,0,0,

		
		0.f,0.f,1.f,		0,0,		0,0,0,
		0.f,0.f,0.f,		1,0,		0,0,0,
		2.f,0.f,0.f,		1,1,		0,0,0,
		2.f,0.f,1.f,		0,1,		0,0,0,

		
		0.f,0.f,0.f,		0,0,		0,0,0,
		0.f,0.f,1.f,		1,0,		0,0,0,
		0.f,3.f,1.f,		1,1,		0,0,0,
		0.f,3.f,0.f,		0,1,		0,0,0,

		
		0.f,3.f,0.f,		0,0,		0,0,0,
		0.f,3.f,1.f,		1,0,		0,0,0,
		2.f,4.f,1.f,		1,1,		0,0,0,
		2.f,4.f,0.f,		0,1,		0,0,0,

		
		2.f,0.f,0.f,		0,0,		0,0,0,
		0.f,0.f,0.f,		1,0,		0,0,0,
		0.f,3.f,0.f,		1,1,		0,0,0,
		2.f,4.f,0.f,		0,1,		0,0,0
	};

	GLshort Trapezoid_indices[] =
	{
		0,1,2,
		0,2,3,

		4,5,6,
		4,6,7,

		8,9,10,
		8,10,11,

		12,13,14,
		12,14,15,

		16,17,18,
		16,18,19,

		20,21,22,
		20,22,23
	};

	m_indexCount = sizeof(Trapezoid_indices) / sizeof(GLshort);
	m_vertexCount = sizeof(Trapezoid_vertices) / (sizeof(GLfloat) * m_vertexLength);
	Mesh::calcAverageNormals(Trapezoid_indices, m_indexCount, Trapezoid_vertices, m_vertexCount, m_vertexLength, 5);

	Mesh::CreateMesh(Trapezoid_vertices, Trapezoid_indices, m_vertexCount, m_indexCount);
}