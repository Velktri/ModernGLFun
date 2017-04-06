#pragma once
#include <GL/glew.h>
#include <SDL.h>

class Engine {
public:
	Engine();

public:
	bool Init();
	void Run();
	void CleanUp();

private:
	class World* MyWorld;
	class Layout* UILayout;
	class Input* MyInput;
	class Manager* MyManager;
	class FrameBuffer* SceneFrames;
	SDL_Window* MainWindow;
	SDL_GLContext context;

	GLuint WIDTH;
	GLuint HEIGHT;
	bool bIsRunning;
};

