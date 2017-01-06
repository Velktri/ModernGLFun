#include "Shader.h"

Shader::Shader(const char* vertFile, const char* fragFile) {
	std::string tempvert = ReadShaderFile(vertFile);
	std::string tempfrag = ReadShaderFile(fragFile);
	const GLchar* vertSource = tempvert.c_str();
	const GLchar* fragSource = tempfrag.c_str();

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertSource, NULL);
	glCompileShader(vertShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragSource, NULL);
	glCompileShader(fragShader);

	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, vertShader);
	glAttachShader(ShaderProgram, fragShader);
	glBindFragDataLocation(ShaderProgram, 0, "outColor");
	glLinkProgram(ShaderProgram);

	glDeleteShader(fragShader);
	glDeleteShader(vertShader);
}


Shader::~Shader() {
	glDeleteProgram(ShaderProgram);
}


std::string Shader::ReadShaderFile(const char* filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();

	return content;
}

void Shader::Use() { glUseProgram(this->ShaderProgram); }

GLuint Shader::GetShader() {
	return ShaderProgram;
}
