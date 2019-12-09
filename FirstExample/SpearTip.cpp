#include <cmath>
#include "SpearTip.h"

SpearTip::SpearTip()
{
}

SpearTip::~SpearTip()
{
}

void SpearTip::CreateMesh()
{
	GLfloat SpearTip_vertices[] =
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
		-1.f,-3.f,0.f,		0,0,			0,0,0,
		0.f,-3.f,1.f,		1,0,			0,0,0,
		0.f,0.f,1.f,		1,1,			0,0,0,
		-2.f,0.f,0.f,		0,1,			0,0,0,

		//Front Right Down
		0.f,-3.f,1.f,		0,0,			0,0,0,
		1.f, -3.f,0.f,		1,0,			0,0,0,
		2.f,0.f,0.f,		1,1,			0,0,0,
		0.f,0.f,1.f,		1,0,			0,0,0,

		//Back Left Down
		0.f,-3.f,-1.f,		0,0,			0,0,0,
		-1.f,-3.f,0.f,		1,0,			0,0,0,
		-2.f,0.f,0.f,		1,1,			0,0,0,
		0.f,0.f,-1.f,		0,1,			0,0,0,

		//Back Right Down
		1.f,-3.f,0.f,		0,0,			0,0,0,
		0.f,-3.f,-1.f,		1,0,			0,0,0,
		0.f,0.f,-1.f,		1,1,			0,0,0,
		2.f,0.f,0.f,		0,1,			0,0,0,

		//Bottom
		0.f,-3.f,1.f,		0,0,			0,0,0,
		-1.f,-3.f,0.f,		1,0,			0,0,0,
		0.f-3.f,-1.f,		1,1,			0,0,0,
		1.f,-3.f,0.f,		1,0,			0,0,0
	};

	GLshort SpearTip_indices[] =
	{
		0,1,2,

		3,4,5,

		6,7,8,

		9,10,11,

		12,13,14,
		12,14,15,

		16,17,18,
		16,18,19,

		20,21,22,
		20,22,23,

		24,25,26,
		24,26,27,

		28,29,30,
		28,30,31
	};

	m_indexCount = sizeof(SpearTip_indices) / sizeof(GLshort);
	m_vertexCount = sizeof(SpearTip_vertices) / (sizeof(GLfloat) * m_vertexLength);
	Mesh::calcAverageNormals(SpearTip_indices, m_indexCount, SpearTip_vertices, m_vertexCount, m_vertexLength, 5);

	Mesh::CreateMesh(SpearTip_vertices, SpearTip_indices, m_vertexCount, m_indexCount);
}