#pragma once
#include "Light.h"

class SpotLight : public Light {
public:
	SpotLight(Texture* InTexture, glm::vec3 InPosition);
	~SpotLight();
};

