#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class World;
class Camera;

class FrameBuffer {
public:
	FrameBuffer(GLuint InFrameSize_X = 1920.0f, GLuint InFrameSize_Y = 1080.0f);
	~FrameBuffer();

	void RenderWorldFrame(Camera* InCamera, World* world, glm::vec2 FrameSize); 
	void RenderColorPick(Camera* InCamera, World* world, glm::vec2 FrameSize/*, glm::vec2 pickerCoords*/);
	GLuint GetFrameTexture();


private:
	GLuint Framebuffer;
	GLuint FrameSize_X;
	GLuint FrameSize_Y;
	GLuint RenderBufferObject;
	GLuint FrameTexture;

	GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);
};

