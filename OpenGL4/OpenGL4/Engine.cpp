#include "Engine.h"
#include "World.h"
#include "UI/Layout.h"
#include "Input.h"
#include "FrameBuffer.h"
#include "Manager.h"
#include "Lights\Light.h"

bool Engine::Init() {
	WIDTH = 1920;
	HEIGHT = 1080;
	bIsRunning = true;

	/* SDL Init */
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error: %s\n", SDL_GetError());
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	MainWindow = SDL_CreateWindow("GL_Render", 200, 180, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!MainWindow) {
		printf("Error: %s\n", SDL_GetError());
		return false;
	}

	context = SDL_GL_CreateContext(MainWindow);
	if (!context) {
		printf("Error: %s\n", SDL_GetError());
		return false;
	}

	/* GLEW Init */
	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_DEPTH_TEST);

	UILayout = new Layout(MainWindow, ImVec2(WIDTH, HEIGHT));
	MyManager = new Manager();
	MyWorld = new World(UILayout->GetSceneDimensions().x, UILayout->GetSceneDimensions().y);
	MyInput = new Input(MyWorld);

	UILayout->SetManager(MyManager);
	UILayout->SetWorld(MyWorld);

	MyWorld->SetManager(MyManager);
	MyInput->SetManger(MyManager);

	SceneFrames = new FrameBuffer(UILayout->GetSceneDimensions().x, UILayout->GetSceneDimensions().y);
	PickerFrames = new FrameBuffer(UILayout->GetSceneDimensions().x, UILayout->GetSceneDimensions().y);

	return true;
}

void Engine::Run() {
	MyWorld->StartClock();
	while (bIsRunning) {
		MyWorld->UpdateClock();
		MyWorld->GetLights()[0]->Translate(sin(MyWorld->GetTime()) / 20.0f, 0, cos(MyWorld->GetTime()) / 20.0f);

		MyInput->UpdateInput();
		bIsRunning = MyInput->ExecuteInput(UILayout->GetSceneHovering());

		if (MyInput->bColorPick) {
			glm::vec2 coords = MyInput->StartSelectionCoods;
			coords.y -= (HEIGHT - UILayout->GetSceneDimensions().y);
			MyManager->CheckForSelection(PickerFrames->RenderColorPick(MyWorld, coords));
			MyInput->bColorPick = false;
		}

		SceneFrames->RenderWorldFrame(MyWorld);
		bIsRunning &= UILayout->RenderLayout(SceneFrames->GetFrameTexture());

		SDL_GL_SwapWindow(MainWindow);
	}
}

void Engine::CleanUp() {
	MyWorld->~World();
	MyManager->~Manager();
	SceneFrames->~FrameBuffer();
	UILayout->~Layout();
	MyInput->~Input();
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(MainWindow);
	SDL_Quit();
}
