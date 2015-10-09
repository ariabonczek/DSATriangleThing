#include "MeshBuilder.hpp"

NS_BEGIN

MeshBuilder::~MeshBuilder()
{

}

// Followed this tutorial: http://www.binpress.com/tutorial/creating-an-octahedron-sphere/162
MeshData MeshBuilder::CreateSphere(float radius, uint numSubdivisions, Color color)
{
	MeshData data;
	MeshVertex toAdd;

	numSubdivisions = std::max(0u, std::min(numSubdivisions, 7u));

	int resolution = 1 << numSubdivisions;
	data.vertices.resize((resolution + 1) * (resolution + 1) * 4 - (resolution * 2 - 1) * 3);
	data.indices.resize((1 << (numSubdivisions * 2 + 3)) * 3);

	CreateOctahedron(data.vertices, data.indices, resolution);

	for (uint i = 0; i < data.vertices.size(); ++i)
	{
		// Color
		data.vertices[i].color = color;
		// Position and Normals
		data.vertices[i].position.Normalize();
		data.vertices[i].normal = data.vertices[i].position;

		Vector3 p = Vector3::Multiply(data.vertices[i].position, radius);
		data.vertices[i].position = p;

		// UVs
		float theta = atan2(data.vertices[i].position.x, data.vertices[i].position.z);
		float phi = asinf(data.vertices[i].position.y);

		data.vertices[i].texcoord.x = theta / PI * 2;
		if (data.vertices[i].texcoord.x < 0.0f)
			data.vertices[i].texcoord.x += 1.0f;

		data.vertices[i].texcoord.y = phi / PI + 0.5f;

		// Tangents
		data.vertices[i].tangent.x = -radius*sinf(phi)*sinf(theta);
		data.vertices[i].tangent.y = 0.0f;
		data.vertices[i].tangent.z = +radius*cosf(phi)*cosf(theta);

		data.vertices[i].tangent.Normalize();
	}

	return data;
}

void MeshBuilder::CreateOctahedron(std::vector<MeshVertex>& vertices, std::vector<uint>& indices, uint resolution)
{
	uint v = 0, vBottom = 0, t = 0;

	Vector3 directions[4] =
	{
		-Vector3::Right,
		-Vector3::Forward,
		Vector3::Right,
		Vector3::Forward
	};

	for (uint i = 0; i < 4; i++)
	{
		vertices[v++].position = -Vector3::Up;
	}

	// bottom hemisphere
	for (uint i = 1; i <= resolution; i++)
	{
		float progress = (float)i / resolution;
		Vector3 from, to;
		vertices[v++].position = to = Vector3::Lerp(-Vector3::Up, Vector3::Forward, progress);
		for (uint d = 0; d < 4; d++)
		{
			from = to;
			to = Vector3::Lerp(-Vector3::Up, directions[d], progress);
			t = CreateLowerStrip(i, v, vBottom, t, indices);
			v = CreateVertexLine(from, to, i, v, vertices);
			vBottom += i > 1 ? (i - 1) : 1;
		}
		vBottom = v - 1 - i * 4;
	}

	// top hemisphere
	for (uint i = resolution - 1; i >= 1; i--)
	{
		float progress = (float)i / resolution;
		Vector3 from, to;
		vertices[v++].position = to = Vector3::Lerp(Vector3::Up, Vector3::Forward, progress);
		for (uint d = 0; d < 4; d++)
		{
			from = to;
			to = Vector3::Lerp(Vector3::Up, directions[d], progress);
			t = CreateUpperStrip(i, v, vBottom, t, indices);
			v = CreateVertexLine(from, to, i, v, vertices);
			vBottom += i + 1;
		}
		vBottom = v - 1 - i * 4;
	}

	for (uint i = 0; i < 4; i++)
	{
		indices[t++] = vBottom;
		indices[t++] = v;
		indices[t++] = ++vBottom;
		vertices[v++].position = Vector3::Up;
	}
}

