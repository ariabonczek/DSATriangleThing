#ifndef DATA_HPP
#define DATA_HPP

#include "../Config.hpp"

NS_BEGIN

struct Color
{
	float r, g, b, a;

	Color();
	Color(float r, float g, float b, float a);

	static Color Red;
	static Color Green;
	static Color Blue;
	static Color Black;
	static Color White;
	static Color Gray;
	static Color Yellow;
	static Color Magenta;
	static Color Cyan;
	static Color RebeccaPurple;
	static Color PapayaWhip;
};

struct BaseVertex 
{
	Vector3 position;
	Vector2 texcoord;
};

struct ColoredVertex
{
	Vector3 position;
	Vector2 texcoord;
	Color color;
};

struct MeshVertex
{
	Vector3 position;
	Vector2 texcoord;
	Color color;
	Vector3 normal;
	Vector3 tangent;
};

struct MeshData
{
	MeshData() {}
	MeshData(MeshVertex* verts, uint numVertices, uint* inds, uint numIndices)
	{
		vertices.resize(numVertices);
		memcpy(&vertices[0], verts, sizeof(MeshVertex) * numVertices);

		indices.resize(numIndices);
		memcpy(&indices[0], inds, sizeof(uint) * numIndices);
	}
	
	std::vector<MeshVertex> vertices;
	std::vector<uint> indices;
};

NS_END

#endif