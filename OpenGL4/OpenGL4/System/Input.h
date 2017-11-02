#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class World;
class Manager;
class Region;

class Input {
public:
	Input(World* InWorld, Manager* InManager);
	~Input();
	void UpdateInput();
	bool ExecuteInput(Region* InActiveRegion);
	glm::vec2 StartSelectionCoods;
	bool PollSelectionRequest();

private:
	SDL_Event windowEvent;
	const Uint8* keyState;
	Uint32 mouseState;
	Uint32 relativeMouseState;
	World* world;
	Manager* MyManager;
	Region* ActiveSceneRegion;

	int xState;
	int yState;

	int xRelState;
	int yRelState;

	bool bLeftIsPressed;
	bool bSelectionRequest;
	glm::vec2 EndSelectionCoods;

	void ProcessMouseEvents();
	void ProcessKeyEvents();
	void QuerySelection();
	void SelectAssets(glm::vec2 Start, glm::vec2 End);
};

