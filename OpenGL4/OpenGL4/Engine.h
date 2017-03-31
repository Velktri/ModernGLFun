#pragma once
#include "World.h"
#include "UI/Layout.h"
#include "Input.h"
#include "FrameBuffer.h"
#include "Managers/Manager.h"

class Engine {
public:
	Engine();

public:
	bool Init();
	void Run();
	void CleanUp();

private:
	World* MyWorld;
	Layout* UILayout;
	Input* MyInput;
	Manager* MyManager;
	FrameBuffer* SceneFrames;
	SDL_Window* MainWindow;
	SDL_GLContext context;

	GLuint WIDTH;
	GLuint HEIGHT;
	bool bIsRunning;
};

