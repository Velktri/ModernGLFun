#include "VR_HMD.h"
#include "Models/MeshAsset.h"
#include "World.h"
#include "Universe.h"
#include "Manager.h"
#include <string>

VR_HMD::VR_HMD(Universe* InUniverse, vr::IVRSystem* InHMD)
{
	HMD = InHMD;
	SceneUniverse = InUniverse;

	NearClip = 0.1f;
	FarClip = 30.0f;

	LeftController = VRDevice();
	RightController = VRDevice();
	HMDDevice = VRDevice();
	CreateVRDevices();

	Projection_Left = GetHMDMatrixProjectionEye(vr::Eye_Left);
	Projection_Right = GetHMDMatrixProjectionEye(vr::Eye_Right);
	WorldPosition_Left = GetHMDMatrixPoseEye(vr::Eye_Left);
	WorldPosition_Right = GetHMDMatrixPoseEye(vr::Eye_Right);
	SetupStereoRenderTargets();
}


VR_HMD::~VR_HMD()
{
	glDeleteRenderbuffers(1, &LeftEyeFrame.DepthBufferId);
	glDeleteTextures(1, &LeftEyeFrame.RenderTextureId);
	glDeleteFramebuffers(1, &LeftEyeFrame.RenderFramebufferId);
	glDeleteTextures(1, &LeftEyeFrame.ResolveTextureId);
	glDeleteFramebuffers(1, &LeftEyeFrame.ResolveFramebufferId);

	glDeleteRenderbuffers(1, &RightEyeFrame.DepthBufferId);
	glDeleteTextures(1, &RightEyeFrame.RenderTextureId);
	glDeleteFramebuffers(1, &RightEyeFrame.RenderFramebufferId);
	glDeleteTextures(1, &RightEyeFrame.ResolveTextureId);
	glDeleteFramebuffers(1, &RightEyeFrame.ResolveFramebufferId);
}

glm::mat4 VR_HMD::GetHMDMatrixProjectionEye(vr::Hmd_Eye nEye)
{
	if (!HMD) { return glm::mat4(); }

	vr::HmdMatrix44_t mat = HMD->GetProjectionMatrix(nEye, NearClip, FarClip);

	return glm::mat4(
		mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
		mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
		mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
		mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
	);
}

glm::mat4 VR_HMD::GetHMDMatrixPoseEye(vr::Hmd_Eye nEye)
{
	return (HMD) ? glm::inverse(Hmd34ToGLM(HMD->GetEyeToHeadTransform(nEye))) : glm::mat4();
}

void VR_HMD::UpdateHMDMatrixPose()
{
	if (HMD)
	{
		vr::VRCompositor()->WaitGetPoses(TrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

		if (TrackedDevicePose[HMDDevice.DeviceID].bPoseIsValid)
		{
			HMDDevice.PoseData = glm::inverse(Hmd34ToGLM(TrackedDevicePose[HMDDevice.DeviceID].mDeviceToAbsoluteTracking));
		}

		if (TrackedDevicePose[LeftController.DeviceID].bPoseIsValid)
		{
			LeftController.PoseData = Hmd34ToGLM(TrackedDevicePose[LeftController.DeviceID].mDeviceToAbsoluteTracking);
		}

		if (TrackedDevicePose[RightController.DeviceID].bPoseIsValid)
		{
			RightController.PoseData = Hmd34ToGLM(TrackedDevicePose[RightController.DeviceID].mDeviceToAbsoluteTracking);
		}
	}
}

bool VR_HMD::SetupStereoRenderTargets()
{
	if (HMD)
	{
		HMD->GetRecommendedRenderTargetSize(&FrameWidth, &FrameHeight);

		CreateFrameBuffer(FrameWidth, FrameHeight, LeftEyeFrame);
		CreateFrameBuffer(FrameWidth, FrameHeight, RightEyeFrame);

		return true;
	}
	return false;
}

/* @TODO: update to use the Framebuffer class. */
bool VR_HMD::CreateFrameBuffer(uint32_t InFrameWidth, uint32_t InFrameHeight, EyeFrameData &InEyeFrame)
{
	glGenFramebuffers(1, &InEyeFrame.RenderFramebufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, InEyeFrame.RenderFramebufferId);

	glGenRenderbuffers(1, &InEyeFrame.DepthBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, InEyeFrame.DepthBufferId);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, InFrameWidth, InFrameHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, InEyeFrame.DepthBufferId);

	glGenTextures(1, &InEyeFrame.RenderTextureId);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, InEyeFrame.RenderTextureId);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, InFrameWidth, InFrameHeight, true);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, InEyeFrame.RenderTextureId, 0);

	glGenFramebuffers(1, &InEyeFrame.ResolveFramebufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, InEyeFrame.ResolveFramebufferId);

	glGenTextures(1, &InEyeFrame.ResolveTextureId);
	glBindTexture(GL_TEXTURE_2D, InEyeFrame.ResolveTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, InFrameWidth, InFrameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, InEyeFrame.ResolveTextureId, 0);

	// check FBO status
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) { return false; }

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

