#include "Pyramid_kindof.h"

PyramidKindOf::PyramidKindOf()
{

}

PyramidKindOf::~PyramidKindOf()
{

}

void PyramidKindOf::CreateMesh()
{
	GLfloat pyramidkindof_vertices[] =
	{
		//Front 
		//x, y, z				u, v		nx, ny, nz
		-1.0f, -1.0f, 1.0f,			0, 1,		0, 0, 0,	// 0.
		1.0f, -1.0f, 1.0f,			1, 1,		0, 0, 0,	// 1
		0.5f, 0.5f, 0.5f,			1, 0,		0, 0, 0,	// 2.
		-0.5f, 0.5f, 0.5f,			0, 0,		0, 0, 0,	// 3.

		//Right
		1.0f, -1.0f, 1.0f,			0, 1,		0, 0, 0,	// 4
		1.0f, -1.0f, -1.0f,			1, 1,		0, 0, 0,	// 5
		0.5f, 0.5f, -0.5f,			1, 0,		0, 0, 0,	// 6
		0.5f, 0.5f, 0.5f,			0, 0,		0, 0, 0,	// 7.

		//Left
		-1.0f, -1.0f, -1.0f,		0, 1,		0, 0, 0,	// 8
		-1.0f, -1.0f, 1.0f,			1, 1,		0, 0, 0,	// 9
		-0.5f, 0.5f, 0.5f,			1, 0,		0, 0, 0,	// 10
		-0.5f, 0.5f, -0.5f,			0, 0,		0, 0, 0,	// 11

		//Back
		1.0f, -1.0f, -1.0f,			0, 1,		0, 0, 0,	// 12
		-1.0f, -1.0f, -1.0f,		1, 1,		0, 0, 0,	// 13
		-0.5f, 0.5f, -0.5f,			1, 0,		0, 0, 0,	// 14
		0.5f, 0.5f, -0.5f,			0, 0,		0, 0, 0,	// 15

		//Up
		-0.5f, 0.5f, 0.5f,			0, 1,		0, 0, 0,	//16
		0.5f, 0.5f, 0.5f,			1, 1,		0, 0, 0,	//17
		0.5f, 0.5f, -0.5f,			1, 0,		0, 0, 0,	//18
		-0.5f, 0.5f, -0.5f,			0, 0,		0, 0, 0,	//19

		//Down
		-1.0f, -1.0f, -1.0f,		0, 1,		0, 0, 0,	//20
		1.0f, -1.0f, -1.0f,			1, 1,		0, 0, 0,	//21
		1.0f, -1.0f, 1.0f,			1, 0,		0, 0, 0,	//22
		-1.0f, -1.0f, 1.0f,			0, 0,		0, 0, 0		//23

	};

	GLshort pyramidkindof_indices[] =
	{
		//Front
		0, 1, 3,
		1, 2, 3,

		//Right
		4, 5, 7,
		5, 6, 7,

		//Left
		8, 9, 11,
		9, 10, 11,

		//Back
		12, 13, 15,
		13, 14, 15,

		//Up
		16, 17, 19,
		17, 18, 19,

		//DOwn
		20, 21, 23,
		21, 22, 23

	};

	m_indexCount = sizeof(pyramidkindof_indices) / sizeof(GLshort);
	m_vertexCount = sizeof(pyramidkindof_vertices) / (sizeof(GLfloat) * m_vertexLength);
	Mesh::calcAverageNormals(pyramidkindof_indices, m_indexCount, pyramidkindof_vertices, m_vertexCount, m_vertexLength, 5);

	Mesh::CreateMesh(pyramidkindof_vertices, pyramidkindof_indices, m_vertexCount, m_indexCount);
}
