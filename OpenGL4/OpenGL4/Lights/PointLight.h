#pragma once
#include "Light.h"
class PointLight : public Light {
public:
	PointLight(glm::vec3 Position);
	~PointLight();
};

