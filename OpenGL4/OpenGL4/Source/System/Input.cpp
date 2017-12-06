#include "Input.h"
#include <imgui.h>
#include "Universe.h"
#include "World.h"
#include "Camera.h"
#include "Timer.h"
#include "Manager.h"
#include "UI/Layout.h"
#include "UI/Region.h"

Input::Input(Universe* InUniverse, Manager* InManager)
{
	MyUniverse = InUniverse;
	MyManager = InManager;
	bLeftIsPressed = false;
	bSelectionRequest = false;
}

Input::~Input()
{
}

void Input::UpdateInput()
{
	keyState = SDL_GetKeyboardState(NULL);
	mouseState = SDL_GetMouseState(&xState, &yState);
	relativeMouseState = SDL_GetRelativeMouseState(&xRelState, &yRelState);
}

bool Input::ExecuteInput(Region* InActiveRegion)
{
	bool SceneHovering = false;
	if (InActiveRegion) // @TODO: rewrite for multiple scenes
	{
		Container* ActiveContainer = dynamic_cast<Container*>(InActiveRegion);
		if (ActiveContainer && ActiveContainer->GetType() == RegionTypes::Scene)
		{
			SceneHovering = InActiveRegion->IsSceneHovered();
			ActiveSceneRegion = InActiveRegion;
		}
	}

	while (SDL_PollEvent(&windowEvent))
	{
		if (windowEvent.type == SDL_MOUSEWHEEL && SceneHovering)
		{
			ActiveSceneRegion->GetRegionCamera()->ZoomCamera(windowEvent.wheel.y, MyUniverse->GetUniversalTimer()->GetDeltaTime());
		}

		if (windowEvent.type == SDL_QUIT)
		{
			return false;
		}
	}

	SDL_PumpEvents();
	if (keyState[SDL_SCANCODE_LALT] || keyState[SDL_SCANCODE_RALT])
	{
		if (mouseState && SceneHovering) { ProcessMouseEvents(); }
	}
	else
	{
		if (SceneHovering) { QuerySelection(); }
	}


	StartSelectionCoods = glm::vec2(xState, yState);

	ProcessKeyEvents();
	return true;
}

void Input::ProcessMouseEvents()
{
	if (mouseState & SDL_BUTTON_LMASK)
	{
		ActiveSceneRegion->GetRegionCamera()->OrbitCamera(xRelState, yRelState, MyUniverse->GetUniversalTimer()->GetDeltaTime());
	}
	else if (mouseState & SDL_BUTTON_RMASK)
	{
		ActiveSceneRegion->GetRegionCamera()->PanCamera(xRelState, yRelState, MyUniverse->GetUniversalTimer()->GetDeltaTime());
	}
}

void Input::ProcessKeyEvents()
{
	if (keyState[SDL_SCANCODE_F])
	{
		ActiveSceneRegion->GetRegionCamera()->Refocus(MyManager->GetSelectedAsset());
	}
}

void Input::QuerySelection()
{
	if (mouseState & SDL_BUTTON_LMASK)
	{
		if (bLeftIsPressed == false)
		{
			StartSelectionCoods = glm::vec2(xState, yState);
			bLeftIsPressed = true;
		}
		EndSelectionCoods = glm::vec2(xState, yState);
	}
	else if (bLeftIsPressed == true)
	{
		bLeftIsPressed = false;
		SelectAssets(StartSelectionCoods, EndSelectionCoods);
	}
}

void Input::SelectAssets(glm::vec2 Start, glm::vec2 End)
{
	MyUniverse->ActiveWorld->CastRaytrace(MyUniverse->GetCamaras().Perspective, glm::vec2(Start.x - ActiveSceneRegion->GetScenePosition().x, Start.y - ActiveSceneRegion->GetScenePosition().y), 
						glm::vec2(ActiveSceneRegion->GetSceneSize().x, ActiveSceneRegion->GetSceneSize().y));
	bSelectionRequest = true;
}

bool Input::PollSelectionRequest(int* InActiveRegion) 
{
	if (bSelectionRequest)
	{
		bSelectionRequest = false;
		*InActiveRegion = ActiveSceneRegion->GetRegionID();
		return true;
	}
	return false;
}
