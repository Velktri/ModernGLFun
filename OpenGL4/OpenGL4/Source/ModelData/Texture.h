#pragma once
#include "ModelData/Resource.h"
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

class Texture : public Resource 
{
public:
	Texture(std::string texturePath);
	~Texture();

	GLuint GetTexture();
	glm::vec2 GetTextureSize();

private:
	GLuint texture;
	glm::vec2 TextureDimensions;
	std::vector<unsigned char> imageRGBA;
};

