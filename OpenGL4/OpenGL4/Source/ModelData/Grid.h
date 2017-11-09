#pragma once
#include "Element.h"

class Grid : public Element {
public:
	Grid(int RowRadius, int ColumnRadius, float spacing);
	~Grid();
};