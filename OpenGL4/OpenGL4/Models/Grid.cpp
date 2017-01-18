#include "Grid.h"

Grid::Grid(int RowRadius, int ColumnRadius, float spacing) {
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

	vertSize = vertices.size() / 3;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices), &vertices.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}


Grid::~Grid() {

}

void Grid::Draw(Shader* shader, Camera* worldCamera) {
	shader->Use();
	glUniformMatrix4fv(shader->ShaderList["model"], 1, GL_FALSE, glm::value_ptr(glm::mat4()));
	glUniformMatrix4fv(shader->ShaderList["view"], 1, GL_FALSE, glm::value_ptr(worldCamera->GetViewMatrix()));
	glUniformMatrix4fv(shader->ShaderList["projection"], 1, GL_FALSE, glm::value_ptr(worldCamera->GetProjection()));

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, vertSize);
}
