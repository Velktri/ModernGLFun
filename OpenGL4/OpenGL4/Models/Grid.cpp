#include "Grid.h"

Grid::Grid(int RowRadius, int ColumnRadius, float spacing) {
	type = GRID;

	for (int i = -1 * RowRadius; i <= RowRadius; i++) {
		float GridDistance = i * spacing;
		float startPoint = (-1 * ColumnRadius) * spacing;
		float endPoint = (ColumnRadius) * spacing;

		/* Start */
		vertices.push_back(GridDistance);  // x
		vertices.push_back(0.0f); // y
		vertices.push_back(startPoint); // z

		/* End */
		vertices.push_back(GridDistance); // x
		vertices.push_back(0.0f); // y
		vertices.push_back(endPoint); // z
	}

	for (int i = -1 * ColumnRadius; i <= ColumnRadius; i++) {
		float GridDistance = i * spacing;
		float startPoint = (-1 * RowRadius) * spacing;
		float endPoint = (RowRadius) * spacing;

		/* Start */
		vertices.push_back(startPoint); // x
		vertices.push_back(0.0f); // y
		vertices.push_back(GridDistance); // z

		/* End */
		vertices.push_back(endPoint); // x
		vertices.push_back(0.0f); // y
		vertices.push_back(GridDistance); // z
	}

	Init();
}


Grid::~Grid() {

}