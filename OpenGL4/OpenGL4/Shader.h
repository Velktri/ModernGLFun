#pragma once
#include <GL/glew.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>

struct ShaderUniforms {
	std::string Type;
	std::string Name;
};

class Shader {
public:
	Shader(const char* vertSource, const char* fragSource);
	~Shader();


	void Use();
	GLuint GetShader();

	std::unordered_map<std::string, GLuint> ShaderList;
	std::unordered_map<std::string, std::vector<std::string>> StructList;

private:
	std::string ReadShaderFile(const char* filePath);
	void CheckUniform(std::string line);
	bool ParseStructs(std::string Shader);
	GLuint ShaderProgram;
	std::vector<ShaderUniforms> UniformList;
};

