#include "Curve.h"



Curve::Curve()
{
	type = CURVE;
	StepSize = 20.0f;

	p0 = glm::vec3(0.0f, 1.0f, 0.0f);
	p1 = glm::vec3(5.0f, 2.0f, 4.0f);
	p2 = glm::vec3(6.0f, 4.0f, 0.0f);

	EvaluateCubic();
	Init();
}


Curve::~Curve()
{
}

void Curve::EvaluateCubic()
{
	int check = 0;
	float step = (1 / StepSize);
	float t = 0 - step;
	for (float u = 0; u <= StepSize; u++)
	{
		t += step;

		Vertex point;
		point.Position = glm::vec3(((1 - t)*(1 - t)*p0.x) + (2 * (1 - t) * t* p1.x) + (t*t*p2.x),
			((1 - t)*(1 - t)*p0.y) + (2 * (1 - t) * t* p1.y) + (t*t*p2.y),
								   ((1 - t)*(1 - t)*p0.z) + (2 * (1 - t) * t* p1.z) + (t*t*p2.z));

		vertices.push_back(point);
		check++;
	}
}

void Curve::UpdateControlPoint(int index, glm::vec3 InPosition)
{
	if (index == 1)
	{
		p0 += InPosition;
	}
	else 	if (index == 2)
	{
		p1 += InPosition;
	}
	else	if (index == 3)
	{
		p2 += InPosition;
	}
	vertices.clear();
	EvaluateCubic();


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);
}


