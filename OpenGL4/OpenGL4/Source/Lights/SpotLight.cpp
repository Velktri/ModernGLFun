#include "SpotLight.h"

SpotLight::SpotLight(glm::vec3 InPosition) : Light(InPosition) {
	WorldPosition = InPosition;
	BuildLight();
}


SpotLight::~SpotLight() {
}
