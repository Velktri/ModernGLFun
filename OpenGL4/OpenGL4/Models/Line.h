#pragma once
#include "Entity.h"

class Line : public Entity {
public:
	Line(glm::vec3 Start, glm::vec3 End);
	~Line();
};

