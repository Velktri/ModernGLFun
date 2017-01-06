#include <GL/glew.h>
#include <SDL.h>
#include <SDL_main.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include "Model.h"
#include "Shader.h"

int main(int argc, char *argv[]) {
	//auto t_start = std::chrono::high_resolution_clock::now();

	/* SDL Init */
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_Window* window = SDL_CreateWindow("GeoViewer", 760, 180, 1920, 1080, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	/* GLEW Init */
	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEPTH_TEST);

	/* Build Shaders */
	Shader* shaderOne = new Shader("assets/Shaders/simple.vert", "assets/Shaders/simple.frag");
	Shader* pyramidShader = new Shader("assets/Shaders/pyramid.vert", "assets/Shaders/pyramid.frag");

	/* Build Models */
	std::vector<Model*> modelList;
	for (int i = 0; i < 3; i++) {
		Model* data = new Model(shaderOne->GetShader());
		data->Transpose((i / 5.0f), (i / 5.0f), 0);
		modelList.push_back(data);

		data->PrintVerts();
	}

	/*GLint uniModel = glGetUniformLocation(shaderOne->ShaderProgram, "model");
	glm::mat4 view = glm::lookAt(
		glm::vec3(2.5f, 2.5f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);

	GLint uniView = glGetUniformLocation(shaderOne->ShaderProgram, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 1.0f, 10.0f);
	GLint uniProj = glGetUniformLocation(shaderOne->ShaderProgram, "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));*/

	SDL_Event windowEvent;
	int i = 0;
	while (true) {
		if (i == 1000) {
			i = 0;
		}

		if (SDL_PollEvent(&windowEvent)) {
			if (windowEvent.type == SDL_QUIT) {
				break;
			}
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*glm::mat4 model;
		model = glm::rotate(model, i * glm::radians(2.0f), glm::vec3(0.0f, 0.0f, 1.0f));		
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));*/

		shaderOne->Use();
		for each (Model* mod in modelList) {

			glBindVertexArray(mod->VAO);
			//glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			//glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		glBindVertexArray(0);

		SDL_GL_SwapWindow(window);
		i++;
	}

	/* Clean up */
	shaderOne->~Shader();
	pyramidShader->~Shader();
	for each (Model* mod in modelList) {
		mod->~Model();
	}
	SDL_GL_DeleteContext(context);
	SDL_Quit();


	return 0;
}