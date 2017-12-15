#include "SunLight.h"

SunLight::SunLight(glm::vec3 InPosition, glm::vec3 Direction) : Light(InPosition) {
	WorldPosition = InPosition;
	WorldDirection = Direction;
	BuildLight();
}


SunLight::~SunLight() {

}

glm::vec3 SunLight::GetDirection() {
	return WorldDirection;
}
