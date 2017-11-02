#include "Input.h"
#include <imgui.h>
#include "World.h"
#include "Camera.h"
#include "Timer.h"
#include "Manager.h"
#include "UI/Layout.h"

Input::Input(World* InWorld) {
	world = InWorld;
	bLeftIsPressed = false;
	bColorPick = false;
}

Input::~Input() {
}

void Input::UpdateInput() {
	keyState = SDL_GetKeyboardState(NULL);
	mouseState = SDL_GetMouseState(&xState, &yState);
	relativeMouseState = SDL_GetRelativeMouseState(&xRelState, &yRelState);
}

bool Input::ExecuteInput(Region* ActiveRegion) {
	bool SceneHovering = false;

	if (ActiveRegion && ActiveRegion->GetType() == RegionTypes::Scene) // @TODO: rewrite for multiple scenes
	{
		SceneHovering = true;
	}

	while (SDL_PollEvent(&windowEvent)) {
		if (windowEvent.type == SDL_MOUSEWHEEL && SceneHovering) {
			world->GetCamera()->ZoomCamera(windowEvent.wheel.y, world->GetTimer()->GetDeltaTime());
		}

		if (windowEvent.type == SDL_QUIT) {
			return false;
		}
	}

	SDL_PumpEvents();
	if (keyState[SDL_SCANCODE_LALT] || keyState[SDL_SCANCODE_RALT]) {
		if (mouseState && SceneHovering) { ProcessMouseEvents(); }
	} else {
		if (SceneHovering) { QuerySelection(); }
	}

	StartSelectionCoods = glm::vec2(xState, yState);

	ProcessKeyEvents();
	return true;
}

void Input::ProcessMouseEvents() {
	if (mouseState & SDL_BUTTON_LMASK) {
		world->GetCamera()->OrbitCamera(xRelState, yRelState, world->GetTimer()->GetDeltaTime());
	} else if (mouseState & SDL_BUTTON_RMASK) {
		world->GetCamera()->PanCamera(xRelState, yRelState, world->GetTimer()->GetDeltaTime());
	}
}

void Input::ProcessKeyEvents() {
	if (keyState[SDL_SCANCODE_F]) {
		world->GetCamera()->Refocus(MyManager->GetSelectedAsset());
	}
}

void Input::QuerySelection() {
	if (mouseState & SDL_BUTTON_LMASK) {
		if (bLeftIsPressed == false) {
			StartSelectionCoods = glm::vec2(xState, yState);
			bLeftIsPressed = true;
		}
		EndSelectionCoods = glm::vec2(xState, yState);
	} else if (bLeftIsPressed == true) {
		bLeftIsPressed = false;
		SelectAssets(StartSelectionCoods, EndSelectionCoods);
	}
}

void Input::SelectAssets(glm::vec2 Start, glm::vec2 End) {
	world->CastRaytrace(Start);
	bColorPick = true;
}

void Input::SetManger(Manager* m) {
	MyManager = m;
}
