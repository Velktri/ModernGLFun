#include "Camera.h"
#include "VR_HMD.h"
#include "Models\Asset.h"
#include "FrameBuffer.h"
#include <iostream>
#include "Universe.h"

Camera::Camera(Universe* InUniverse, glm::vec3 position)
{
	OwningUniverse = InUniverse;
	WorldPosition = position;
	WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	FocusPoint = glm::vec3(0.0f, 0.0f, 0.0f);

	PanSpeed = 1.0f;
	ZoomModifier = 50.0f;
	OrbitSpeed = 25.0f;
	CameraNearDistance = 0.5f;

	NearClip = 0.01f;
	FarClip = 1000.0f;
	FieldOfView = 45.0f;

	UpdateCameraVectors();
}

Camera::~Camera()
{
	if (SceneFrame) { SceneFrame->~FrameBuffer(); }
	if (PickerFrame) { PickerFrame->~FrameBuffer(); }
}

void Camera::PanCamera(int Start_X, int Start_Y, GLfloat deltaTime)
{
	GLfloat velocity_X = PanSpeed * deltaTime * Start_X;
	GLfloat velocity_Y = PanSpeed * deltaTime * Start_Y;

	WorldPosition += RightVector * velocity_X;
	FocusPoint += RightVector * velocity_X;

	WorldPosition += UpVector * velocity_Y;
	FocusPoint += UpVector * velocity_Y;
	UpdateCameraVectors();
}

void Camera::ZoomCamera(int scroll, GLfloat deltaTime)
{
	GLfloat velocity = ZoomModifier * deltaTime;
	if (scroll > 0)
	{
		/* Zooming In */
		if (glm::length(FocusPoint - (WorldPosition - (ForwardVector * velocity))) > CameraNearDistance)
		{
			WorldPosition -= ForwardVector * velocity;
		}
	}
	else if (scroll < 0)
	{
		/* Zooming Out */
		WorldPosition += ForwardVector * velocity;
	}

	UpdateCameraVectors();
}

void Camera::OrbitCamera(int Start_X, int Start_Y, GLfloat deltaTime)
{
	float YawAngle = -1.0f * Start_X * deltaTime * OrbitSpeed;
	float PitchAngle = Start_Y * deltaTime * OrbitSpeed;

	glm::vec3 camFocusVector = WorldPosition - FocusPoint;
	glm::mat4 YawRotation = glm::rotate(glm::mat4(1.0f), glm::radians(YawAngle), UpVector);
	glm::mat4 PitchRotation = glm::rotate(glm::mat4(1.0f), glm::radians(PitchAngle), RightVector);

	WorldPosition = glm::vec3(PitchRotation * YawRotation * glm::vec4(camFocusVector, 1.0)) + FocusPoint;
	UpdateCameraVectors();
}

void Camera::UpdatePerspective(glm::vec2 FrameSize)
{
	Projection = glm::perspective(glm::radians(FieldOfView), (GLfloat) FrameSize.x / (GLfloat) FrameSize.y, NearClip, FarClip);
}

void Camera::UpdateFieldOfView(float InFOV)
{
	if (InFOV > 0 && InFOV < 180)
	{
		FieldOfView = InFOV;
	}
}

void Camera::UpdateCameraVectors()
{
	ForwardVector = glm::normalize(WorldPosition - FocusPoint);
	RightVector = glm::normalize(glm::cross(ForwardVector, WorldUp));
	UpVector = glm::normalize(glm::cross(RightVector, ForwardVector));
	ViewMatrix = glm::lookAt(WorldPosition, FocusPoint, UpVector);
}

void Camera::Refocus(Asset* InSelection)
{
	if (InSelection == NULL)
	{
		FocusPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	else
	{
		FocusPoint = InSelection->GetOrigin();
	}

	UpdateCameraVectors();
}


GLuint Camera::RenderCameraFrame(glm::vec2 InFrameSize, EFrameTypes FrameType)
{
	UpdatePerspective(InFrameSize);

	switch (FrameType)
	{
		case EFrameTypes::ColorPicker:
		{
			if (!PickerFrame)
			{
				PickerFrame = new FrameBuffer(InFrameSize.x, InFrameSize.y);
			}
			PickerFrame->RenderColorPick(this, OwningUniverse->ActiveWorld, InFrameSize);
			return PickerFrame->GetFrameTexture();
		}
		case EFrameTypes::Scene:
		{
			if (!SceneFrame)
			{
				SceneFrame = new FrameBuffer(InFrameSize.x, InFrameSize.y);
			}
			SceneFrame->RenderWorldFrame(this, OwningUniverse->ActiveWorld, InFrameSize);
			return SceneFrame->GetFrameTexture();
		}
		case EFrameTypes::VR_LeftEye:
		{
			return OwningUniverse->GetCamaras().VRCamera->GetLeftFrame().ResolveTextureId;
		}
		case EFrameTypes::VR_RightEye:
		{	
			return OwningUniverse->GetCamaras().VRCamera->GetRightFrame().ResolveTextureId;
		}
		default:
		{
			return NULL;
		}
	}
}

glm::vec3 Camera::GetPosition() { return WorldPosition; }
glm::vec3 Camera::GetFrontCameraDirection() { return ForwardVector; }
glm::vec3 Camera::GetUpCameraDirection() { return UpVector; }
glm::vec3 Camera::GetRightCameraDirection() { return RightVector; }
float Camera::GetFieldOfView() { return FieldOfView; }
glm::mat4 Camera::GetProjection() { return Projection; }
glm::mat4 Camera::GetViewMatrix() { return ViewMatrix; }
glm::mat4 Camera::GetViewProjection() { return Projection * ViewMatrix; }
Universe* Camera::GetOwningUniverse() { return OwningUniverse; }
FrameBuffer* Camera::GetPickerFrame() { return PickerFrame; }