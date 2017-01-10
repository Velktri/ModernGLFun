#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_main.h>

/* GLM */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

class Camera {
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
	~Camera();

	void processKeyEvents(SDL_Keycode key, GLfloat deltaTime);
	void ProcessMouseEvents(SDL_MouseButtonEvent Button, GLboolean constrainPitch = true);

	glm::mat4 GetViewMatrix();
	void PanCamera();
	void ZoomCamera(SDL_Event event, GLfloat deltaTime);
	void OrbitCamera();


private:
	/* Camera World Attributes */
	glm::vec3 WorldPosition;
	glm::vec3 WorldUp;

	/* Camera Local Attributes */
	glm::vec3 LocalFront;
	glm::vec3 LocalUp;
	glm::vec3 LocalRight;

	/* Camera options */
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;

	/* Eular Angles */
	GLfloat Yaw;
	GLfloat Pitch;

	GLfloat lastX;
	GLfloat lastY;
	bool firstMouse;

	void updateCameraVectors();

};
