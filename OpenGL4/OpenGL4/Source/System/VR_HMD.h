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

struct VRDevice
{
	Asset* DeviceModel;
	vr::TrackedDeviceIndex_t DeviceID;
	vr::ETrackedControllerRole ControllerRole;

	VRDevice()
	{
		DeviceModel = NULL;
		ControllerRole = vr::TrackedControllerRole_Invalid;
		DeviceID = vr::k_unMaxTrackedDeviceCount + 1;
	}
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

	bool InitCompositor();
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

	VRDevice RightController;
	VRDevice LeftController;
	VRDevice HMDDevice;

	bool SetupStereoRenderTargets();
	bool CreateFrameBuffer(uint32_t InFrameWidth, uint32_t InFrameHeight, EyeFrameData &InEyeFrame);
	void CreateVRDevices();
	void RenderVRDevices();
	void RenderHMDEyes();
	glm::mat4 GetCurrentViewProjectionMatrix(vr::Hmd_Eye nEye);
};

