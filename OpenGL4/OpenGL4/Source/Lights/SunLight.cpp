#include "SunLight.h"

SunLight::SunLight(Texture* InTexture, glm::vec3 InPosition) : Light(InTexture, InPosition)
{
	WorldDirection = glm::vec3();
}


SunLight::~SunLight()
{

}

glm::vec3 SunLight::GetDirection() { return WorldDirection; }