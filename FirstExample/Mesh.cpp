#include "Mesh.h"
#include "glm\glm.hpp"

Mesh::Mesh()
{
	m_VAO = 0;
	m_VBO = 0;
	m_IBO = 0;
	m_indexCount = 0;
	m_vertexCount = 0;
	m_vertexLength = 8;
}
Mesh::~Mesh()
{
	
}

void Mesh::CreateMesh(GLfloat* vertices, GLshort* indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	//Get ID and Create VAO and bind it
	//A Vertex Array Object(VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store the information for a complete rendered object.
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	//Create Index Buffer
	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

	//Get ID and Create VBO, bind it to Array Buufer, copy data to array buffer
	//A Vertex Buffer Object(VBO) is a memory buffer in the high speed memory of your video card designed to hold information about vertices.In our example we have two VBOs, one that describes the coordinates of our verticesand another that describes the color associated with each vertex.VBOs can also store information such as normals, texcoords, indicies,
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices * m_vertexLength, vertices, GL_STATIC_DRAW);

	//Vertex attributes can be described for vertex shader, tell location0 is for vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
	//enable location 0 in vertex shader
	glEnableVertexAttribArray(0);

	//5th paramter: stride, where the next value start
	//6th paramter: offset, where the first element start
	//tell location1 is for tex coordinate
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);

	//tell location2 is for normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	//unbind array buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//unbind VAO
	glBindVertexArray(0);

	//unbind index buffer after unbind VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::calcAverageNormals(GLshort* indices, unsigned int indiceCount,
	GLfloat* vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
{
	for (int i = 0; i < indiceCount; i += 3)
	{
		//Get the location of vertex in vertex array(vertices) using index calculation
		unsigned int in0 = indices[i] * vLength;		//First vertex's index of vertex array
		unsigned int in1 = indices[i + 1] * vLength;	//Second vertex's index of vertex array
		unsigned int in2 = indices[i + 2] * vLength;	//Third vertex's index of vertex array

		//Get the line vector between two vectexs
		//Calculate vector from First vertex to Second vertex
		glm::vec3 v1(vertices[in1] - vertices[in0],
			vertices[in1 + 1] - vertices[in0 + 1],
			vertices[in1 + 2] - vertices[in0 + 2]);
		//Calculate vector from First vertex to Third vertex
		glm::vec3 v2(vertices[in2] - vertices[in0],
			vertices[in2 + 1] - vertices[in0 + 1],
			vertices[in2 + 2] - vertices[in0 + 2]);

		//Calculate perpendicular vector of two vectors(v1, v2) which is face normal
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		//Find the index of normal in vertex array
		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		//Add calculated normal to each vertex's normal
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	//Normalize each vertex's normal
	for (int i = 0; i < verticeCount; ++i)
	{
		//Find index of normal in vertex array
		unsigned int nOffset = i * vLength + normalOffset;

		//Normalize the vertex's normal
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);

		//Set normalized normal
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void Mesh::RenderMesh()
{
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
	if (m_IBO != 0)
	{
		glDeleteBuffers(1, &m_IBO);
		m_IBO = 0;
	}

	if (m_VBO != 0)
	{
		glDeleteBuffers(1, &m_VBO);
		m_VBO = 0;
	}

	if (m_VAO != 0)
	{
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}

	m_indexCount = 0;
}
