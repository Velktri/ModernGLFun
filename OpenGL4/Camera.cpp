#include "Camera.h"

Camera::Camera(glm::vec3 position) {
	WorldPosition = position;
	WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	Yaw = -90.0f;
	Pitch = 0.0f;
	MovementSpeed = 10;
	MouseSensitivity = 0.002f;

	lastX = 1920 / 2;
	lastY = 1080 / 2;
	firstMouse = true;

	updateCameraVectors();
}

Camera::~Camera() {
}

void Camera::processKeyEvents(SDL_Keycode key, GLfloat deltaTime) {

	GLfloat velocity = MovementSpeed * deltaTime;
	if (key == SDLK_a)
		WorldPosition -= LocalRight * velocity;
	if (key == SDLK_d)
		WorldPosition += LocalRight * velocity;
}

void Camera::ProcessMouseEvents(SDL_MouseButtonEvent Button, GLboolean constrainPitch) {
	if (Button.button == SDL_BUTTON_LEFT) {
		int xpos, ypos;
		SDL_GetMouseState(&xpos, &ypos);


		if (firstMouse) {
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		GLfloat xoffset = xpos - lastX;
		GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

		lastX = xpos;
		lastY = ypos;




		xpos *= MouseSensitivity;
		ypos *= MouseSensitivity;

		Yaw += xpos;
		Pitch += ypos;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch) {
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		this->updateCameraVectors();
	}
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(WorldPosition, WorldPosition + LocalFront, LocalUp);
}

void Camera::PanCamera() {

}

void Camera::ZoomCamera(SDL_Event event, GLfloat deltaTime) {
	GLfloat velocity = MovementSpeed * deltaTime;
	if (event.wheel.y > 0)
		WorldPosition += LocalFront * velocity;
	else if (event.wheel.y < 0)
		WorldPosition -= LocalFront * velocity;
}

void Camera::OrbitCamera() {

}

void Camera::updateCameraVectors() {
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

	LocalFront = glm::normalize(front);
	LocalRight = glm::normalize(glm::cross(LocalFront, WorldUp));
	LocalUp = glm::normalize(glm::cross(LocalRight, LocalFront));
}