void VR_HMD::CreateVRDevices()
{
	for (vr::TrackedDeviceIndex_t TrackedDevice = vr::k_unTrackedDeviceIndex_Hmd; TrackedDevice < vr::k_unMaxTrackedDeviceCount; ++TrackedDevice)
	{
		if (HMD->IsTrackedDeviceConnected(TrackedDevice) && 
			HMD->GetTrackedDeviceClass(TrackedDevice) == vr::TrackedDeviceClass_Controller)
		{
			if (HMD->GetControllerRoleForTrackedDeviceIndex(TrackedDevice) == vr::TrackedControllerRole_LeftHand &&
				LeftController.ControllerRole == vr::TrackedControllerRole_Invalid)
			{
				LeftController.ControllerRole = vr::TrackedControllerRole_LeftHand;
				LeftController.DeviceID = TrackedDevice;
				LeftController.DeviceModel = SceneUniverse->GetManager()->BuildAsset("Models/Primitives/VRController.obj");
			}
			else if (HMD->GetControllerRoleForTrackedDeviceIndex(TrackedDevice) == vr::TrackedControllerRole_RightHand &&
					 RightController.ControllerRole == vr::TrackedControllerRole_Invalid)
			{
				RightController.ControllerRole = vr::TrackedControllerRole_RightHand;
				RightController.DeviceID = TrackedDevice;
				RightController.DeviceModel = SceneUniverse->GetManager()->BuildAsset("Models/Primitives/VRController.obj");
			}
		}

		if (HMD->IsTrackedDeviceConnected(TrackedDevice) &&
			HMD->GetTrackedDeviceClass(TrackedDevice) == vr::TrackedDeviceClass_HMD &&
			HMDDevice.DeviceModel == NULL)
		{
			HMDDevice.DeviceModel = SceneUniverse->GetManager()->BuildAsset("Models/Primitives/VRHMD.obj");
			HMDDevice.DeviceID = TrackedDevice;
		}
	}
}

void VR_HMD::RenderVRDevices()
{
	if (LeftController.ControllerRole == vr::TrackedControllerRole_Invalid ||
		RightController.ControllerRole == vr::TrackedControllerRole_Invalid ||
		HMDDevice.DeviceModel == NULL)
	{
		CreateVRDevices();
	}

	if (TrackedDevicePose[LeftController.DeviceID].bPoseIsValid) 
	{ 
		LeftController.DeviceModel->SetWorldSpace(LeftController.PoseData);
	}

	if (TrackedDevicePose[RightController.DeviceID].bPoseIsValid)
	{
		RightController.DeviceModel->SetWorldSpace(RightController.PoseData);
	}

	//if (TrackedDevicePose[HMDDevice.DeviceID].bPoseIsValid)
	//{
	//	HMDDevice.DeviceModel->SetWorldSpace(DevicePose[HMDDevice.DeviceID]);
	//}
}

