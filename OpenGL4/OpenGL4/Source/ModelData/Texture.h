#pragma once
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

class Texture {
public:
	Texture(std::string texturePath);
	~Texture();

	GLuint GetTexture();

private:
	GLuint texture;
	glm::vec2 TextureDimensions;
	std::vector<unsigned char> imageRGBA;
};

