#include "GeometryGenerator.h"

#include "Cube.h"

std::vector<Mesh*> GeometryGenerator::m_vecMeshes;

void GeometryGenerator::GenerateMeshes()
{
	//Create Cube Mesh
	Cube* pCube = new Cube;
	pCube->CreateMesh();
	m_vecMeshes.push_back(pCube);
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
	return nullptr;
}
