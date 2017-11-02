#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class World;

class FrameBuffer {
public:
	FrameBuffer(GLuint InFrameSize_X, GLuint InFrameSize_Y);
	~FrameBuffer();

	void RenderWorldFrame(World* world, glm::vec2 FrameSize); 
	int RenderColorPick(World* world, glm::vec2 FrameSize, glm::vec2 pickerCoords);
	GLuint GetFrameTexture();


private:
	GLuint Framebuffer;
	GLuint FrameSize_X;
	GLuint FrameSize_Y;
	GLuint RenderBufferObject;
	GLuint FrameTexture;

	GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);
};

