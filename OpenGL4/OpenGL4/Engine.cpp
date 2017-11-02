#include "Engine.h"
#include "UI\Layout.h"
#include "System\World.h"
#include "System\Timer.h"
#include "System\Input.h"
#include "System\FrameBuffer.h"
#include "System\Manager.h"
#include "Lights\Light.h"

//@TODO: Add an config file system for easy tweaks
bool Engine::Init()
{
	WIDTH = 1920;
	HEIGHT = 1080;
	bIsRunning = true;

	/* SDL Init */
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
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
	if (!MainWindow)
	{
		printf("Error: %s\n", SDL_GetError());
		return false;
	}

	context = SDL_GL_CreateContext(MainWindow);
	if (!context)
	{
		printf("Error: %s\n", SDL_GetError());
		return false;
	}

	/* GLEW Init */
	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_DEPTH_TEST);

	UILayout = new Layout(MainWindow);
	MyManager = new Manager();
	MyWorld = new World(WIDTH, HEIGHT/*  @TODO: Create world rendering based on the scene Size. */);
	MyInput = new Input(MyWorld);

	UILayout->SetManager(MyManager);
	UILayout->SetWorld(MyWorld);

	MyWorld->SetManager(MyManager);
	MyInput->SetManger(MyManager);

	// TODO: change picker frame to be with a scene frame; add ability to swap them in the region.
	PickerFrames = new FrameBuffer(WIDTH, HEIGHT);

	return true;
}

void Engine::Run()
{
	MyWorld->GetTimer()->Start();
	while (bIsRunning)
	{
		MyWorld->GetTimer()->Update();
		MyInput->UpdateInput();
		bIsRunning = MyInput->ExecuteInput(UILayout->GetHoveredRegion());

		//if (MyInput->bColorPick)
		//{
		//	glm::vec2 coords = MyInput->StartSelectionCoods;
		//	coords.y -= (HEIGHT - UILayout->GetSceneDimensions().y);
		//	MyManager->CheckForSelection(PickerFrames->RenderColorPick(MyWorld, coords));
		//	MyInput->bColorPick = false;
		//}

		bIsRunning &= UILayout->RenderLayout();

		SDL_GL_SwapWindow(MainWindow);
	}
}

void Engine::CleanUp()
{
	MyWorld->~World();
	MyManager->~Manager();
	UILayout->~Layout();
	MyInput->~Input();
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(MainWindow);
	SDL_Quit();
}
