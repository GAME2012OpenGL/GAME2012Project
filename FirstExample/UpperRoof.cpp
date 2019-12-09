#include "UpperRoof.h"

UpperRoof::UpperRoof()
{

}

UpperRoof::~UpperRoof()
{

}

void UpperRoof::CreateMesh()
{
	GLfloat UpperRoof_vertices[] =
	{
		//Front
		//x, y, z			u, v		nx, ny, nz
		-2.f,0.f,2.f,		0,0,		0,0,0,
		2.f,0.f,2.f,		1,0,		0,0,0,
		1.f,1.f,0.f,		1,1,		0,0,0,	
		-1.f,1.f,0.f,		0,1,		0,0,0,

		//Left
		-2.f,0.f,-2.f,		0,0,		0,0,0,
		-2.f,0.f,2.f,		1,0,		0,0,0,
		-1.f,1.f,0.f,		0.5,1,		0,0,0,

		//Right
		2.f,0.f,2.f,		0,0,		0,0,0,
		2.f,0.f,-2.f,		1,0,		0,0,0,
		1.f,1.f,0.f,		0.5,1,		0,0,0,

		//Back
		2.f,0.f,-2.f,		0,0,		0,0,0,
		-2.f,0.f,-2.f,		1,0,		0,0,0,
		-1.f,1.f,0.f,		1,1,		0,0,0,
		1.f,1.f,0.f,		0,1,		0,0,0,

		//Bottom
		-2.f,0.f,2.f,		0,0,		0,0,0,
		2.f,0.f,2.f,		1,0,		0,0,0,
		2.f,0.f,-2.f,		1,1,		0,0,0,
		-2.f,0.f,-2.f,		0,1,		0,0,0
	};

	GLshort UpperRoof_indices[] =
	{
		0,1,2,
		0,2,3,

		4,5,6,

		7,8,9,

		10,11,12,
		10,12,13,

		15,14,17,
		15,17,16
	};

	m_indexCount = sizeof(UpperRoof_indices) / sizeof(GLshort);
	m_vertexCount = sizeof(UpperRoof_vertices) / (sizeof(GLfloat) * m_vertexLength);
	Mesh::calcAverageNormals(UpperRoof_indices, m_indexCount, UpperRoof_vertices, m_vertexCount, m_vertexLength, 5);

	Mesh::CreateMesh(UpperRoof_vertices, UpperRoof_indices, m_vertexCount, m_indexCount);
}
