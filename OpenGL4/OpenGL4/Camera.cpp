#include "Camera.h"
#include "Models\Asset.h"
#include <iostream>

Camera::Camera(glm::vec3 position) {
	WorldPosition = position;
	WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	FocusPoint = glm::vec3(0.0f, 0.0f, 0.0f);

	PanSpeed = 1.0f;
	ZoomModifier = 50.0f;
	OrbitSpeed = 25.0f;

	updateCameraVectors();
}

Camera::~Camera() {
}

void Camera::PanCamera(int Start_X, int Start_Y, GLfloat deltaTime) {
	GLfloat velocity_X = PanSpeed * deltaTime * Start_X;
	GLfloat velocity_Y = PanSpeed * deltaTime * Start_Y;

	WorldPosition += LocalRight * velocity_X;
	FocusPoint += LocalRight * velocity_X;

	WorldPosition += LocalUp * velocity_Y;
	FocusPoint += LocalUp * velocity_Y;
	updateCameraVectors();
}

void Camera::ZoomCamera(int scroll, GLfloat deltaTime) {
	GLfloat velocity = ZoomModifier * deltaTime;
	if (scroll > 0)
		WorldPosition -= LocalFront * velocity;
	else if (scroll < 0)
		WorldPosition += LocalFront * velocity;
	updateCameraVectors();
}

void Camera::OrbitCamera(int Start_X, int Start_Y, GLfloat deltaTime) {
	float YawAngle = -1.0f * Start_X * deltaTime * OrbitSpeed;
	float PitchAngle = Start_Y * deltaTime * OrbitSpeed;

	glm::vec3 camFocusVector = WorldPosition - FocusPoint;
	glm::mat4 YawRotation = glm::rotate(glm::mat4(1.0f), glm::radians(YawAngle), LocalUp);
	glm::mat4 PitchRotation = glm::rotate(glm::mat4(1.0f), glm::radians(PitchAngle), LocalRight);

	WorldPosition = glm::vec3(PitchRotation * YawRotation * glm::vec4(camFocusVector, 1.0)) + FocusPoint;
	updateCameraVectors();
}

glm::vec3 Camera::GetPosition() {
	return WorldPosition;
}

glm::vec3 Camera::GetFrontCameraDirection() {
	return LocalFront;
}

glm::vec3 Camera::GetUpCameraDirection() {
	return LocalUp;
}

glm::vec3 Camera::GetRightCameraDirection() {
	return LocalRight;
}

void Camera::SetProjection(glm::mat4 InProjection) {
	Projection = InProjection;
}

glm::mat4 Camera::GetProjection() {
	return Projection;
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(WorldPosition, FocusPoint, LocalUp);
}


void Camera::updateCameraVectors() {
	LocalFront = glm::normalize(WorldPosition - FocusPoint);
	LocalRight = glm::normalize(glm::cross(LocalFront, WorldUp));
	LocalUp = glm::normalize(glm::cross(LocalRight, LocalFront));
}

void Camera::Refocus(Asset* InAsset) {
	if (InAsset == NULL) {
		FocusPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	} else {
		FocusPoint = InAsset->GetOrigin();
	}

	updateCameraVectors();
}