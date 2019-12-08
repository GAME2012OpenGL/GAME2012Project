#include "Cylinder.h"

#include <cmath>

Cylinder::Cylinder()
{

}

Cylinder::~Cylinder()
{

}

void Cylinder::CreateMesh()
{
	const float PI = 3.1415926f;
	float sectorCount = 10.f;
	float sectorStep = 2 * PI / sectorCount;
	float sectorAngle = 0.f;  // radian

	//Top circle + center of circle
	m_vertexCount = sectorCount + 1;
	GLfloat* vertices = new GLfloat[m_vertexCount * m_vertexLength];
	int iIndex = 0;

	//Center of circle
	vertices[iIndex] = 0.f;
	vertices[iIndex + 1] = 0.5f;
	vertices[iIndex + 2] = 0.f;

	//u, v
	vertices[iIndex + 3] = 0.5f;
	vertices[iIndex + 4] = 0.5f;

	//Normal
	vertices[iIndex + 5] = 0.f;
	vertices[iIndex + 6] = 1.f;
	vertices[iIndex + 7] = 0.f;

	iIndex += m_vertexLength;
	for (int i = 0; i < sectorCount; ++i)
	{
		sectorAngle = i * sectorStep;

		//Center of circle
		vertices[iIndex] = cos(sectorAngle);
		vertices[iIndex + 1] = 0.5f;
		vertices[iIndex + 2] = sin(sectorAngle);

		//u, v
		vertices[iIndex + 3] = cos(sectorAngle) * 0.5f + 0.5f;
		vertices[iIndex + 4] = sin(sectorAngle) * 0.5f + 0.5f;

		//Normal
		vertices[iIndex + 5] = 0.f;
		vertices[iIndex + 6] = 1.f;
		vertices[iIndex + 7] = 0.f;

		iIndex += m_vertexLength;
	}

	m_indexCount = sectorCount * 3;
	GLshort* indices = new GLshort[m_indexCount];
	iIndex = 0;
	for (int i = 1; i < sectorCount + 1; ++i)
	{
		indices[iIndex] = i;
		indices[iIndex + 1] = 0;
		if (i == sectorCount)
			indices[iIndex + 2] = 1;
		else
			indices[iIndex + 2] = i + 1;
		iIndex += 3;
	}

	////Create Index
	//m_indexCount = (iNumOfGrid * iNumOfGrid) * 2 * 3;
	//GLshort* plane_indices = new GLshort[m_indexCount];
	//iIndex = 0;
	//for (int j = 0; j < iNumOfGrid; ++j)
	//{
	//	for (int i = 0; i < iNumOfGrid; ++i)
	//	{
	//		int row1 = j * (iNumOfGrid + 1);
	//		int row2 = (j + 1) * (iNumOfGrid + 1);

	//		// triangle 1
	//		plane_indices[iIndex] = row1 + i;
	//		plane_indices[iIndex + 1] = row2 + i + 1;
	//		plane_indices[iIndex + 2] = row1 + i + 1;
	//		iIndex += 3;

	//		// triangle 2
	//		plane_indices[iIndex] = row1 + i;
	//		plane_indices[iIndex + 1] = row2 + i;
	//		plane_indices[iIndex + 2] = row2 + i + 1;
	//		iIndex += 3;
	//	}
	//}



	//Mesh::calcAverageNormals(plane_indices, m_indexCount, plane_vertices, m_vertexCount, m_vertexLength, 5);

	Mesh::CreateMesh(vertices, indices, m_vertexCount, m_indexCount);

	delete[] indices;
	delete[] vertices;
}
