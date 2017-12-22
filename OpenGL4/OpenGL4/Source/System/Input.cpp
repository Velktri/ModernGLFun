#include "Input.h"
#include <imgui.h>
#include "Universe.h"
#include "World.h"
#include "Camera.h"
#include "Timer.h"
#include "Manager.h"
#include "UI/Layout.h"
#include "UI/Region.h"
#include "Models/Gizmo.h"

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
	ActiveSceneCamera = MyUniverse->GetCamaras().Perspective; // @TODO: have every scene remember a camera and update with that in future.
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

	if (SceneHovering)
	{
		if (keyState[SDL_SCANCODE_LALT] || keyState[SDL_SCANCODE_RALT])
		{
			if (mouseState) { ProcessMouseEvents(); }
		}
		else
		{
			QuerySelection();
		}
	}



	//StartSelectionCoods = glm::vec2(xState, yState);

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
		if (bLeftIsPressed == false) //Left mouse has been pressed this frame
		{
			StartSelectionCoods = glm::vec2(xState - ActiveSceneRegion->GetScenePosition().x, yState - ActiveSceneRegion->GetScenePosition().y);
			CurrentSelectionCoods = StartSelectionCoods;
			MyUniverse->GetSelectionResults(StartSelectionCoods, glm::vec2(ActiveSceneRegion->GetSceneSize().x, ActiveSceneRegion->GetSceneSize().y));
			bLeftIsPressed = true;
		}
		else //Left mouse was pressed in a previous frame but hasn't been released.
		{
			if (MyManager->GetGizmo()->IsGizmoSelected())
			{
				CurrentSelectionCoods = glm::vec2(xState - ActiveSceneRegion->GetScenePosition().x, yState - ActiveSceneRegion->GetScenePosition().y);
				glm::vec2 SceneSize = glm::vec2(ActiveSceneRegion->GetSceneSize().x, ActiveSceneRegion->GetSceneSize().y);
				glm::vec3 CameraPosition;
				glm::vec3 RayCastDirection;
				glm::vec3 CameraLookDir = ActiveSceneCamera->GetFrontCameraDirection();
				MyUniverse->ConvertScreenToWorldSpace(CurrentSelectionCoods,
													  SceneSize,
													  glm::inverse(ActiveSceneCamera->GetViewProjection()),
													  CameraPosition,
													  RayCastDirection);

				MyManager->GetGizmo()->CalculateGizmoMovement(CameraPosition, RayCastDirection, CameraLookDir);
			}
		}
	}
	else if (bLeftIsPressed == true) // Left mouse was just released this frame
	{
		bLeftIsPressed = false;
		if (MyManager->GetGizmo()->IsGizmoSelected())
		{
			MyManager->GetGizmo()->ActiveAxis = ActiveHandle::None;
		}
		else
		{
			SelectAssets(StartSelectionCoods, CurrentSelectionCoods);
		}
	}
}

void Input::SelectAssets(glm::vec2 Start, glm::vec2 End)
{
	glm::vec2 SceneSize = glm::vec2(ActiveSceneRegion->GetSceneSize().x, ActiveSceneRegion->GetSceneSize().y);
	MyUniverse->CastRaytrace(MyUniverse->GetCamaras().Perspective, Start, SceneSize);
	MyUniverse->GetSelectionResults(Start, SceneSize);
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
