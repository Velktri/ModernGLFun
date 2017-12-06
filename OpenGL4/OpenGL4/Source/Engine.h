#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include "openvr.h"

class Engine
{
public:
	static Engine& getInstance()
	{
		static Engine instance;
		return instance;
	}

	Engine(Engine const&) = delete;
	void operator=(Engine const&) = delete;

	bool Init();
	void Run();
	void CleanUp();

private:
	Engine() {}

	class Universe* MyUniverse;
	class Layout* UILayout;
	class Input* MyInput;
	class Manager* MyManager;

	SDL_Window* MainWindow;
	SDL_GLContext context;

	GLuint WIDTH;
	GLuint HEIGHT;
};