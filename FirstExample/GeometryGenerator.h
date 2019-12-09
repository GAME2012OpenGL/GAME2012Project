#pragma once
#ifndef __GEOMETRY_GENERATOR__
#define __GEOMETRY_GENERATOR__

#include <vector>

class Mesh;

class GeometryGenerator
{
public :
	enum EMeshList{MESH_CUBE,MESH_PYRAMID, MESH_PYRAMIDKINDOF, MESH_PLANE, 
				   MESH_SANDWICH, MESH_CYLINDER, MESH_RHOMBUS, 
				   MESH_OCTAHEDRON, MESH_UPPERROOF, MESH_TRAPEZOID, MESH_END};

private :
	static std::vector<Mesh*> m_vecMeshes;

public :
	static void GenerateMeshes(int iNumOfPlaneGrid);
	static void DestroyMeshes();
	static Mesh* GetMesh(EMeshList meshType);
};

#endif