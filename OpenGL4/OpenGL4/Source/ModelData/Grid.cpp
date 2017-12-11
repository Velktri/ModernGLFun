#include "Grid.h"

Grid::Grid(int RowRadius, int ColumnRadius, float spacing)
{
	type = GRID;
	for (int i = -1 * RowRadius; i <= RowRadius; i++)
	{
		float GridDistance = i * spacing;
		float startPoint = (-1 * ColumnRadius) * spacing;
		float endPoint = (ColumnRadius) * spacing;

		Vertex start, end;
		start.Position = glm::vec3(GridDistance, 0.0f, startPoint);
		end.Position = glm::vec3(GridDistance, 0.0f, endPoint);
		vertices.push_back(start);
		vertices.push_back(end);
	}

	for (int i = -1 * ColumnRadius; i <= ColumnRadius; i++)
	{
		float GridDistance = i * spacing;
		float startPoint = (-1 * RowRadius) * spacing;
		float endPoint = (RowRadius) * spacing;

		Vertex start, end;
		start.Position = glm::vec3(startPoint, 0.0f, GridDistance);
		end.Position = glm::vec3(endPoint, 0.0f, GridDistance);
		vertices.push_back(start);
		vertices.push_back(end);
	}

	Color = glm::vec3(0.25f, 0.25f, 0.25f);

	Init();
}


Grid::~Grid()
{

}