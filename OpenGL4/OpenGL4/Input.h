#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class World;
class Manager;

class Input {
public:
	Input(World* InWorld);
	~Input();
	void UpdateInput();
	bool ExecuteInput(bool SceneHovering);
	void SetManger(Manager* m);

private:
	SDL_Event windowEvent;
	const Uint8* keyState;
	Uint32 mouseState;
	Uint32 relativeMouseState;
	World* world;
	Manager* MyManager;

	int xState;
	int yState;

	int xRelState;
	int yRelState;

	bool bLeftIsPressed;
	glm::vec2 StartSelectionCoods;
	glm::vec2 EndSelectionCoods;

	void ProcessMouseEvents();
	void ProcessKeyEvents();
	void QuerySelection();
	void SelectAssets(glm::vec2 Start, glm::vec2 End);
	void DrawLine();
};

