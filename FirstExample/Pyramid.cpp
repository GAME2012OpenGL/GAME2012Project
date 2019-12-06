#include "Pyramid.h"

Pyramid::Pyramid()
{

}

Pyramid::~Pyramid()
{

}

void Pyramid::CreateMesh()
{
	GLfloat pyramid_vertices[] =
	{
		//Front
		//x, y, z			u, v		nx, ny, nz
		0.f, 1.f, 0.f,		0.5, 0,		0, 0, 0,		//0
		-1.f, 0.f, 1.f,		0, 1,		0, 0, 0,		//1
		1.f, 0.f, 1.f,		1, 1,		0, 0, 0,		//2

		//right
		0.f, 1.f, 0.f,		0.5, 0,		0, 0, 0,		//3
		1.f, 0.f, 1.f,		0, 1,		0, 0, 0,		//4
		1.f, 0.f, -1.f,		1, 1,		0, 0, 0,		//5

		//back
		0.f, 1.f, 0.f,		0.5, 0,		0, 0, 0,		//6
		1.f, 0.f, -1.f,		0, 1,		0, 0, 0,		//7
		-1.f, 0.f, -1.f,	1, 1,		0, 0, 0,		//8

		//left
		0.f, 1.f, 0.f,		0.5, 0,		0, 0, 0,		//9
		-1.f, 0.f, -1.f,	0, 1,		0, 0, 0,		//10
		-1.f, 0.f, 1.f,		1, 1,		0, 0, 0,		//11

		//bottom
		-1.f, 0.f, 1.f,		0, 0,		0, 0, 0,		//12
		-1.f, 0.f, -1.f,	0, 1,		0, 0, 0,		//13
		1.f, 0.f, -1.f,		1, 1,		0, 0, 0,		//14
		1.f, 0.f, 1.f,		1, 0,		0, 0, 0			//15

	};

	GLshort pyramid_indices[] =
	{
		//front
		0, 1, 2,

		//rIght
		3, 4, 5,

		//back
		6, 7, 8,

		//left
		9, 10, 11,

		//bottom
		12, 13 ,14,
		12, 14, 15
	
	};

	m_indexCount = sizeof(pyramid_indices) / sizeof(GLshort);
	m_vertexCount = sizeof(pyramid_vertices) / (sizeof(GLfloat) * m_vertexLength);
	Mesh::calcAverageNormals(pyramid_indices, m_indexCount, pyramid_vertices, m_vertexCount, m_vertexLength, 5);

	Mesh::CreateMesh(pyramid_vertices, pyramid_indices, m_vertexCount, m_indexCount);
}
