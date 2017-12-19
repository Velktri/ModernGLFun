#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transforms
{
public:
	Transforms();
	~Transforms();

	void Translate(glm::vec3 InVector);
	void Rotate(glm::vec3 InVector);
	void Scale(glm::vec3 InVector);

	glm::vec3 OriginPoint;
	glm::mat4 WorldSpaceOrientation;

	glm::vec3 RelativePosition;
	glm::vec3 RelativeRotation;
	glm::vec3 RelativeScale;
};

