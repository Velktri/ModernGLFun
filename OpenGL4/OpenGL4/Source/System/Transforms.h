#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transforms
{
public:
	Transforms();
	~Transforms();

	void Translate(float x, float y, float z);
	void Rotate(float x, float y, float z);
	void Scale(float x, float y, float z);

	glm::vec3 OriginPoint;
	glm::mat4 WorldSpaceOrientation;
};

