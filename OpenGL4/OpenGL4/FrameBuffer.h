#pragma once
#include <GL/glew.h>
#include <iostream>
#include "World.h"

class FrameBuffer {
public:
	FrameBuffer(GLuint InFrameSize_X, GLuint InFrameSize_Y);
	~FrameBuffer();

	void RenderWorldFrame(World* world);
	GLuint GetFrameTexture();


private:
	GLuint Framebuffer;
	GLuint FrameSize_X;
	GLuint FrameSize_Y;
	GLuint RenderBufferObject;

	GLuint FrameTexture;

	GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);
};

