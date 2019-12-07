#include "Cylinder.h"

Cylinder::Cylinder()
{

}

Cylinder::~Cylinder()
{

}

void Cylinder::CreateMesh()
{
	//m_vertexCount = (iNumOfGrid + 1) * (iNumOfGrid + 1);

	//GLfloat* plane_vertices = new GLfloat[m_vertexCount * m_vertexLength];
	//int iIndex = 0;
	//for (int j = 0; j <= iNumOfGrid; ++j)
	//{
	//	for (int i = 0; i <= iNumOfGrid; ++i)
	//	{
	//		float x = (float)i / (float)iNumOfGrid;
	//		float y = 0;
	//		float z = (float)j / (float)iNumOfGrid;

	//		//x, y, z
	//		plane_vertices[iIndex] = x;
	//		plane_vertices[iIndex + 1] = y;
	//		plane_vertices[iIndex + 2] = z;

	//		//u, v
	//		plane_vertices[iIndex + 3] = (float)i / (float)iNumOfGrid;
	//		plane_vertices[iIndex + 4] = (float)j / (float)iNumOfGrid;

	//		//Normal
	//		plane_vertices[iIndex + 5] = 0.f;
	//		plane_vertices[iIndex + 6] = 0.f;
	//		plane_vertices[iIndex + 7] = 0.f;

	//		iIndex += m_vertexLength;
	//	}
	//}

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

	//Mesh::CreateMesh(plane_vertices, plane_indices, m_vertexCount, m_indexCount);

	//delete[] plane_indices;
	//delete[] plane_vertices;
}
