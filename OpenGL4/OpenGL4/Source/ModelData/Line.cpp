#include "Line.h"

Line::Line(glm::vec3 Start, glm::vec3 End) {
	type = LINE;
	Vertex start, end;
	start.Position = Start;
	end.Position = End;
	vertices.push_back(start);
	vertices.push_back(end);

	Init();
}


Line::~Line() {

}