uint MeshBuilder::CreateUpperStrip(uint steps, uint vTop, uint vBottom, uint t, std::vector<uint>& indices)
{
	indices[t++] = vBottom;
	indices[t++] = vTop - 1;
	indices[t++] = ++vBottom;
	for (uint i = 1; i <= steps; i++)
	{
		indices[t++] = vTop - 1;
		indices[t++] = vTop;
		indices[t++] = vBottom;

		indices[t++] = vBottom;
		indices[t++] = vTop++;
		indices[t++] = ++vBottom;
	}

	return t;
}

uint MeshBuilder::CreateLowerStrip(uint steps, uint vTop, uint vBottom, uint t, std::vector<uint>& indices)
{
	for (uint i = 1; i < steps; i++)
	{
		indices[t++] = vBottom;
		indices[t++] = vTop - 1;
		indices[t++] = vTop;

		indices[t++] = vBottom++;
		indices[t++] = vTop++;
		indices[t++] = vBottom;
	}
	indices[t++] = vBottom;
	indices[t++] = vTop - 1;
	indices[t++] = vTop;

	return t;
}

uint MeshBuilder::CreateVertexLine(Vector3 from, Vector3 to, uint steps, uint v, std::vector<MeshVertex>& vertices)
{
	for (uint i = 1; i <= steps; i++)
	{
		vertices[v++].position = Vector3::Lerp(from, to, (float)i / steps);
	}
	return v;
}

