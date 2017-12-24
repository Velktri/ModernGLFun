#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight(Texture* InTexture, glm::vec3 InPosition);
	~PointLight();
};