#include "Input.h"



Input::Input(World* InWorld) {
	world = InWorld;
	bLeftIsPressed = false;
}


Input::~Input() {
}

void Input::UpdateInput() {
	keyState = SDL_GetKeyboardState(NULL);
	mouseState = SDL_GetMouseState(&xState, &yState);
	relativeMouseState = SDL_GetRelativeMouseState(&xRelState, &yRelState);
}

bool Input::ExecuteInput(bool SceneHovering) {
	while (SDL_PollEvent(&windowEvent)) {
		if (windowEvent.type == SDL_MOUSEWHEEL && SceneHovering) {
			world->GetCamera()->ZoomCamera(windowEvent.wheel.y, world->GetDeltaTime());
		}

		if (windowEvent.type == SDL_QUIT) {
			return false;
		}
	}

	SDL_PumpEvents();
	if (keyState[SDL_SCANCODE_LALT] || keyState[SDL_SCANCODE_RALT]) {
		if (mouseState && SceneHovering) {
			ProcessMouseEvents();
		}
	}/* else {
		QuerySelection();
	}*/

	ProcessKeyEvents();
	return true;
}


void Input::ProcessMouseEvents() {
	if (mouseState & SDL_BUTTON_LMASK) {
		world->GetCamera()->OrbitCamera(xRelState, yRelState, world->GetDeltaTime());
	} else if (mouseState & SDL_BUTTON_RMASK) {
		world->GetCamera()->PanCamera(xRelState, yRelState, world->GetDeltaTime());
	}
}

void Input::ProcessKeyEvents() {
	if (keyState[SDL_SCANCODE_F]) {
		world->GetCamera()->Refocus(world->GetSelectedAsset());
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
	glm::vec2 ScreenSize = glm::vec2(1536, 864);
	Start.x -= 50.0f;
	Start.y -= 175.0f;
	End.x -= 50.0f;
	End.y -= 175.0f;
	printf("Select between %f, %f and %f, %f.\n", Start.x, Start.y, End.x, End.y);

	float x = (2.0f * End.x) / ScreenSize.x - 1.0f;
	float y = 1.0f - (2.0f * End.y) / ScreenSize.y;
	float z = 1.0f;
	glm::vec3 ray_nds = glm::vec3(x, y, z);
	glm::vec4 ray_clip = glm::vec4(x, y, -1.0, 1.0);

	glm::vec4 ray_eye = glm::inverse(world->GetCamera()->GetProjection()) * ray_clip;
	ray_eye.z = -1.0f;
	ray_eye.w = 0.0f;
	glm::vec4 a = glm::vec4(glm::inverse(world->GetCamera()->GetViewMatrix()) * ray_eye);
	glm::vec3 ray_wor = glm::vec3(a.x , a.y, a.z);
	// don't forget to normalise the vector at some point
	ray_wor = glm::normalize(ray_wor);

	printf("Norm Dev Coords: %f, %f\n", x, y);
}


void Input::DrawLine() {
	glm::vec3 camDir = world->GetCamera()->GetFrontCameraDirection();
	glm::vec3 h = world->GetCamera()->GetRightCameraDirection();
	glm::vec3 v = world->GetCamera()->GetUpCameraDirection();

}
