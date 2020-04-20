#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexCodeLocation, const char* fragmentcodeLocation)
{
	//Reading the file and storing it in a string
	std::string vertexString = ReadFile(vertexCodeLocation);
	std::string fragmentString = ReadFile(fragmentcodeLocation);

	//Transform string into const char
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	//compiles the shader
	CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open())
	{
		printf("Fail to read %s! File does not exist.", fileLocation);
		return "";
	}

	std::string line = "";
	//while is not the end of the file
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

GLuint Shader::GetProjectionLocation()
{
	return uniformProjection;
}

GLuint Shader::GetModelLocation()
{
	return uniformModel;
}

void Shader::UseShader()
{
	glUseProgram(shaderID);
}

void Shader::ClearShader()
{
	if (shaderID != 0) 
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
}

Shader::~Shader()
{
	ClearShader();
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	//Creating the shader program and assing it to the ID GLuint Shader
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Shader not created :(");
		return;
	}

	//Adding the shader to the program
	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	//If there are any shaders error, they will be logged here
	GLint result = 0;
	GLchar elog[1024] = { 0 };

	//Links the program on the graphic card (creates an exe for the GPU)
	glLinkProgram(shaderID);

	//checking if the shader has been linked or not
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(elog), NULL, elog);
		//Prints where the linking issue has accured
		printf("Error linking program: %s \n", elog);
		return;
	}

	//Validate the program
	glValidateProgram(shaderID);

	//checking if the shader has been validated or not
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(elog), NULL, elog);
		//Prints where the linking issue has accured
		printf("Error validating program: %s \n", elog);
		return;
	}

	uniformModel = glGetUniformLocation(shaderID, "model"); //gets the uniform variable of model and binds it to model var
	uniformProjection = glGetUniformLocation(shaderID, "projection");

}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
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
