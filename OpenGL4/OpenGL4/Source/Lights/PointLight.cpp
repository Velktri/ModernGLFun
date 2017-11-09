#include "PointLight.h"

PointLight::PointLight(glm::vec3 Position) {
	WorldPosition = Position;
	BuildLight();
}


PointLight::~PointLight() {
}
