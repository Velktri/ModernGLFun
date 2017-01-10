#pragma once
#include "Model.h"
class Grid : public Model {
public:
	Grid(int RowRadius, int ColumnRadius, float spacing);
	~Grid();
};

