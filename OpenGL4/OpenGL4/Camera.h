#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_main.h>

/* GLM */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "Models\Asset.h"

class Asset;

class Camera {
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
	~Camera();

	glm::mat4 GetViewMatrix();
	void PanCamera(int Start_X, int Start_Y, GLfloat deltaTime);
	void ZoomCamera(int scroll, GLfloat deltaTime);
	void OrbitCamera(int Start_X, int Start_Y, GLfloat deltaTime);
	void Refocus(Asset* InAsset);

	glm::vec3 GetPosition();
	glm::vec3 GetFrontCameraDirection();
	glm::vec3 GetUpCameraDirection();
	glm::vec3 GetRightCameraDirection();

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

	void updateCameraVectors();


	glm::mat4 Projection;
};
