#pragma once
#include "Element.h"

class Line : public Element
{
public:
	Line(glm::vec3 Start, glm::vec3 End);
	~Line();
};

