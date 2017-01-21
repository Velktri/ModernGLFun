#include "Input.h"



Input::Input(World* InWorld) {
	world = InWorld;
}


Input::~Input() {
}

void Input::UpdateInput() {
	keyState = SDL_GetKeyboardState(NULL);
	mouseState = SDL_GetMouseState(&xState, &yState);
	relativeMouseState = SDL_GetRelativeMouseState(&xRelState, &yRelState);
}

bool Input::ExecuteInput() {
	bool bQuit = true;
	while (SDL_PollEvent(&windowEvent)) {
		if (windowEvent.type == SDL_MOUSEWHEEL) {
			world->GetCamera()->ZoomCamera(windowEvent.wheel.y, world->GetDeltaTime());
		}
	}

	SDL_PumpEvents();
	if (keyState[SDL_SCANCODE_LALT] || keyState[SDL_SCANCODE_RALT]) {
		if (mouseState) {
			ProcessMouseEvents();
		}
	}

	bQuit = ProcessKeyEvents();

	return bQuit;
}


void Input::ProcessMouseEvents() {
	if (mouseState & SDL_BUTTON_LMASK) {
		world->GetCamera()->OrbitCamera(xRelState, yRelState, world->GetDeltaTime());
	} else if (mouseState & SDL_BUTTON_RMASK) {
		world->GetCamera()->PanCamera(xRelState, yRelState, world->GetDeltaTime());
	}
}

bool Input::ProcessKeyEvents() {
	if (keyState[SDL_SCANCODE_F]) {
		world->GetCamera()->Refocus();
	}

	if (keyState[SDL_SCANCODE_Q]) {
		return false;
	}
	return true;
}
