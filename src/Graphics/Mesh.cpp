#include "Mesh.hpp"
#include "Data.hpp"

NS_BEGIN

Mesh::Mesh(MeshData& data)
{
	///
	// Vertex Buffer
	///
	numVertices = data.vertices.size();

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(MeshVertex) * numVertices, &data.vertices[0], GL_STATIC_DRAW);

	// Setup MeshVertex data
	/* Position: */ glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, 0);
	/* Texcoord */  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (void*)(sizeof(float) * 3));
	/* Color: */	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (void*)(sizeof(float) * 5));
	/* Normal: */	glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE,  sizeof(float) * 15, (void*)(sizeof(float) * 9));
	/* Tangent: */	glVertexAttribPointer(4, 3, GL_FLOAT, GL_TRUE,  sizeof(float) * 15, (void*)(sizeof(float) * 12));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	///
	// Index Buffer
	///
	numIndices = data.indices.size();

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * numIndices, &data.indices[0], GL_STATIC_DRAW);
}

Mesh::Mesh(const Mesh& mesh)
{
	vertexBuffer = mesh.vertexBuffer;
	vertexArray = mesh.vertexArray;
	indexBuffer = mesh.indexBuffer;
	numVertices = mesh.numVertices;
	numIndices = mesh.numIndices;
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
}

Mesh& Mesh::operator=(const Mesh& mesh)
{ 
	vertexBuffer = mesh.vertexBuffer;
	vertexArray = mesh.vertexArray;
	indexBuffer = mesh.indexBuffer;
	numVertices = mesh.numVertices;
	numIndices = mesh.numIndices;
	return *this;
}

void Mesh::Bind()
{
	glBindVertexArray(vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
}

void Mesh::Draw()
{
	Bind();
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, (void*)0);
}

NS_END