#pragma once
#include "Light.h"
class SunLight : public Light
{
public:
	SunLight(Texture* InTexture, glm::vec3 InPosition);
	~SunLight();

	glm::vec3 GetDirection();

private:
	glm::vec3 WorldDirection;
};

