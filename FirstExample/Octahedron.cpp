#include <cmath>
#include "Octahedron.h"

Octahedron::Octahedron()
{
}

Octahedron::~Octahedron()
{
}

void Octahedron::CreateMesh()
{
	GLfloat Octahedron_vertices[] =
	{
		//Front Left Up
		-2.f,0.f,0.f,		0,0,			0,0,0,
		0.f,0.f,1.f,		1,0,			0,0,0,
		0.f,4.f,0.f,		0.2, 2/pow(6,1/2)*5, 0,0,0,

		//Front Right Up
		0.f,0.f,1.f,		0,0,				0,0,0,
		2.f,0.f,0.f,		1,0,				0,0,0,
		0.f,4.f,0.f,		0.2, 2 / pow(6,1 / 2) * 5, 0,0,0,

		//Back Left Up
		0.f,0.f,-1.f,		0,0,				0,0,0,
		-2.f,0.f,0.f,		1,0,				0,0,0,
		0.f,4.f,0.f,		0.2, 2 / pow(6,1 / 2) * 5, 0,0,0,

		//Back Right Up
		2.f,0.f,0.f,		0,0,				0,0,0,
		0.f,0.f,-1.f,		1,0,				0,0,0,
		0.f,4.f,0.f,		0.2, 2 / pow(6,1 / 2) * 5, 0,0,0,

		//Front Left Down
		-2.f,0.f,0.f,		0,0,			0,0,0,
		0.f,-4.f,0.f,		0.2, 2 / pow(6,1 / 2) * 5, 0,0,0,
		0.f,0.f,1.f,		1,0,			0,0,0,

		//Front Right Down
		0.f,0.f,1.f,		0,0,				0,0,0,
		0.f,-4.f,0.f,		0.2, 2 / pow(6,1 / 2) * 5, 0,0,0,
		2.f,0.f,0.f,		1,0,				0,0,0,

		//Back Left Down
		0.f,0.f,-1.f,		0,0,				0,0,0,
		0.f,-4.f,0.f,		0.2, 2 / pow(6,1 / 2) * 5, 0,0,0,
		-2.f,0.f,0.f,		1,0,				0,0,0,
		
		//Back Right Down
		2.f,0.f,0.f,		0,0,				0,0,0,
		0.f,-4.f,0.f,		0.2, 2 / pow(6,1 / 2) * 5, 0,0,0,
		0.f,0.f,-1.f,		1,0,				0,0,0
			
	};

	GLshort Octahedron_indices[] =
	{
		0,1,2,

		3,4,5,

		6,7,8,

		9,10,11,

		12,13,14,

		15,16,17,

		18,19,20,

		21,22,23
	};

	m_indexCount = sizeof(Octahedron_indices) / sizeof(GLshort);
	m_vertexCount = sizeof(Octahedron_vertices) / (sizeof(GLfloat) * m_vertexLength);
	Mesh::calcAverageNormals(Octahedron_indices, m_indexCount, Octahedron_vertices, m_vertexCount, m_vertexLength, 5);

	Mesh::CreateMesh(Octahedron_vertices, Octahedron_indices, m_vertexCount, m_indexCount);
}