#include "Mesh.h"


Mesh::Mesh()
{
	//declaring the data we need
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	indexCount = numOfIndices;


	//Creating the VAO  and binding to the variable VAO (vertex array object)
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Creating the IBO buffer and binding it to the variable IBO (Index Buffer Object)
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); //making array buffer for the indeces

	//Passing the data we want to draw to the buffer
	//1 param - which buffer we are using to draw it
	//2 param - size of the data we are drawing
	//3 param - the data we are drawing
	//4 param - the drawing mode 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);


	//Creating the VBO buffer and binding it to the variable VBO (vertex buffer object)
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Passing the data we want to draw to the buffer
	//1 param - which buffer we are using to draw it
	//2 param - size of the data we are drawing
	//3 param - the data we are drawing
	//4 param - the drawing mode 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//Unbinding the VAO and VBO and IBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::RenderMesh()
{
	//If there is nothing to draw, then return
	if (VAO == 0 || VBO == 0 || IBO == 0)
		return;
	
	//Binding that shader program to the a specific VAO
	glBindVertexArray(VAO);
	//binding the shader program to specific IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	//Unbinding the VAO
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}


void Mesh::ClearMesh()
{
	if (IBO != 0)
	{
		//deletes the buffer off the graphics card to free up space - to avoid memory overflow
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	if (VBO != 0)
	{
		//deletes the buffer off the graphics card to free up space - to avoid memory overflow
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0)
	{
		//deletes the buffer off the graphics card to free up space - to avoid memory overflow
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}

Mesh::~Mesh()
{
	ClearMesh();
}
