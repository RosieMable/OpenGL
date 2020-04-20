#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "GLWindow.h"
#include "Mesh.h"
#include "Shader.h"


const float toRadians = 3.14159265f / 180.0f; //equation to convert degrees to radians


GLWindow mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

//Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1, //side 
		1, 3, 2, //side
		2, 3, 0, //front side
		0, 1, 2 //base
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1); //To push back to the end of a list

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj2); //To push back to the end of a list
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(shader1);
}


int main() {

	mainWindow = GLWindow(800, 600);
	mainWindow.Initialise();

	//Create the objects and draw it
	CreateObjects();
	CreateShaders();

	GLuint uniformModel = 0, uniformProjection = 0;

	//glm perspective tells that we want a perspective matrix
	//param 1 - field of view in degrees onto y axis
	//param 2 - aspect ratio, the width of the window/the height of the window
	//param 3 - nearest field of view, what distance is the nearest thing that we can see at
	//param 4 - the furthest field of view, what is the max distance our camera can perceive objects at
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		// Get + handle user input events
		glfwPollEvents();

		//Clear window
		glClearColor(0.57f, 0.30f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Clearing both the colour and depth buffer bit

		//Asks the GPU to run the shader program with the chosen id
		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();

		//Var type of a matrix4x4 (identity matrix, all values are zeros besides the diagonal one)
		glm::mat4 model(1.0f);



		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f)); //translation to the identity matrix by a precise vector 3 
		model = glm::scale(model, glm::vec3(.4f, .4f, 1.0f));
		

		//assign value to the shader program
		//the value pointer because we need a raw format of the value model that will work with the shader
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(.4f, .4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		//Unassign the shader program
		glUseProgram(0);

		
		mainWindow.swapBuffer();
	}

	return 0;
}