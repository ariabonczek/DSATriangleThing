#ifndef MESHBUILDER_HPP
#define MESHBUILDER_HPP

#include "../Config.hpp"
#include "Data.hpp"
#include "Mesh.hpp"

NS_BEGIN

class MeshBuilder
{
public:
	~MeshBuilder();

	static MeshData CreateSphere(float radius = 1.0f, uint numSubdivisions = 3, Color color = Color::Gray);
	static MeshData CreateCube(float size = 1.0f, Color color = Color::Gray);
	static MeshData CreateCylinder(float radius = 0.5f, float height = 1.0, uint axisDivisions = 20, uint heightDivisions = 3, Color color = Color::Gray);
	static MeshData CreateCone(float radius = 0.5f, float height = 1.0f, uint axisDivisions = 20, uint heightDivisions = 3, Color color = Color::Gray);
	static MeshData CreateTube(float outerRadius = 1.0f, float innerRadius = 0.6f, float height = 1.0f, uint axisSubdivisions = 20, uint heightDivisions = 3, Color color = Color::Gray);
	static MeshData CreateTorus(float radius = 1.0f, float sectionRadius = 0.5f, uint numSubdivisions = 20, Color color = Color::Gray);
private:
	MeshBuilder();

	static void CreateOctahedron(std::vector<MeshVertex>& vertices, std::vector<uint>& indices, uint resolution);
	static uint CreateUpperStrip(uint steps, uint vTop, uint vBottom, uint t, std::vector<uint>& indices);
	static uint CreateLowerStrip(uint steps, uint vTop, uint vBottom, uint t, std::vector<uint>& indices);
	static uint CreateVertexLine(Vector3 from, Vector3 to, uint steps, uint v, std::vector<MeshVertex>& vertices);
	static uint CreateVertexLineC(Vector3 from, Vector3 to, uint steps, uint v, std::vector<MeshVertex>& vertices);
	static uint CreateRing(uint axisDivisions, uint heightDivisions, uint h, uint t, std::vector<uint>& indices);

	static uint CreateInnerTubeRing(uint axisDivisions, uint heightDivisions, uint h, uint t, std::vector<uint>& indices);
	static uint CreateOuterTubeRing(uint axisDivisions, uint heightDivisions, uint h, uint t, std::vector<uint>& indices);
	
	static uint CreateTorusLoop(float theta, float radius, float sectionRadius, uint heightDivisions, uint v, std::vector<MeshVertex>& vertices);
};

NS_END

#endif