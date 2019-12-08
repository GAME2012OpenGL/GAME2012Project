#include "GeometryGenerator.h"

#include "Cube.h"
#include "Pyramid.h"
#include "Pyramid_kindof.h"
#include "Plane.h"
#include "Sandwich.h"

std::vector<Mesh*> GeometryGenerator::m_vecMeshes;

void GeometryGenerator::GenerateMeshes(int iNumOfPlaneGrid)
{
	//Create Cube Mesh
	Cube* pCube = new Cube;
	pCube->CreateMesh();
	m_vecMeshes.push_back(pCube);

	//Create Pyramid
	Pyramid* pPyramid = new Pyramid;
	pPyramid->CreateMesh();
	m_vecMeshes.push_back(pPyramid);

	//Create kind of Pyramid
	PyramidKindOf* pPyramid_kindof = new PyramidKindOf;
	pPyramid_kindof->CreateMesh();
	m_vecMeshes.push_back(pPyramid_kindof);

	//Create Plane
	Plane* pPlane = new Plane;
	pPlane->CreateMesh(iNumOfPlaneGrid);
	m_vecMeshes.push_back(pPlane);

	//Create Sandwich
	Sandwich* pSandwich = new Sandwich;
	pSandwich->CreateMesh();
	m_vecMeshes.push_back(pSandwich);
}

void GeometryGenerator::DestroyMeshes()
{
	for (int i = 0; i < m_vecMeshes.size(); ++i)
	{
		m_vecMeshes[i]->ClearMesh();
		delete m_vecMeshes[i];
	}
	m_vecMeshes.clear();
}

Mesh* GeometryGenerator::GetMesh(EMeshList meshType)
{
	if (meshType >= MESH_END || meshType < 0)
		return nullptr;

	return m_vecMeshes[meshType];
}
