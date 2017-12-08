#include "VR_HMD.h"
#include "Models/MeshAsset.h"
#include "Universe.h"
#include "Manager.h"
#include <string>

VR_HMD::VR_HMD(Universe* InUniverse, vr::IVRSystem* InHMD)
{
	HMD = InHMD;
	SceneUniverse = InUniverse;

	NearClip = 0.1f;
	FarClip = 30.0f;

	LeftController = VRController();
	RightController = VRController();
	CreateControllers();

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
	if (!HMD) { return glm::mat4(); }

	vr::HmdMatrix34_t matEyeRight = HMD->GetEyeToHeadTransform(nEye);
	glm::mat4 matrixObj(
		matEyeRight.m[0][0], matEyeRight.m[1][0], matEyeRight.m[2][0], 0.0,
		matEyeRight.m[0][1], matEyeRight.m[1][1], matEyeRight.m[2][1], 0.0,
		matEyeRight.m[0][2], matEyeRight.m[1][2], matEyeRight.m[2][2], 0.0,
		matEyeRight.m[0][3], matEyeRight.m[1][3], matEyeRight.m[2][3], 1.0f
	);

	return glm::inverse(matrixObj);
}

void VR_HMD::UpdateHMDMatrixPose()
{
	if (HMD)
	{
		vr::VRCompositor()->WaitGetPoses(TrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

		ValidPoseCount = 0;
		std::string PoseClasses = "";
		for (int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice)
		{
			if (TrackedDevicePose[nDevice].bPoseIsValid)
			{
				ValidPoseCount++;

				vr::HmdMatrix34_t matPose = TrackedDevicePose[nDevice].mDeviceToAbsoluteTracking;
				glm::mat4 matrixObj(
					matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0,
					matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0,
					matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0,
					matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f
				);
				DevicePose[nDevice] = matrixObj;

				if (DeviceClassChar[nDevice] == 0)
				{
					switch (HMD->GetTrackedDeviceClass(nDevice))
					{
						case vr::TrackedDeviceClass_Controller:        DeviceClassChar[nDevice] = 'C'; break;
						case vr::TrackedDeviceClass_HMD:               DeviceClassChar[nDevice] = 'H'; break;
						case vr::TrackedDeviceClass_Invalid:           DeviceClassChar[nDevice] = 'I'; break;
						case vr::TrackedDeviceClass_GenericTracker:    DeviceClassChar[nDevice] = 'G'; break;
						case vr::TrackedDeviceClass_TrackingReference: DeviceClassChar[nDevice] = 'T'; break;
						default:                                       DeviceClassChar[nDevice] = '?'; break;
					}
				}
				PoseClasses += DeviceClassChar[nDevice];
			}
		}

		if (TrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
		{
			HMDPose = DevicePose[vr::k_unTrackedDeviceIndex_Hmd];
			glm::inverse(HMDPose);
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
bool VR_HMD::CreateFrameBuffer(uint32_t InFrameWidth, uint32_t InFrameHeight, EyeFrameData InEyeFrame)
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

void VR_HMD::CreateControllers()
{
	for (vr::TrackedDeviceIndex_t TrackedDevice = vr::k_unTrackedDeviceIndex_Hmd + 1; TrackedDevice < vr::k_unMaxTrackedDeviceCount; ++TrackedDevice)
	{
		if (HMD->IsTrackedDeviceConnected(TrackedDevice) && 
			HMD->GetTrackedDeviceClass(TrackedDevice) == vr::TrackedDeviceClass_Controller)
		{
			if (HMD->GetControllerRoleForTrackedDeviceIndex(TrackedDevice) == vr::TrackedControllerRole_LeftHand &&
				LeftController.ControllerRole == vr::TrackedControllerRole_Invalid)
			{
				LeftController.ControllerRole = vr::TrackedControllerRole_LeftHand;
				LeftController.DeviceID = TrackedDevice;
				LeftController.ControllerModel = SceneUniverse->GetManager()->BuildAsset("Models/Primitives/VRController.obj");
			}
			else if (HMD->GetControllerRoleForTrackedDeviceIndex(TrackedDevice) == vr::TrackedControllerRole_RightHand &&
					 RightController.ControllerRole == vr::TrackedControllerRole_Invalid)
			{
				RightController.ControllerRole = vr::TrackedControllerRole_RightHand;
				RightController.DeviceID = TrackedDevice;
				RightController.ControllerModel = SceneUniverse->GetManager()->BuildAsset("Models/Primitives/VRController.obj");
			}
		}
	}
}

void VR_HMD::RenderControllerAxes()
{
	//// don't draw controllers if somebody else has input focus
	//if (m_pHMD->IsInputFocusCapturedByAnotherProcess())
	//	return;

	if (LeftController.ControllerRole == vr::TrackedControllerRole_Invalid ||
		RightController.ControllerRole == vr::TrackedControllerRole_Invalid)
	{
		CreateControllers();
	}

	if (TrackedDevicePose[LeftController.DeviceID].bPoseIsValid) 
	{ 
		const glm::mat4& mat = DevicePose[LeftController.DeviceID];
		LeftController.ControllerModel->SetWorldSpace(mat);
	}

	if (TrackedDevicePose[RightController.DeviceID].bPoseIsValid)
	{
		const glm::mat4& mat = DevicePose[RightController.DeviceID];
		RightController.ControllerModel->SetWorldSpace(mat);
	}
}

void VR_HMD::RenderHMDEyes()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_MULTISAMPLE);

	// Left Eye
	glBindFramebuffer(GL_FRAMEBUFFER, LeftEyeFrame.RenderFramebufferId);
	glViewport(0, 0, FrameWidth, FrameHeight);
	//SceneWorld->RenderWorld(vr::Eye_Left);
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
	//SceneWorld->RenderWorld(vr::Eye_Right);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_MULTISAMPLE);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, RightEyeFrame.RenderFramebufferId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, RightEyeFrame.ResolveFramebufferId);

	glBlitFramebuffer(0, 0, FrameWidth, FrameHeight, 0, 0, FrameWidth, FrameHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void VR_HMD::Render()
{
	RenderControllerAxes();
	RenderHMDEyes();
	UpdateHMDMatrixPose();
}




EyeFrameData VR_HMD::GetLeftFrame() { return LeftEyeFrame; }
EyeFrameData VR_HMD::GetRightFrame() { return RightEyeFrame; }
uint32_t VR_HMD::GetFrameWidth() { return FrameWidth; }
uint32_t VR_HMD::GetFrameHeight() { return FrameHeight; }
