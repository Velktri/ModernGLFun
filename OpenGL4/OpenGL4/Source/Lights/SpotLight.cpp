#include "SpotLight.h"

SpotLight::SpotLight(Texture* InTexture, glm::vec3 InPosition) : Light(InTexture, InPosition) {
	//BuildLight();
}


SpotLight::~SpotLight() {
}
