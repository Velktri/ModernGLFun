#include "PointLight.h"

PointLight::PointLight(glm::vec3 InPosition) : Light(InPosition) {
	WorldPosition = InPosition;
	BuildLight();
}


PointLight::~PointLight() {
}
