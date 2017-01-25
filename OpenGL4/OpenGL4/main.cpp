#include "World.h"
#include "Input.h"
#include "UI\Layout.h"
#include "FrameBuffer.h"
#include <imgui.h>
#include "imgui_impl_sdl_gl3.h"

/* Constants */
static const GLuint WIDTH = 1920;
static const GLuint HEIGHT = 1080;

int main(int argc, char *argv[]) {
	bool bIsRunning = true;
	/* SDL Init */
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Window* window = SDL_CreateWindow("GL_Render", 200, 180, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	/* GLEW Init */
	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_DEPTH_TEST);

	Layout* UILayout = new Layout(window, ImVec2(WIDTH, HEIGHT));
	World* MyWorld = new World(UILayout->GetSceneDimensions().x, UILayout->GetSceneDimensions().y);
	UILayout->SetWorld(MyWorld);
	Input* MyInput = new Input(MyWorld);
	FrameBuffer* SceneFrames = new FrameBuffer(UILayout->GetSceneDimensions().x, UILayout->GetSceneDimensions().y);


	MyWorld->StartClock();
	while (bIsRunning) {
		MyWorld->UpdateClock();
		MyWorld->GetLights()[0]->Translate(sin(MyWorld->GetTime()) / 20.0f, 0, cos(MyWorld->GetTime()) / 20.0f);

		MyInput->UpdateInput();
		bIsRunning = MyInput->ExecuteInput(UILayout->GetSceneHovering());

		SceneFrames->RenderWorldFrame(MyWorld);
		bIsRunning &= UILayout->RenderLayout(SceneFrames->GetFrameTexture());

		SDL_GL_SwapWindow(window);
	}

#pragma region Clean Up
	MyWorld->~World();
	SceneFrames->~FrameBuffer();
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
#pragma endregion 

	return 0;
}