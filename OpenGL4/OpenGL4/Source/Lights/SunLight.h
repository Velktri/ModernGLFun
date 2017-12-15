#pragma once
#include "Light.h"
class SunLight : public Light {
public:
	SunLight(glm::vec3 InPosition, glm::vec3 Position);
	~SunLight();

	glm::vec3 GetDirection();

private:
	glm::vec3 WorldDirection;
};

