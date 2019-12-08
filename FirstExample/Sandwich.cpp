#include "Sandwich.h"

Sandwich::Sandwich()
{

}

Sandwich::~Sandwich()
{

}

void Sandwich::CreateMesh()
{
	GLfloat sandwich_vertices[] =
	{
		//Front
		//x, y, z			u, v		nx, ny, nz
		0.f, 1.f, 1.f,		0.5, 0,		0, 0, 0,		//0
		-1.f, 0.f, 1.f,		0, 1,		0, 0, 0,		//1
		1.f, 0.f, 1.f,		1, 1,		0, 0, 0,		//2

		//right
		0.f, 1.f, 1.f,		0.5, 0,		0, 0, 0,		//3
		1.f, 0.f, 1.f,		0, 1,		0, 0, 0,		//4
		1.f, 0.f, 0.f,		1, 1,		0, 0, 0,		//5
		0.f, 1.f, 0.f,		0, 0.5,		0, 0, 0,		//6


		//back
		0.f, 1.f, 0.f,		0.5, 0,		0, 0, 0,		//7
		1.f, 0.f, 0.f,		0, 1,		0, 0, 0,		//8
		-1.f, 0.f, 0.f,		1, 1,		0, 0, 0,		//9

		//left
		0.f, 1.f, 0.f,		0.5, 0,		0, 0, 0,		//10
		-1.f, 0.f, 0.f,		0, 1,		0, 0, 0,		//11
		-1.f, 0.f, 1.f,		1, 1,		0, 0, 0,		//12
		0.f, 1.f, 1.f,		0.5, 0,		0, 0, 0,		//13

		//bottom
		-1.f, 0.f, 1.f,		0, 0,		0, 0, 0,		//14
		-1.f, 0.f, 0.f,		0, 1,		0, 0, 0,		//15
		1.f, 0.f, 0.f,		1, 1,		0, 0, 0,		//16
		1.f, 0.f, 1.f,		1, 0,		0, 0, 0			//17

	};

	GLshort sandwich_indices[] =
	{
		//front
		0, 1, 2,

		//rIght
		3, 4, 5,
		3, 5, 6,

		//back
		7, 8, 9,

		//left
		10, 11, 12,
		10, 12 ,13,


		//bottom
		14, 15 ,16,
		14, 16, 17

	};

	m_indexCount = sizeof(sandwich_indices) / sizeof(GLshort);
	m_vertexCount = sizeof(sandwich_vertices) / (sizeof(GLfloat) * m_vertexLength);
	Mesh::calcAverageNormals(sandwich_indices, m_indexCount, sandwich_vertices, m_vertexCount, m_vertexLength, 5);

	Mesh::CreateMesh(sandwich_vertices, sandwich_indices, m_vertexCount, m_indexCount);
}
