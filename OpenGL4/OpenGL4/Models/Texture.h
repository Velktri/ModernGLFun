#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>

class Texture {
public:
	Texture(std::string texturePath);
	~Texture();

	GLuint GetTexture();
	std::string GetType();
	void SetType(std::string InType);

private:
	GLuint texture;
	std::string type;
	unsigned textureWidth, textureHeight;
	std::vector<unsigned char> imageRGBA;
};

