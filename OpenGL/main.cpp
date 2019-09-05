#include <stdio.h>
#include <string.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

//Window Dimensions
const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader;

// Vertex Shader
static const char* vShader = "				\n\
#version 330								\n\
											\n\
layout (location = 0) in vec3 pos;			\n\
											\n\
void main()									\n\
{											\n\
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);			\n\
}";

//Fragment Shader
static const char* fShader = "				\n\
#version 330								\n\
											\n\
out vec4 colour;								\n\
											\n\
void main()									\n\
{											\n\
	colour = vec4(0.3, 0.0, 0.45, 1.0);			\n\
}";

void CreateTriangle()
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	//Creating the VAO and binding to the variable VAO (vertex array object)
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Creating the VBO and binding it to the variable VBO (vertex buffer object)
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

	//Unbinding the VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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

	//Setup Viewport Size
	glViewport(0, 0, bufferWidth, bufferHeight);

	//Create the triangle and draw it
	CreateTriangle();
	CompileShaders();

	//Loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get + handle user input events
		glfwPollEvents();

		//Clear window
		glClearColor(0.57f, 0.30f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Asks the GPU to run the shader program with the chosen id
		glUseProgram(shader);

		//Binding that shader program to the a specific VAO
		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Unbinding the VAO
		glBindVertexArray(0);

		//Unassign the shader program
		glUseProgram(0);

		//Swaps the back scene (the one that has been just drawn) with the front scene (the one that is there, while the back scene is drawn)
		glfwSwapBuffers(mainWindow);
	}

	return 0;
}