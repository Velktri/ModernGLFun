#pragma once
#include <GL/glew.h>
#include <SDL.h>

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

	class World* MyWorld;
	class Layout* UILayout;
	class Input* MyInput;
	class Manager* MyManager;
	//class FrameBuffer* SceneFrames;
	class FrameBuffer* PickerFrames;

	SDL_Window* MainWindow;
	SDL_GLContext context;

	GLuint WIDTH;
	GLuint HEIGHT;
	bool bIsRunning;
};