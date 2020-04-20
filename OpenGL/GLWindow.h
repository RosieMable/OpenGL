#pragma once

#include <stdio.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>


class GLWindow
{
public:
	GLWindow();

	GLWindow(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	/*
	Swaps the back scene (the one that has been just drawn) with the front scene (the one that is there, while the back scene is drawn)
	*/
	void swapBuffer() { glfwSwapBuffers(mainWindow); }

	~GLWindow();

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;
};

