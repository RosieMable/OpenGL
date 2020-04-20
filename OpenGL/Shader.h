#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>

class Shader
{
public:
	Shader();

	/*
	Creating a shader program from reading a string.
	@param vertexCode String containing the code for the vertex shader.
	@param fragmentCode String containing the code for the fragment shader.
	*/
	void CreateFromString(const char* vertexCode, const char* fragmentCode);

	/*Creating a shader program from reading the vertex and fragment shader files*/
	void CreateFromFiles(const char* vertexCodeLocation, const char* fragmentcodeLocation);

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();

	GLuint GetModelLocation();

	void UseShader();

	/**
	Clear shader program from the GPU, to avoid memory overflow issues and sets them back to 0.
	It does NOT destroy the class Shader.
	*/
	void ClearShader();

	~Shader();

private:
	GLuint shaderID, uniformProjection, uniformModel;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

