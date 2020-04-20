#pragma once

#include <GL\glew.h>

/*
Class that contains all information related to the data within a model
VAO - Vertex Array Object
VBO - Vertex Buffer Object
IBO - Indeces Buffer Object

indexCount - How many indices to draw 
*/
class Mesh
{

public: 
	Mesh();

	/**
	* Compute mesh through the parameters assigned.
	 *
	* @param numOfVertices The number of vertices inside the mesh
	* @param numOfIndices The number of indices inside the mesh
	*/
	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);

	/**
	* Renders the mesh on screen.
	*/
	void RenderMesh();

	/**
	Clear all buffers from the GPU, to avoid memory overflow issues and sets them back to 0.
	It does NOT destroy the class Mesh.
	*/
	void ClearMesh();

	~Mesh();


private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount; 
};

