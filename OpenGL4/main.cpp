#include <chrono>
#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "Grid.h"
#include "Pyramid.h"

/* Constants */
static const GLuint WIDTH = 1920;
static const GLuint HEIGHT = 1080;
static const int GRIDRADIUS_X = 10;
static const int GRIDRADIUS_Y = 10;
static const float GRIDSPACING = 1.0;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLfloat lastX = WIDTH / 2;
GLfloat lastY = HEIGHT / 2;


int main(int argc, char *argv[]) {
	auto t_start = std::chrono::high_resolution_clock::now();

	/* SDL Init */
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_Window* window = SDL_CreateWindow("GeoViewer", 760, 180, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	/* GLEW Init */
	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEPTH_TEST);

	/*Build Camera */
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.01f, 1000.0f);
	Camera* worldCamera = new Camera(glm::vec3(0.0f, 5.0f, 20.0f));

	/* Build Shaders */
	Shader* shaderOne = new Shader("assets/Shaders/simple.vert", "assets/Shaders/simple.frag");
	Shader* pyramidShader = new Shader("assets/Shaders/pyramid.vert", "assets/Shaders/pyramid.frag");
	Shader* TransformShader = new Shader("assets/Shaders/transform2.vert", "assets/Shaders/transform2.frag");
	Shader* SceneShader = new Shader("assets/Shaders/transform.vert", "assets/Shaders/transform.frag");

	/* Build scene elements */
	Grid* Scene = new Grid(GRIDRADIUS_X, GRIDRADIUS_Y, GRIDSPACING);

	/* Build Models */
	std::vector<Model*> modelList;
	for (int i = 0; i < 5; i++) {
		Model* data = new Model();
		data->GenerateModel();
		data->TranslateModel(i, i, i);
		modelList.push_back(data);
	}

	/* Main Loop */
	SDL_Event windowEvent;
	while (true) {
		auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		deltaTime = time - lastFrame;
		lastFrame = time;


		if (SDL_PollEvent(&windowEvent)) {
			if (windowEvent.type == SDL_QUIT) {
				break;
			}

			if (windowEvent.type == SDL_MOUSEWHEEL) {
				worldCamera->ZoomCamera(windowEvent, deltaTime);
			}

			if (windowEvent.type == SDL_KEYDOWN) {
				worldCamera->processKeyEvents(windowEvent.key.keysym.sym, deltaTime);
			}

			if (windowEvent.type == SDL_MOUSEMOTION || windowEvent.type == SDL_MOUSEBUTTONDOWN || windowEvent.type == SDL_MOUSEBUTTONUP) {
				worldCamera->ProcessMouseEvents(windowEvent.button);
			}		
		}


		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Render Scene */
		SceneShader->Use();

		// Create transformations and get their uniform location
		GLint modelLoc = glGetUniformLocation(SceneShader->GetShader(), "model");
		GLint viewLoc = glGetUniformLocation(SceneShader->GetShader(), "view");
		GLint projLoc = glGetUniformLocation(SceneShader->GetShader(), "projection");

		// Pass them to the shaders
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Scene->orientation));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(worldCamera->GetViewMatrix()));
		// Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glBindVertexArray(Scene->VAO);
		glDrawArrays(GL_LINES, 0, 4 * (GRIDRADIUS_X + GRIDRADIUS_Y + 1));
		glBindVertexArray(0);



		/* Render Models */
		TransformShader->Use();

		// Get their uniform location
		modelLoc = glGetUniformLocation(TransformShader->GetShader(), "model");
		viewLoc = glGetUniformLocation(TransformShader->GetShader(), "view");
		projLoc = glGetUniformLocation(TransformShader->GetShader(), "projection");

		// Pass them to the shaders
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(worldCamera->GetViewMatrix()));
		// Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		for each (Model* mod in modelList) {
			glBindVertexArray(mod->VAO);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mod->orientation));
			glDrawElements(GL_TRIANGLES, mod->indicesSize, GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);

		SDL_GL_SwapWindow(window);
	}

	/* Clean up */
	shaderOne->~Shader();
	pyramidShader->~Shader();
	TransformShader->~Shader();
	for each (Model* mod in modelList) {
		mod->~Model();
	}

	Scene->~Grid();
	

	SDL_GL_DeleteContext(context);
	SDL_Quit();


	return 0;
}