#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include "World.h"

class Input {
public:
	Input(World* InWorld);
	~Input();
	void UpdateInput();
	bool ExecuteInput();

	void ProcessMouseEvents();
	bool ProcessKeyEvents();
private:
	SDL_Event windowEvent;
	const Uint8* keyState;
	Uint32 mouseState;
	Uint32 relativeMouseState;
	World* world;

	int xState;
	int yState;

	int xRelState;
	int yRelState;
};

