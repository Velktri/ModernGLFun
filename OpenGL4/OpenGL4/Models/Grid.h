#pragma once
#include "../Entity.h"

class Grid : public Entity {
public:
	Grid(int RowRadius, int ColumnRadius, float spacing);
	~Grid();
};