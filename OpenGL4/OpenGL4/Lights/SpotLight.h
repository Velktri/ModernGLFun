#pragma once
#include "Light.h"
class SpotLight : public Light {
public:
	SpotLight(glm::vec3 Position);
	~SpotLight();
};

