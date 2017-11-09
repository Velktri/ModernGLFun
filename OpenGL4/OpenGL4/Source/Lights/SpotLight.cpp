#include "SpotLight.h"

SpotLight::SpotLight(glm::vec3 Position) {
	WorldPosition = Position;
	BuildLight();
}


SpotLight::~SpotLight() {
}
