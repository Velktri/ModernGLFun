#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_main.h>

/* GLM */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "HelperTypes.h"

class Asset;
class FrameBuffer;
class Universe;

class Camera {
public:
	Camera(Universe* InUniverse, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
	~Camera();

	void PanCamera(int Start_X, int Start_Y, GLfloat deltaTime);
	void ZoomCamera(int scroll, GLfloat deltaTime);
	void OrbitCamera(int Start_X, int Start_Y, GLfloat deltaTime);
	void Refocus(Asset* InSelection);
	GLuint RenderCameraFrame(glm::vec2 InFrameSize, EFrameTypes FrameType);


	/* GETTERS */
	glm::vec3 GetPosition();
	glm::vec3 GetFrontCameraDirection();
	glm::vec3 GetUpCameraDirection();
	glm::vec3 GetRightCameraDirection();
	void UpdatePerspective(glm::vec2 FrameSize);
	void UpdateFieldOfView(float InFOV);
	float GetFieldOfView();
	glm::mat4 GetProjection();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetViewProjection();
	Universe* GetOwningUniverse();
	FrameBuffer* GetPickerFrame();

private:
	/* Camera World Attributes */
	glm::vec3 WorldPosition;
	glm::vec3 WorldUp;
	glm::vec3 FocusPoint;

	/* Camera Local Attributes */
	glm::vec3 ForwardVector;
	glm::vec3 UpVector;
	glm::vec3 RightVector;

	/* Camera options */
	GLfloat PanSpeed;
	GLfloat ZoomModifier;
	GLfloat OrbitSpeed;
	GLfloat CameraNearDistance;

	FrameBuffer* SceneFrame;
	FrameBuffer* PickerFrame;

	void UpdateCameraVectors();

	float NearClip;
	float FarClip;
	float FieldOfView;

	glm::mat4 Projection;
	glm::mat4 ViewMatrix;

	Universe* OwningUniverse;
};
