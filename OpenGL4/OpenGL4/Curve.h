#pragma once
#include "Entity.h"

class Curve : public Entity {
public:
	Curve();
	~Curve();

	glm::vec3 p0;
	glm::vec3 p1;
	glm::vec3 p2;


	void UpdateControlPoint(int index, glm::vec3 InPosition);

private:
	void EvaluateCubic();
	float StepSize;
};

