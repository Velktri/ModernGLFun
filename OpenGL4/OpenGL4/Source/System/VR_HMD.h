#pragma once
#include <GL/glew.h>

/* GLM */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <openvr.h>

class Asset;
class Universe;

struct EyeFrameData
{
	GLuint DepthBufferId;
	GLuint RenderTextureId;
	GLuint RenderFramebufferId;
	GLuint ResolveTextureId;
	GLuint ResolveFramebufferId;
};

class VR_HMD
{
public:
	VR_HMD(Universe* InUniverse, vr::IVRSystem* InHMD);
	~VR_HMD();


	glm::mat4 GetHMDMatrixProjectionEye(vr::Hmd_Eye nEye);
	glm::mat4 GetHMDMatrixPoseEye(vr::Hmd_Eye nEye);
	void UpdateHMDMatrixPose();

	/* GETTERS */
	EyeFrameData GetLeftFrame();
	EyeFrameData GetRightFrame();
	uint32_t GetFrameWidth();
	uint32_t GetFrameHeight();
	void Render();

private:
	vr::IVRSystem* HMD;
	Universe* SceneUniverse;

	glm::mat4 Projection_Left;
	glm::mat4 Projection_Right;
	glm::mat4 WorldPosition_Left;
	glm::mat4 WorldPosition_Right;

	float NearClip;
	float FarClip;

	/* Framebuffers */
	uint32_t FrameWidth;
	uint32_t FrameHeight;
	EyeFrameData LeftEyeFrame;
	EyeFrameData RightEyeFrame;

	vr::TrackedDevicePose_t TrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	glm::mat4 DevicePose[vr::k_unMaxTrackedDeviceCount];
	char DeviceClassChar[vr::k_unMaxTrackedDeviceCount];
	glm::mat4 HMDPose;
	int ValidPoseCount = 0;
	int TrackedControllerCount = 0;

	Asset* RightController;
	Asset* LeftController;

	bool SetupStereoRenderTargets();
	bool CreateFrameBuffer(uint32_t InFrameWidth, uint32_t InFrameHeight, EyeFrameData InEyeFrame);
	void CreateControllers();
	void RenderControllerAxes();
	void RenderHMDEyes();
};

