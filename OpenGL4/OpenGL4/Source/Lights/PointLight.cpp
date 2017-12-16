#include "PointLight.h"

PointLight::PointLight(Texture* InTexture, glm::vec3 InPosition) : Light(InTexture, InPosition) {
	//BuildLight();
}


PointLight::~PointLight() {
}
