#include "GeometryGenerator.h"

#include "Cube.h"
#include "Pyramid.h"
#include "Pyramid_kindof.h"
#include "Plane.h"
#include "Sandwich.h"
#include "Cylinder.h"
#include "Rhombus.h"
#include "Octahedron.h"
#include "UpperRoof.h"
#include "Trapezoid.h"
#include "Cone.h"

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

	//Create Cylinder
	Cylinder* pCylinder = new Cylinder;
	pCylinder->CreateMesh();
	m_vecMeshes.push_back(pCylinder);


	//Create Upper roof
	UpperRoof* pURoof = new UpperRoof;
	pURoof->CreateMesh();
	m_vecMeshes.push_back(pURoof);

	Rhombus* pRhombus = new Rhombus;
	pRhombus->CreateMesh();
	m_vecMeshes.push_back(pRhombus);

	Octahedron* pOctahedron = new Octahedron;
	pOctahedron->CreateMesh();
	m_vecMeshes.push_back(pOctahedron);

	Trapezoid* pTrapezoid = new Trapezoid;
	pTrapezoid->CreateMesh();
	m_vecMeshes.push_back(pTrapezoid);

	Cone* pCone = new Cone;
	pCone->CreateMesh();
	m_vecMeshes.push_back(pCone);

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