MeshData MeshBuilder::CreateCube(float size, Color color)
{
	MeshData data;
	uint v = 0, i = 0;

	data.vertices.resize(24);
	data.indices.resize(36);

	MeshVertex cVert;
	cVert.color = color;
	///
	// MeshVertex order when facing quad: bottom left, top left, top right, bottom right
	///

	//Front
	cVert.position = Vector3(-size, -size, -size);
	cVert.normal = Vector3(0.0f, 0.0f, -1.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texcoord = Vector2(0.0f, 1.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(-size, size, -size);
	cVert.normal = Vector3(0.0f, 0.0f, -1.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texcoord = Vector2(0.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(size, size, -size);
	cVert.normal = Vector3(0.0f, 0.0f, -1.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texcoord = Vector2(1.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(size, -size, -size);
	cVert.normal = Vector3(0.0f, 0.0f, -1.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texcoord = Vector2(1.0f, 1.0f);
	data.vertices[v++] = cVert;

	// Back
	cVert.position = Vector3(-size, -size, size);
	cVert.normal = Vector3(0.0f, 0.0f, 1.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texcoord = Vector2(0.0f, 1.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(-size, size, size);
	cVert.normal = Vector3(0.0f, 0.0f, 1.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texcoord = Vector2(0.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(size, size, size);
	cVert.normal = Vector3(0.0f, 0.0f, 1.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texcoord = Vector2(1.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(size, -size, size);
	cVert.normal = Vector3(0.0f, 0.0f, 1.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texcoord = Vector2(1.0f, 1.0f);
	data.vertices[v++] = cVert;

	// Left
	cVert.position = Vector3(-size, -size, size);
	cVert.normal = Vector3(-1.0f, 0.0f, 0.0f);
	cVert.tangent = Vector3(0.0f, 0.0f, -1.0f);
	cVert.texcoord = Vector2(0.0f, 1.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(-size, size, size);
	cVert.normal = Vector3(-1.0f, 0.0f, 0.0f);
	cVert.tangent = Vector3(0.0f, 0.0f, -1.0f);
	cVert.texcoord = Vector2(0.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(-size, size, -size);
	cVert.normal = Vector3(-1.0f, 0.0f, 0.0f);
	cVert.tangent = Vector3(0.0f, 0.0f, -1.0f);
	cVert.texcoord = Vector2(1.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(-size, -size, -size);
	cVert.normal = Vector3(-1.0f, 0.0f, 0.0f);
	cVert.tangent = Vector3(0.0f, 0.0f, -1.0f);
	cVert.texcoord = Vector2(1.0f, 1.0f);
	data.vertices[v++] = cVert;

	// Right
	cVert.position = Vector3(size, -size, -size);
	cVert.normal = Vector3(1.0f, 0.0f, 0.0f);
	cVert.tangent = Vector3(0.0f, 0.0f, -1.0f);
	cVert.texcoord = Vector2(0.0f, 1.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(size, size, -size);
	cVert.normal = Vector3(1.0f, 0.0f, 0.0f);
	cVert.tangent = Vector3(0.0f, 0.0f, -1.0f);
	cVert.texcoord = Vector2(0.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(size, size, size);
	cVert.normal = Vector3(1.0f, 0.0f, 0.0f);
	cVert.tangent = Vector3(0.0f, 0.0f, -1.0f);
	cVert.texcoord = Vector2(1.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(size, -size, size);
	cVert.normal = Vector3(1.0f, 0.0f, 0.0f);
	cVert.tangent = Vector3(0.0f, 0.0f, -1.0f);
	cVert.texcoord = Vector2(1.0f, 1.0f);
	data.vertices[v++] = cVert;

	// Top
	cVert.position = Vector3(-size, size, -size);
	cVert.normal = Vector3(0.0f, 1.0f, 0.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texcoord = Vector2(0.0f, 1.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(-size, size, size);
	cVert.normal = Vector3(0.0f, 1.0f, 0.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texcoord = Vector2(0.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(size, size, size);
	cVert.normal = Vector3(0.0f, 1.0f, 0.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texcoord = Vector2(1.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(size, size, -size);
	cVert.normal = Vector3(0.0f, 1.0f, 0.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texcoord = Vector2(1.0f, 1.0f);
	data.vertices[v++] = cVert;

	// Bottom
	cVert.position = Vector3(-size, -size, -size);
	cVert.normal = Vector3(0.0f, -1.0f, 0.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texcoord = Vector2(0.0f, 1.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(-size, -size, size);
	cVert.normal = Vector3(0.0f, -1.0f, 0.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texcoord = Vector2(0.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(size, -size, size);
	cVert.normal = Vector3(0.0f, -1.0f, 0.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texcoord = Vector2(1.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(size, -size, -size);
	cVert.normal = Vector3(0.0f, -1.0f, 0.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texcoord = Vector2(1.0f, 1.0f);
	data.vertices[v++] = cVert;

	// INDICES
	data.indices[i++] = (0);
	data.indices[i++] = (1);
	data.indices[i++] = (3);
	data.indices[i++] = (3);
	data.indices[i++] = (1);
	data.indices[i++] = (2);

	data.indices[i++] = (4);
	data.indices[i++] = (6);
	data.indices[i++] = (5);
	data.indices[i++] = (6);
	data.indices[i++] = (4);
	data.indices[i++] = (7);

	data.indices[i++] = (8);
	data.indices[i++] = (9);
	data.indices[i++] = (10);
	data.indices[i++] = (10);
	data.indices[i++] = (11);
	data.indices[i++] = (8);

	data.indices[i++] = (12);
	data.indices[i++] = (13);
	data.indices[i++] = (14);
	data.indices[i++] = (14);
	data.indices[i++] = (15);
	data.indices[i++] = (12);

	data.indices[i++] = (16);
	data.indices[i++] = (17);
	data.indices[i++] = (18);
	data.indices[i++] = (18);
	data.indices[i++] = (19);
	data.indices[i++] = (16);

	data.indices[i++] = (20);
	data.indices[i++] = (22);
	data.indices[i++] = (21);
	data.indices[i++] = (20);
	data.indices[i++] = (23);
	data.indices[i++] = (22);

	return data;
}

MeshData MeshBuilder::CreateCylinder(float radius, float height, uint axisDivisions, uint heightDivisions, Color color)
{
	MeshData data;

	uint v = 0, t = 0;
	uint numVerts = 2 + (axisDivisions * (heightDivisions + 1));
	uint numTris = 2 * axisDivisions + 2 * heightDivisions * axisDivisions;
	data.vertices.resize(numVerts);
	data.indices.resize(numTris * 3);

	height *= 0.5f;

	data.vertices[v++].position = Vector3::Up * -height;

	for (uint i = 0; i < axisDivisions; i++)
	{
		float theta = ((float)i / axisDivisions) * PI * 2;
		float x = cosf(theta) * radius;
		float z = sinf(theta) * radius;
		v = CreateVertexLineC(Vector3(x, -height, z), Vector3(x, height, z), heightDivisions, v, data.vertices);
	}

	data.vertices[v++].position = Vector3::Up * height;

	///
	// Indices
	///

	// bottom circle
	uint base = 1;
	for (uint i = 1; i <= axisDivisions; i++)
	{
		data.indices[t++] = 0;
		data.indices[t++] = base;
		data.indices[t++] = base + (heightDivisions + 1);

		base += heightDivisions + 1;
	}

	data.indices[t - 1] = 1;

	// rings
	for (uint i = 1; i <= heightDivisions ; i++)
	{
		t = CreateRing(axisDivisions, heightDivisions + 1, i, t, data.indices);
	}

	// Top circle
	for (uint i = 1; i <= axisDivisions; i++)
	{
		data.indices[t++] = (heightDivisions + 1) * i;
		data.indices[t++] = data.vertices.size() - 1;
		data.indices[t++] = (heightDivisions + 1) * i + (heightDivisions + 1);
	}

	data.indices[t - 1] = heightDivisions + 1;

	for (uint i = 0; i < data.vertices.size(); i++)
	{
		data.vertices[i].color = color;
		data.vertices[i].normal = Vector3::Normalize(data.vertices[i].position);
	}

	return data;
}

uint MeshBuilder::CreateVertexLineC(Vector3 from, Vector3 to, uint steps, uint v, std::vector<MeshVertex>& vertices)
{
	for (uint i = 0; i <= steps; i++)
	{
		vertices[v++].position = Vector3::Lerp(from, to, (float)i / steps);
	}
	return v;
}


MeshData MeshBuilder::CreateCone(float radius, float height, uint axisDivisions, uint heightDivisions, Color color)
{
	MeshData data;

	uint v = 0, t = 0;

	uint numVerts = 2 + (axisDivisions * heightDivisions);
	uint numTris = axisDivisions * ((2 * heightDivisions) - 1) + axisDivisions;
	data.vertices.resize(numVerts);
	data.indices.resize(numTris * 3);

	height *= 0.5f; // centering our cone at the origion

	///
	// Vertices
	///
	// bottom
	data.vertices[v++].position = Vector3::Up * -height;

	// rings
	for (uint i = 0; i < axisDivisions; i++)
	{
		float theta = ((float)i / axisDivisions) * PI * 2;
		float x = cosf(theta) * radius;
		float z = sinf(theta) * radius;
		v = CreateVertexLine(Vector3::Up * -height, Vector3(x, height, z), heightDivisions , v, data.vertices);
	}

	// top
	data.vertices[v++].position = Vector3::Up * height;


	///
	// Indices
	///
	// bottom "ring"
	uint nextAxis;
	for (uint i = 0; i < axisDivisions; i++)
	{
		nextAxis = i * heightDivisions;
		data.indices[t++] = 0;
		data.indices[t++] = nextAxis + 1;
		data.indices[t++] = nextAxis + heightDivisions + 1;
	}
	
	// Set the last value back to one
	data.indices[t - 1] = 1;

	// Remaining "rings"
	for (uint i = 1; i < heightDivisions; i++)
	{
		t = CreateRing(axisDivisions, heightDivisions, i, t, data.indices);
	}

	// Top circle
	for (uint i = 1; i <= axisDivisions; i++)
	{
		data.indices[t++] = heightDivisions * i;
		data.indices[t++] = data.vertices.size() - 1;
		data.indices[t++] = heightDivisions * i + heightDivisions;
	}

	data.indices[t - 1] = heightDivisions;

	for (uint i = 0; i < data.vertices.size(); i++)
	{
		data.vertices[i].color = color;

		// BAD
		data.vertices[i].normal = Vector3::Normalize(data.vertices[i].position);
	}

	return data;
}

uint MeshBuilder::CreateRing(uint axisDivisions, uint heightDivisions, uint h, uint t, std::vector<uint>& indices)
{
	uint base = h;
	for (uint i = 0; i < axisDivisions; i++)
	{
		indices[t++] = base;					 // 1 6			// 0
		indices[t++] = base + 1;				 // 2 7			// 1
		indices[t++] = base + heightDivisions;	 // 6 11		// 2

		indices[t++] = base + heightDivisions; 		// 6 11		// 2
		indices[t++] = base + 1;					// 2 7		// 1
		indices[t++] = base + heightDivisions + 1;	// 7 12		// 3

		base += heightDivisions;			// 6 11
	}

	indices[t - 4] = h;
	indices[t - 3] = h;
	indices[t - 1] = h + 1;

	return t;
}

uint MeshBuilder::CreateInnerTubeRing(uint axisDivisions, uint heightDivisions, uint h, uint t, std::vector<uint>& indices)
{
	uint base = h;
	for (uint i = 0; i < axisDivisions; i++)
	{
		uint next = base + heightDivisions + 1;

		indices[t++] = base;	
		indices[t++] = next + 1;
		indices[t++] = base + 1;

		indices[t++] = base;
		indices[t++] = next; 	
		indices[t++] = next + 1;

		// NOTE: DIFFERENCE FROM CREATE RING IS THE PLUS ONE, CLEAN THIS UP
		base += heightDivisions + 1;			// 6 11			// 2
	}

	indices[t - 5] = h + 1;
	indices[t - 2] = h;
	indices[t - 1] = h + 1;

	return t;
}

uint MeshBuilder::CreateOuterTubeRing(uint axisDivisions, uint heightDivisions, uint h, uint t, std::vector<uint>& indices)
{
	uint base = h;
	for (uint i = 0; i < axisDivisions; i++)
	{
		uint next = base + heightDivisions + 1;

		indices[t++] = base;	
		indices[t++] = base + 1;
		indices[t++] = next;	

		indices[t++] = next; 	
		indices[t++] = base + 1;
		indices[t++] = next + 1;

		// NOTE: DIFFERENCE FROM CREATE RING IS THE PLUS ONE, CLEAN THIS UP
		base += heightDivisions + 1;			// 6 11			// 2
	}

	indices[t - 4] = h;
	indices[t - 3] = h;
	indices[t - 1] = h + 1;

	return t;
}

MeshData MeshBuilder::CreateTube(float outerRadius, float innerRadius, float height, uint axisDivisions, uint heightDivisions, Color color)
{
	MeshData data;

	uint v = 0, t = 0;
	uint numVerts = 4 * axisDivisions + (heightDivisions - 1) * axisDivisions * 2;
	uint numTris = 4 * axisDivisions + 2 * heightDivisions * axisDivisions * 2;
	data.vertices.resize(numVerts);
	data.indices.resize(numTris * 3);

	height *= 0.5f;

	// Inner "Cylinder"
	for (uint i = 0; i < axisDivisions; i++)
	{
		float theta = ((float)i / axisDivisions) * PI * 2;
		float x = cosf(theta) * innerRadius;
		float z = sinf(theta) * innerRadius;
		v = CreateVertexLineC(Vector3(x, -height, z), Vector3(x, height, z), heightDivisions, v, data.vertices);
	}

	// Outer "Cylinder"
	for (uint i = 0; i < axisDivisions; i++)
	{
		float theta = ((float)i / axisDivisions) * PI * 2;
		float x = cosf(theta) * outerRadius;
		float z = sinf(theta) * outerRadius;
		v = CreateVertexLineC(Vector3(x, -height, z), Vector3(x, height, z), heightDivisions, v, data.vertices);
	}

	///
	// Indices
	///

	// Inner "Cylinder"

	// bottom circle
	uint offsetToOuter = axisDivisions * (heightDivisions + 1);
	uint offsetToNext = heightDivisions + 1;
	uint base = 0;
	for (uint i = 0; i < axisDivisions; i++)
	{
		uint next = base + offsetToNext;
		uint outer = base + offsetToOuter;

		// TODO: better way without branching
		if (i == axisDivisions - 1)
		{
			data.indices[t++] = base;
			data.indices[t++] = outer;
			data.indices[t++] = 0;

			data.indices[t++] = 0;
			data.indices[t++] = outer;
			data.indices[t++] = offsetToOuter;
			break;
		}
		data.indices[t++] = base;
		data.indices[t++] = outer;
		data.indices[t++] = next;
							
		data.indices[t++] = next;
		data.indices[t++] = outer;
		data.indices[t++] = outer + offsetToNext;

		base = next;
	}
	//
	// inner rings
	for (uint i = 0; i < heightDivisions; i++)
	{
		t = CreateInnerTubeRing(axisDivisions, heightDivisions, i, t, data.indices);
	}
	
	// outer rings
	for (uint i = 0; i < heightDivisions; i++)
	{
		t = CreateOuterTubeRing(axisDivisions, heightDivisions, offsetToOuter + i, t, data.indices);
	}
	
	
	base = heightDivisions;
	for (uint i = 0; i < axisDivisions; i++)
	{
		uint next = base + offsetToNext;
		uint outer = base + offsetToOuter;

		// TODO: better way without branching
		if (i == axisDivisions - 1)
		{
			data.indices[t++] = base;
			data.indices[t++] = heightDivisions + offsetToOuter;
			data.indices[t++] = outer;

			data.indices[t++] = base;
			data.indices[t++] = heightDivisions;
			data.indices[t++] = heightDivisions + offsetToOuter;
			break;
		}
		data.indices[t++] = base;
		data.indices[t++] = next + offsetToOuter;
		data.indices[t++] = outer;

		data.indices[t++] = base;
		data.indices[t++] = next;
		data.indices[t++] = outer + offsetToNext;

		base = next;
	}
	
	for (uint i = 0; i < data.vertices.size(); i++)
	{
		data.vertices[i].color = color;
		//data.vertices[i].normal = Vector3::Normalize(data.vertices[i].position);
	}

	return data;
}

MeshData MeshBuilder::CreateTorus(float radius, float sectionRadius, uint numDivisions, Color color)
{
	MeshData data;

	uint v = 0, t = 0;
	uint numVerts = numDivisions * numDivisions;
	uint numTris = 2 * numDivisions * numDivisions;
	data.vertices.resize(numVerts);
	data.indices.resize(numTris * 3);

	// Vertices
	for (uint i = 0; i < numDivisions; ++i)
	{
		float theta = ((float)i / numDivisions) * PI * 2;
		v = CreateTorusLoop(theta, radius, sectionRadius, numDivisions, v, data.vertices);
	}

	//Indices
	for (uint i = 0; i < numDivisions; ++i)
	{
		uint base = i * numDivisions;
		for (uint j = 0; j < numDivisions; ++j)
		{
			uint next, next_prev, current_prev;

			i == numDivisions - 1 ? next = j : next = base + numDivisions;
			j == 0 ? (next_prev = next + numDivisions - 1, current_prev = base + numDivisions - 1) : (next_prev = next - 1, current_prev = base - 1);

			data.indices[t++] = base;
			data.indices[t++] = next;
			data.indices[t++] = next_prev;

			data.indices[t++] = base;
			data.indices[t++] = next_prev;
			data.indices[t++] = current_prev;
			base++;
		}
	}

	for (uint i = 0; i < data.vertices.size(); i++)
	{
		data.vertices[i].color = color;
		//data.vertices[i].normal = Vector3::Normalize(data.vertices[i].position);
	}

	return data;
}

uint MeshBuilder::CreateTorusLoop(float theta, float radius, float sectionRadius, uint heightDivisions, uint v, std::vector<MeshVertex>& vertices)
{
	float costheta = cosf(theta);
	float sintheta = sinf(theta);
	
	float offset = 0.0f;

	// bitwise-optimize this
	if (heightDivisions % 2 != 0)
	{
		offset = 1.0f / heightDivisions * PI;
	}
	for (uint i = 0; i < heightDivisions; ++i)
	{
		float phi = ((float)i / heightDivisions) * PI * 2 + offset;
		
		float cosphi = cosf(phi);
		float xy = radius + sectionRadius * cosphi;

		vertices[v++].position = Vector3(costheta * xy, sectionRadius * sinf(phi), sintheta * xy);
	}

	return v;
}

MeshBuilder::MeshBuilder()
{}

NS_END