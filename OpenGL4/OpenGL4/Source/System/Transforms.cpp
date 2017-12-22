#include "Transforms.h"



Transforms::Transforms()
{
	RelativePosition = glm::vec3();
	RelativeRotation = glm::vec3();
	RelativeScale = glm::vec3(1.0f, 1.0f, 1.0f);

	OriginPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	WorldSpaceOrientation = glm::mat4();
}


Transforms::~Transforms()
{
}

void Transforms::Translate(glm::vec3 InVector)
{
	WorldSpaceOrientation = glm::translate(WorldSpaceOrientation, InVector);
	OriginPoint += InVector;
}

void Transforms::Rotate(glm::vec3 InVector)
{
	if (InVector.x > 0) { WorldSpaceOrientation = glm::rotate(WorldSpaceOrientation, glm::radians(InVector.x), glm::vec3(1.0f, 0.0f, 0.0f)); }
	if (InVector.y > 0) { WorldSpaceOrientation = glm::rotate(WorldSpaceOrientation, glm::radians(InVector.y), glm::vec3(0.0f, 1.0f, 0.0f)); }
	if (InVector.z > 0) { WorldSpaceOrientation = glm::rotate(WorldSpaceOrientation, glm::radians(InVector.z), glm::vec3(0.0f, 0.0f, 1.0f)); }
}

void Transforms::Scale(glm::vec3 InVector)
{
	WorldSpaceOrientation = glm::scale(WorldSpaceOrientation, InVector);
}

glm::vec3 Transforms::GetPosition()
{
	return glm::vec3(WorldSpaceOrientation[3][0], WorldSpaceOrientation[3][1], WorldSpaceOrientation[3][2]);
}
