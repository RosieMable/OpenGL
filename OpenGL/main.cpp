#include <stdio.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
//Window Dimensions
const GLint WIDTH = 800, HEIGHT = 600;


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

	//Loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get + handle user input events
		glfwPollEvents();

		//Clear window
		glClearColor(0.57f, 0.30f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Swaps the back scene (the one that has been just drawn) with the front scene (the one that is there, while the back scene is drawn)
		glfwSwapBuffers(mainWindow);
	}

	return 0;
}