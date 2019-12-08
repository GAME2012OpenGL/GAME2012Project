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
	float sectorCount = 36.f;
	float sectorStep = 2 * PI / sectorCount;
	float sectorAngle = 0.f;  // radian

	//(Top circle + center of upper circle + end of circle vertex) + 
	//(Uppder side + end of circle vertex) +
	//(Bottom side + end of circle vertex) +
	//(Botton circle + center of bottom circle + end of circle vertex)
	m_vertexCount = (sectorCount + 1 + 1) + 
					(sectorCount + 1 ) + 
					(sectorCount + 1) +
					(sectorCount + 1 + 1);
	GLfloat* vertices = new GLfloat[(m_vertexCount * m_vertexLength)];
	int iIndex = 0;

	//Center of Top circle
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

	//Top circle
	for (int i = 0; i <= sectorCount; ++i)
	{
		sectorAngle = i * sectorStep;

		//x, y, z
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

	//Upper Side vertex
	for (int i = 0; i <= sectorCount; ++i)
	{
		sectorAngle = i * sectorStep;

		//x, y, z
		vertices[iIndex] = cos(sectorAngle);
		vertices[iIndex + 1] = 0.5f;
		vertices[iIndex + 2] = sin(sectorAngle);

		//u, v
		vertices[iIndex + 3] = (float)i / sectorCount;
		vertices[iIndex + 4] = 0;

		//Normal
		vertices[iIndex + 5] = cos(sectorAngle);
		vertices[iIndex + 6] = 0.f;
		vertices[iIndex + 7] = sin(sectorAngle);

		iIndex += m_vertexLength;
	}

	//Down Side vertex
	for (int i = 0; i <= sectorCount; ++i)
	{
		sectorAngle = i * sectorStep;

		//x, y, z
		vertices[iIndex] = cos(sectorAngle);
		vertices[iIndex + 1] = -0.5f;
		vertices[iIndex + 2] = sin(sectorAngle);

		//u, v
		vertices[iIndex + 3] = (float)i / sectorCount;
		vertices[iIndex + 4] = 1.f;

		//Normal
		vertices[iIndex + 5] = cos(sectorAngle);
		vertices[iIndex + 6] = 0.f;
		vertices[iIndex + 7] = sin(sectorAngle);

		iIndex += m_vertexLength;
	}

	//Center of Bottom circle
	vertices[iIndex] = 0.f;
	vertices[iIndex + 1] = -0.5f;
	vertices[iIndex + 2] = 0.f;

	//u, v
	vertices[iIndex + 3] = 0.5f;
	vertices[iIndex + 4] = 0.5f;

	//Normal
	vertices[iIndex + 5] = 0.f;
	vertices[iIndex + 6] = -1.f;
	vertices[iIndex + 7] = 0.f;

	iIndex += m_vertexLength;

	//Bottom circle
	for (int i = 0; i <= sectorCount; ++i)
	{
		sectorAngle = i * sectorStep;

		//x, y, z
		vertices[iIndex] = cos(sectorAngle);
		vertices[iIndex + 1] = -0.5f;
		vertices[iIndex + 2] = sin(sectorAngle);

		//u, v
		vertices[iIndex + 3] = cos(sectorAngle) * 0.5f + 0.5f;
		vertices[iIndex + 4] = sin(sectorAngle) * 0.5f + 0.5f;

		//Normal
		vertices[iIndex + 5] = 0.f;
		vertices[iIndex + 6] = -1.f;
		vertices[iIndex + 7] = 0.f;

		iIndex += m_vertexLength;
	}





	//Top circle + Side + Bottom circle
	m_indexCount = (sectorCount * 3) + (sectorCount * 6) + (sectorCount * 3);
	GLshort* indices = new GLshort[m_indexCount];
	iIndex = 0;
	//Top circle
	for (int i = 1; i < sectorCount + 1; ++i)
	{
		indices[iIndex] = i;
		indices[iIndex + 1] = 0;
		indices[iIndex + 2] = i + 1;
		iIndex += 3;
	}

	//Side
	int iStartIndex = sectorCount + 2;
	for (int i = 0; i < sectorCount; ++i)
	{
		//First Triangle
		indices[iIndex] = iStartIndex;
		indices[iIndex + 1] = iStartIndex + 1;
		indices[iIndex + 2] = iStartIndex + sectorCount + 1;
		iIndex += 3;

		//Second Triangle
		indices[iIndex] = iStartIndex + 1;
		indices[iIndex + 1] = iStartIndex + 1 + sectorCount + 1;
		indices[iIndex + 2] = iStartIndex + 1 + sectorCount;
		iIndex += 3;
		
		iStartIndex += 1;
	}

	//Bottom circle
	iStartIndex = (sectorCount * 3) + 5;
	for (int i = 1; i < sectorCount + 1; ++i)
	{
		indices[iIndex] = iStartIndex;
		indices[iIndex + 1] = iStartIndex + 1;
		indices[iIndex + 2] = (sectorCount * 3) + 4;

		iIndex += 3;
		iStartIndex += 1;
	}

	//Mesh::calcAverageNormals(plane_indices, m_indexCount, plane_vertices, m_vertexCount, m_vertexLength, 5);

	Mesh::CreateMesh(vertices, indices, m_vertexCount, m_indexCount);

	delete[] indices;
	delete[] vertices;
}
