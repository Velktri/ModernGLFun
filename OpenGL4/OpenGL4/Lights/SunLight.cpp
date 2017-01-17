#include "SunLight.h"



SunLight::SunLight(glm::vec3 Direction) {
	WorldDirection = Direction;
	BuildLight();
}


SunLight::~SunLight() {

}

glm::vec3 SunLight::GetDirection() {
	return WorldDirection;
}
