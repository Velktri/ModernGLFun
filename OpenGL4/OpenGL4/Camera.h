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
	void ProcessMouseEvents(SDL_Event event, GLfloat deltaTime);

	glm::mat4 GetViewMatrix();
	void PanCamera(int Start_X, int Start_Y, GLfloat deltaTime);
	void ZoomCamera(SDL_Event event, GLfloat deltaTime);
	void OrbitCamera(int Start_X, int Start_Y, GLfloat deltaTime);

	glm::vec3 GetPosition();
	void SetProjection(glm::mat4 InProjection);
	glm::mat4 GetProjection();

private:
	/* Camera World Attributes */
	glm::vec3 WorldPosition;
	glm::vec3 WorldUp;
	glm::vec3 FocusPoint;

	/* Camera Local Attributes */
	glm::vec3 LocalFront;
	glm::vec3 LocalUp;
	glm::vec3 LocalRight;

	/* Camera options */
	GLfloat PanSpeed;
	GLfloat ZoomModifier;
	GLfloat OrbitSpeed;

	/* Eular Angles */
	GLfloat Yaw;
	GLfloat Pitch;

	void updateCameraVectors();
	void Refocus();

	glm::mat4 Projection;
};
