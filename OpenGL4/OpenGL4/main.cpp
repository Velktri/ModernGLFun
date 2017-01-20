#include "World.h"

/* Constants */
static const GLuint WIDTH = 1920;
static const GLuint HEIGHT = 1080;

int main(int argc, char *argv[]) {

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

	World* MyWorld = new World(WIDTH, HEIGHT);
	SDL_Event windowEvent;
	MyWorld->StartClock();
	while (true) {
		MyWorld->UpdateClock();
		MyWorld->GetLights()[0]->Translate(sin(MyWorld->GetTime()) / 20.0f, 0, cos(MyWorld->GetTime()) / 20.0f);

#pragma region Inputs
		if (SDL_PollEvent(&windowEvent)) {
			if (windowEvent.type == SDL_QUIT) {
				break;
			}

			if (windowEvent.type == SDL_MOUSEWHEEL) {
				MyWorld->GetCamera()->ZoomCamera(windowEvent, MyWorld->GetDeltaTime());
			}

			if (windowEvent.type == SDL_KEYDOWN) {
				MyWorld->GetCamera()->processKeyEvents(windowEvent.key.keysym.sym, MyWorld->GetDeltaTime());
			}

			if (windowEvent.type == SDL_MOUSEMOTION || windowEvent.type == SDL_MOUSEBUTTONDOWN || windowEvent.type == SDL_MOUSEBUTTONUP) {
				MyWorld->GetCamera()->ProcessMouseEvents(windowEvent, MyWorld->GetDeltaTime());
			}		
		}
#pragma endregion

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MyWorld->RenderWorld();
		SDL_GL_SwapWindow(window);
	}

#pragma region Clean Up
	/* Clean up */
	MyWorld->~World();

	SDL_GL_DeleteContext(context);
	SDL_Quit();
#pragma endregion 

	return 0;
}