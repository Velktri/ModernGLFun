#include "Engine.h"
#include "UI\Layout.h"
#include "System\World.h"
#include "System\Timer.h"
#include "System\Input.h"
#include "System\FrameBuffer.h"
#include "System\Manager.h"


bool Engine::Init()
{
	WIDTH = 1920;
	HEIGHT = 1080;

	/* SDL Init */
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return false;
	}

	/* Init SteamVR */
	bInitVR = vr::VR_IsHmdPresent() /* || other VR options*/;
	if (bInitVR)
	{
		VRError = vr::VRInitError_None;
		VR_HMD = vr::VR_Init(&VRError, vr::VRApplication_Scene);

		if (VRError != vr::VRInitError_None)
		{
			VR_HMD = NULL;
			char buf[1024];
			sprintf_s(buf, sizeof(buf), "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(VRError));
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "VR_Init Failed", buf, NULL);
			return false;
		}
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	MainWindow = SDL_CreateWindow("GeoEngine", 200, 180, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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
	GLenum GlewError = glewInit();
	if (GlewError != GLEW_OK)
	{
		printf("%s - Error initializing GLEW! %s\n", __FUNCTION__, glewGetErrorString(GlewError));
		return false;
	}
	glGetError(); // to clear the error caused deep in GLEW

	glEnable(GL_DEPTH_TEST);

	MyManager = new Manager();
	MyWorld = new World(MyManager);
	if (bInitVR) { MyWorld->InitVR(VR_HMD); }
	MyInput = new Input(MyWorld, MyManager);
	UILayout = new Layout(MainWindow, MyManager, MyWorld, MyInput);

	if (MyManager && MyWorld && MyInput && UILayout)
	{
		if (!MyManager) { printf("Error: Engine Failed to initialize Manager.\n"); }
		if (!MyWorld)   { printf("Error: Engine Failed to initialize World.\n"); }
		if (!MyInput)   { printf("Error: Engine Failed to initialize Input.\n"); }
		if (!UILayout)  { printf("Error: Engine Failed to initialize UILayout.\n"); }
	}

	return true;
}

void Engine::Run()
{
	MyWorld->GetTimer()->Start();
	while (true)
	{
		MyWorld->GetTimer()->Update();
		MyInput->UpdateInput();

		if (!MyInput->ExecuteInput(UILayout->GetHoveredRegion())) { break; }
		if (!UILayout->RenderLayout()) { break; }

		SDL_GL_SwapWindow(MainWindow);
	}
}

void Engine::CleanUp()
{
	if (MyWorld)   { MyWorld->~World(); }
	if (MyManager) { MyManager->~Manager(); }
	if (UILayout)  { UILayout->~Layout(); }
	if (MyInput)   { MyInput->~Input(); }

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(MainWindow);
	SDL_Quit();
}
