#include <stdio.h>
#include <string.h>
#include <cmath>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

//Window Dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f; //equation to convert degrees to radians

GLuint VAO, VBO, IBO, shader, uniformModel, uniformProjection; //uniformModel will allow us to translate the model coordinates to the world coordinates
//IBO = Index Buffer Object for indexed Draws

//Control the movement of the triangle
bool direction = true;

float triOffeset = 0.0f; //triangle will start at 0
float triMaxOffset = 0.7f; 
float triIncrement = 0.005f; //increment the position by this value

float curAngle = 0.0f;


// Vertex Shader
static const char* vShader = "				\n\
#version 330								\n\
											\n\
layout (location = 0) in vec3 pos;			\n\
											\n\
out vec4 vCol; \n\
uniform mat4 model;											\n\
uniform mat4 projection;											\n\
											\n\
void main()									\n\
{											\n\
	gl_Position = projection * model * vec4(pos, 1.0);			\n\
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f); \n\
}";

//Fragment Shader
static const char* fShader = "				\n\
#version 330								\n\
											\n\
in vec4 vCol;								\n\
out vec4 colour;								\n\
											\n\
void main()									\n\
{											\n\
	colour = vCol;			\n\
}";

void CreateTriangle()
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//Creating the VBO buffer and binding it to the variable VBO (vertex buffer object)
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Passing the data we want to draw to the buffer
	//1 param - which buffer we are using to draw it
	//2 param - size of the data we are drawing
	//3 param - the data we are drawing
	//4 param - the drawing mode 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//Unbinding the VAO and VBO and IBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) 
{
	//Creates an empty shader of the type of the parameter
	GLuint theShader = glCreateShader(shaderType);

	//Passing the shader code memory allocation
	//Pointer to the first element of the code
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	//Passing the shader code length
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	//If there are any shaders error, they will be logged here
	GLint result = 0;
	GLchar elog[1024] = { 0 };

	//checking if the shader has been compiled correctly or not
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(elog), NULL, elog);
		//Prints where the compiling issue has accured
		printf("Error compiling the %d shader: %s \n", shaderType, elog);
		return;
	}
	
	//Attaching the shader to the program
	glAttachShader(theProgram, theShader);
}

void CompileShaders() {

	//Creating the shader program and assing it to the ID GLuint Shader
	shader = glCreateProgram();

	if (!shader)
	{
		printf("Shader not created :(");
		return;
	}

	//Adding the shader to the program
	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	//If there are any shaders error, they will be logged here
	GLint result = 0;
	GLchar elog[1024] = { 0 };

	//Links the program on the graphic card (creates an exe for the GPU)
	glLinkProgram(shader);

	//checking if the shader has been linked or not
	glGetProgramiv(shader, GL_LINK_STATUS, &result); 

	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
		//Prints where the linking issue has accured
		printf("Error linking program: %s \n", elog);
		return;
	}

	//Validate the program
	glValidateProgram(shader);

	//checking if the shader has been validated or not
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
		//Prints where the linking issue has accured
		printf("Error validating program: %s \n", elog);
		return;
	}

	uniformModel = glGetUniformLocation(shader, "model"); //gets the uniform variable of model and binds it to model var
	uniformProjection = glGetUniformLocation(shader, "projection");

}


int main() {

	//Initialise GLFW
	if (!glfwInit()) 
	{

		printf("GLFW Initialisation failed!");
		glfwTerminate();
		return 1; 
		//1 is the usual number that is returned in C++ to say 
		//that there has been an error
	}

	//Setup GLFW window properties
	//OpenGL Version => Setting it up to be version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Setting the OpenGL profile (profile defines how the code is going to be treated/passed)
	//Using the core profile, because we do not want to use any deprecated features
	//No backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Make it foward compatible
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	if (!mainWindow) {
		printf("GLFW window creation failed :c");
		glfwTerminate();
		return 1;
	}

	//Get Buffer size information
	//Buffer is the space within the window, that is going to hold the OpenGL data
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//Set context for GLEW to use
	//Select the window where to draw stuff
	glfwMakeContextCurrent(mainWindow);

	//Allow modern extensions features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		printf("Glew initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	//Setting up the depth testing for the depth buffer to determine which pixels to draw first 
	glEnable(GL_DEPTH_TEST);

	//Setup Viewport Size
	glViewport(0, 0, bufferWidth, bufferHeight);

	//Create the triangle and draw it
	CreateTriangle();
	CompileShaders();

	//glm perspective tells that we want a perspective matrix
	//param 1 - field of view in degrees onto y axis
	//param 2 - aspect ratio, the width of the window/the height of the window
	//param 3 - nearest field of view, what distance is the nearest thing that we can see at
	//param 4 - the furthest field of view, what is the max distance our camera can perceive objects at
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

	//Loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get + handle user input events
		glfwPollEvents();

		if (direction)
		{
			//if heading to the right
			triOffeset += triIncrement;

		}
		else
		{
			//if heading to the left
			triOffeset -= triIncrement;
		}

		if (abs(triOffeset) >= triMaxOffset) { 
			//if the abs value of triangle offset is more or equal to the triangle max offset value, then switch direction
			direction = !direction;
		}

		curAngle += 0.1f;
		if (curAngle >= 360)
		{
			curAngle -= 360;
		}

		//Clear window
		glClearColor(0.57f, 0.30f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Clearing both the colour and depth buffer bit

		//Asks the GPU to run the shader program with the chosen id
		glUseProgram(shader);

		//Var type of a matrix4x4 (identity matrix, all values are zeros besides the diagonal one)
		glm::mat4 model(1.0f);



		model = glm::translate(model, glm::vec3(0.0f, triOffeset, -2.5f)); //translation to the identity matrix by a precise vector 3 
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(.4f, .4f, 1.0f));
		

		//assign value to the shader program
		//the value pointer because we need a raw format of the value model that will work with the shader
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));


		//Binding that shader program to the a specific VAO
		glBindVertexArray(VAO);
		//binding the shader program to specific IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		//Unbinding the VAO
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//Unassign the shader program
		glUseProgram(0);

		//Swaps the back scene (the one that has been just drawn) with the front scene (the one that is there, while the back scene is drawn)
		glfwSwapBuffers(mainWindow);
	}

	return 0;
}