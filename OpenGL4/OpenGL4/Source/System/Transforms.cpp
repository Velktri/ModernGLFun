#include "Transforms.h"



Transforms::Transforms()
{
	OriginPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	WorldSpaceOrientation = glm::mat4();
}


Transforms::~Transforms()
{
}

void Transforms::Translate(float x, float y, float z)
{
	WorldSpaceOrientation = glm::translate(WorldSpaceOrientation, glm::vec3(x, y, z));
	OriginPoint = glm::vec3(OriginPoint.x + x, OriginPoint.y + y, OriginPoint.z + z);
}

void Transforms::Rotate(float x, float y, float z)
{
	if (x > 0) { WorldSpaceOrientation = glm::rotate(WorldSpaceOrientation, glm::radians(x), glm::vec3(1.0f, 0.0f, 0.0f)); }
	if (y > 0) { WorldSpaceOrientation = glm::rotate(WorldSpaceOrientation, glm::radians(y), glm::vec3(0.0f, 1.0f, 0.0f)); }
	if (z > 0) { WorldSpaceOrientation = glm::rotate(WorldSpaceOrientation, glm::radians(z), glm::vec3(0.0f, 0.0f, 1.0f)); }
}

void Transforms::Scale(float x, float y, float z)
{
	WorldSpaceOrientation = glm::scale(WorldSpaceOrientation, glm::vec3(x, y, z));
}