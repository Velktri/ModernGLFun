#include "Curve.h"



Curve::Curve() {
	type = CURVE;
	float StepSize = 20;

	glm::vec3 p0 = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 p1 = glm::vec3(5.0f, 2.0f, 4.0f);
	glm::vec3 p2 = glm::vec3(6.0f, 4.0f, 0.0f);

	for (float t = 0; t <= 1; t += (1 / StepSize)) {
		vertices.push_back(((1 - t)*(1 - t)*p0.x) + (2 * (1 - t) * t* p1.x) + (t*t*p2.x));
		vertices.push_back(((1 - t)*(1 - t)*p0.y) + (2 * (1 - t) * t* p1.y) + (t*t*p2.y));
		vertices.push_back(((1 - t)*(1 - t)*p0.z) + (2 * (1 - t) * t* p1.z) + (t*t*p2.z));
	}

	Init();
}


Curve::~Curve() {
}
