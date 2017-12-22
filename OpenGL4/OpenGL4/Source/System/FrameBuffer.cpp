#include "FrameBuffer.h"
#include "World.h"
#include "System/Manager.h"
#include "Camera.h"
#include <iostream>

FrameBuffer::FrameBuffer(GLuint InFrameSize_X, GLuint InFrameSize_Y)
{
	FrameSize_X = InFrameSize_X;
	FrameSize_Y = InFrameSize_Y;

	glGenFramebuffers(1, &Framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);

	FrameTexture = generateAttachmentTexture(false, false);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FrameTexture, 0);

	glGenRenderbuffers(1, &RenderBufferObject);
	glBindRenderbuffer(GL_RENDERBUFFER, RenderBufferObject);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, FrameSize_X, FrameSize_Y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RenderBufferObject);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &Framebuffer);
}

void FrameBuffer::RenderWorldFrame(Camera* InCamera, World* InWorld, glm::vec2 FrameSize)
{
	glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
	glViewport(0, 0, FrameSize.x, FrameSize.y);
	glClearColor(0.35f, 0.35f, 0.35f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	InWorld->RenderWorld(InCamera->GetPosition(), InCamera->GetViewMatrix(), InCamera->GetProjection(), FrameSize);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(0.35f, 0.55f, 0.55f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void FrameBuffer::RenderColorPick(Camera* InCamera, World* InWorld, glm::vec2 FrameSize)
{
	glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
	glViewport(0, 0, FrameSize.x, FrameSize.y);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	InWorld->RenderColorWorld(InCamera, FrameSize);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void FrameBuffer::GetFrameSelection(Manager* InManager, glm::vec2 pickerCoords) // @TODO: expand to use box select in future. Move color selection to different function.
{
	unsigned char data[4];
	glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
	glReadPixels(pickerCoords.x, FrameSize_Y - pickerCoords.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data); //box selection here. Make sure that the the Y pixels are correctly inverted when they are read when creating box selection.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	InManager->CheckForSelection(data[0] + data[1] * 256 + data[2] * 256 * 256);
}

GLuint FrameBuffer::generateAttachmentTexture(GLboolean depth, GLboolean stencil) {
	// What enum to use?
	GLenum attachment_type;
	if (!depth && !stencil)
	{ 		
		attachment_type = GL_RGB;
	}
	else if (depth && !stencil)
	{
		attachment_type = GL_DEPTH_COMPONENT;
	}
	else if (!depth && stencil)
	{
		attachment_type = GL_STENCIL_INDEX;
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (!depth && !stencil)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, FrameSize_X, FrameSize_Y, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, FrameSize_X, FrameSize_Y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

GLuint FrameBuffer::GetFrameTexture() { return FrameTexture; }