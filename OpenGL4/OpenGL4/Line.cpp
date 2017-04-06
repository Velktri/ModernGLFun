#include "Line.h"

Line::Line(glm::vec3 Start, glm::vec3 End) {
	type = LINE;

	vertices.push_back(Start.x);
	vertices.push_back(Start.y);
	vertices.push_back(Start.z);

	vertices.push_back(End.x);
	vertices.push_back(End.y);
	vertices.push_back(End.z);

	vertSize = vertices.size() / 3;

	Init();
}


Line::~Line() {

}