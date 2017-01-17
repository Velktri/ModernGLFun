#include "Camera.h"

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

void Camera::processKeyEvents(SDL_Keycode key, GLfloat deltaTime) {
	if (key == SDLK_f) {
		Refocus();
	}

	if (key == SDLK_q) {

	}
}

void Camera::ProcessMouseEvents(SDL_Event event, GLfloat deltaTime) {
	if (event.button.button == SDL_BUTTON_LEFT && event.motion.state == 1) {
		OrbitCamera(event.motion.xrel, event.motion.yrel, deltaTime);
	} else if (event.button.button == SDL_BUTTON_X1) {
		PanCamera(event.motion.xrel, event.motion.yrel, deltaTime);
	}
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(WorldPosition, FocusPoint, LocalUp);
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

void Camera::ZoomCamera(SDL_Event event, GLfloat deltaTime) {
	GLfloat velocity = ZoomModifier * deltaTime;
	if (event.wheel.y > 0)
		WorldPosition -= LocalFront * velocity;
	else if (event.wheel.y < 0)
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

void Camera::updateCameraVectors() {
	LocalFront = glm::normalize(WorldPosition - FocusPoint);
	LocalRight = glm::normalize(glm::cross(LocalFront, WorldUp));
	LocalUp = glm::normalize(glm::cross(LocalRight, LocalFront));
}

void Camera::Refocus() {
	FocusPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	updateCameraVectors();
}