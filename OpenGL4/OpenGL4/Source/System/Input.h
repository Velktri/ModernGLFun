#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Universe;
class Manager;
class Region;
class Camera;

class Input {
public:
	Input(Universe* InUniverse, Manager* InManager);
	~Input();
	void UpdateInput();
	bool ExecuteInput(Region* InActiveRegion);
	glm::vec2 StartSelectionCoods;
	bool PollSelectionRequest(int* InActiveRegion);

private:
	SDL_Event windowEvent;
	const Uint8* keyState;
	Uint32 mouseState;
	Uint32 relativeMouseState;
	Universe* MyUniverse;
	Manager* MyManager;
	Region* ActiveSceneRegion;
	Camera* ActiveSceneCamera;

	int xState;
	int yState;

	int xRelState;
	int yRelState;

	bool bLeftIsPressed;
	bool bSelectionRequest;
	glm::vec2 CurrentSelectionCoods;

	void ProcessMouseEvents();
	void ProcessKeyEvents();
	void QuerySelection();
	void SelectAssets(glm::vec2 Start, glm::vec2 End);
};

