#pragma once
#include <GL/glew.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>

class Shader {
public:
	Shader(const char* vertSource, const char* fragSource);
	~Shader();


	void Use();
	GLuint GetShader();

private:
	std::string ReadShaderFile(const char* filePath);
	GLuint ShaderProgram;
};