void VR_HMD::RenderHMDEyes()
{
	glClearColor(0.35f, 0.35f, 0.35f, 1.0f);
	glEnable(GL_MULTISAMPLE);
	glm::vec3 HMDPosition = glm::vec3(HMDDevice.PoseData[0][3], HMDDevice.PoseData[1][3], HMDDevice.PoseData[2][3]);

	// Left Eye
	glBindFramebuffer(GL_FRAMEBUFFER, LeftEyeFrame.RenderFramebufferId);
	glViewport(0, 0, FrameWidth, FrameHeight);
	SceneUniverse->ActiveWorld->RenderWorld(HMDPosition, GetCurrentViewProjectionMatrix(vr::Eye_Left), glm::vec2(FrameWidth, FrameHeight));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_MULTISAMPLE);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, LeftEyeFrame.RenderFramebufferId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, LeftEyeFrame.ResolveFramebufferId);

	glBlitFramebuffer(0, 0, FrameWidth, FrameHeight, 0, 0, FrameWidth, FrameHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glEnable(GL_MULTISAMPLE);

	// Right Eye
	glBindFramebuffer(GL_FRAMEBUFFER, RightEyeFrame.RenderFramebufferId);
	glViewport(0, 0, FrameWidth, FrameHeight);
	SceneUniverse->ActiveWorld->RenderWorld(HMDPosition, GetCurrentViewProjectionMatrix(vr::Eye_Right), glm::vec2(FrameWidth, FrameHeight));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_MULTISAMPLE);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, RightEyeFrame.RenderFramebufferId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, RightEyeFrame.ResolveFramebufferId);

	glBlitFramebuffer(0, 0, FrameWidth, FrameHeight, 0, 0, FrameWidth, FrameHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

glm::mat4 VR_HMD::GetCurrentViewProjectionMatrix(vr::Hmd_Eye nEye)
{
	glm::mat4 matMVP;
	if (nEye == vr::Eye_Left)
	{
		matMVP = Projection_Left * WorldPosition_Left * HMDDevice.PoseData;
	}
	else if (nEye == vr::Eye_Right)
	{
		matMVP = Projection_Right * WorldPosition_Right * HMDDevice.PoseData;
	}

	return matMVP;
}

void VR_HMD::Render()
{
	RenderVRDevices();
	RenderHMDEyes();

	vr::Texture_t leftEyeTexture = { (void*) (uintptr_t) LeftEyeFrame.ResolveTextureId, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
	vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
	vr::Texture_t rightEyeTexture = { (void*) (uintptr_t) RightEyeFrame.ResolveTextureId, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
	vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);

	UpdateHMDMatrixPose();
}

bool VR_HMD::InitCompositor()
{
	vr::EVRInitError peError = vr::VRInitError_None;

	if (!vr::VRCompositor())
	{
		printf("Compositor initialization failed. See log file for details\n");
		return false;
	}

	return true;
}

glm::mat4 VR_HMD::Hmd34ToGLM(vr::HmdMatrix34_t InHmdM)
{
	return glm::mat4(
		InHmdM.m[0][0], InHmdM.m[1][0], InHmdM.m[2][0], 0.0,
		InHmdM.m[0][1], InHmdM.m[1][1], InHmdM.m[2][1], 0.0,
		InHmdM.m[0][2], InHmdM.m[1][2], InHmdM.m[2][2], 0.0,
		InHmdM.m[0][3], InHmdM.m[1][3], InHmdM.m[2][3], 1.0f
	);
}


EyeFrameData VR_HMD::GetLeftFrame() { return LeftEyeFrame; }
EyeFrameData VR_HMD::GetRightFrame() { return RightEyeFrame; }
uint32_t VR_HMD::GetFrameWidth() { return FrameWidth; }
uint32_t VR_HMD::GetFrameHeight() { return FrameHeight; }
