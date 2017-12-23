#include "Gizmo.h"
#include "Components/MeshComponent.h"
#include "System/Transforms.h"
#include "ModelData/Element.h"
#include "System/Manager.h"
#include <glm/gtx/intersect.hpp>

Gizmo::Gizmo(GLuint InAssetID, Manager* InManager) : Asset(InAssetID, InManager)
{
	ActiveAxis = ActiveHandle::None;
	Asset* AttachedAsset = NULL;

	TranslateX = new MeshComponent(this, "Models/Primitives/TranslationGizmo.obj");
	TranslateX->RotateComponent(0.0f, 0.0f, 270.0f);
	TranslateX->Color = glm::vec3(1.0f, 0.0f, 0.0f);

	TranslateY = new MeshComponent(this, "Models/Primitives/TranslationGizmo.obj");
	TranslateY->Color = glm::vec3(0.0f, 1.0f, 0.0f);

	TranslateZ = new MeshComponent(this, "Models/Primitives/TranslationGizmo.obj");
	TranslateZ->RotateComponent(90.0f, 0.0f, 0.0f);
	TranslateZ->Color = glm::vec3(0.0f, 0.0f, 1.0f);

	GetRoot()->AddComponent(TranslateX);
	GetRoot()->AddComponent(TranslateY);
	GetRoot()->AddComponent(TranslateZ);
}

Gizmo::~Gizmo()
{

}

void Gizmo::Render(Shader* shader)
{
	// @TODO: build logic to show/hide different gizmo types
	if (AttachedAsset)
	{
		Asset::Render(shader);
	}
}

void Gizmo::CalculateGizmoMovement(glm::vec3 CameraPosition, glm::vec3 RayCastDirection, glm::vec3 CameraLookDir)
{
	glm::vec3 AxisX = glm::vec3(AssetTransform->WorldSpaceOrientation[0]);
	glm::vec3 AxisY = glm::vec3(AssetTransform->WorldSpaceOrientation[1]);
	glm::vec3 AxisZ = glm::vec3(AssetTransform->WorldSpaceOrientation[2]);
	glm::vec3 PlaneNormal = AxisY;
	glm::vec3 NormalToRemove;

	switch (ActiveAxis)
	{
		case ActiveHandle::Translate_X:
			GetAxisPlane(AxisX, CameraLookDir, PlaneNormal, NormalToRemove);
			break;
		case ActiveHandle::Translate_Y:
			GetAxisPlane(AxisY, CameraLookDir, PlaneNormal, NormalToRemove);
			break;
		case ActiveHandle::Translate_Z:
			GetAxisPlane(AxisZ, CameraLookDir, PlaneNormal, NormalToRemove);
			break;
		default:
			break;
	}

	MyManager->GetSelectedAsset()->TranslateAsset(GetMovementVector(CameraPosition, RayCastDirection, PlaneNormal, NormalToRemove));
}

void Gizmo::GetAxisPlane(glm::vec3 InAxis, glm::vec3 CameraLookDir, glm::vec3 &PlaneNormal, glm::vec3 &NormalToRemove)
{
	glm::vec3 AxisX = glm::vec3(AssetTransform->WorldSpaceOrientation[0]);
	glm::vec3 AxisY = glm::vec3(AssetTransform->WorldSpaceOrientation[1]);
	glm::vec3 AxisZ = glm::vec3(AssetTransform->WorldSpaceOrientation[2]);

	float XDot = glm::abs(glm::dot(CameraLookDir, AxisX));
	float YDot = glm::abs(glm::dot(CameraLookDir, AxisY));
	float ZDot = glm::abs(glm::dot(CameraLookDir, AxisZ));

	if (glm::dot(InAxis, AxisX) > .1f)
	{
		PlaneNormal = (YDot > ZDot) ? AxisY : AxisZ;
		NormalToRemove = (YDot > ZDot) ? AxisZ : AxisY;
	}
	else if (glm::dot(InAxis, AxisY) > .1f)
	{
		PlaneNormal = (XDot > ZDot) ? AxisX : AxisZ;
		NormalToRemove = (XDot > ZDot) ? AxisZ : AxisX;
	}
	else
	{
		PlaneNormal = (XDot > YDot) ? AxisX : AxisY;
		NormalToRemove = (XDot > YDot) ? AxisY : AxisX;
	}
}

glm::vec3 Gizmo::GetMovementVector(glm::vec3 CameraPosition, glm::vec3 RayCastDirection, glm::vec3 PlaneNormal, glm::vec3 NormalToRemove)
{
	glm::vec3 RequestedPosition = GetOrigin();

	float DotProductWithPlaneNormal = glm::dot(RayCastDirection, PlaneNormal);
	if (DotProductWithPlaneNormal < 0) { PlaneNormal *= -1; }

	//check to make sure we're not co-planar
	if (glm::abs(DotProductWithPlaneNormal) > 0.00001f)
	{
		//Get Ray-Plane intersection point
		float FarClip = 1000.0f;
		glm::intersectRayPlane(CameraPosition, -RayCastDirection, RequestedPosition, PlaneNormal, FarClip);
		RequestedPosition = CameraPosition + (-RayCastDirection * FarClip);
	}

	// Calculate Offset
	glm::vec3 OffsetAxis, ReturnPosition;
	switch (ActiveAxis)
	{
		case ActiveHandle::Translate_X:
			ReturnPosition = glm::vec3(RequestedPosition.x, GetOrigin().y, GetOrigin().z);
			OffsetAxis = 1.62f * glm::vec3(AssetTransform->WorldSpaceOrientation[0]);
			break;
		case ActiveHandle::Translate_Y:
			ReturnPosition = glm::vec3(GetOrigin().x, RequestedPosition.y, GetOrigin().z);
			OffsetAxis = 1.62f * glm::vec3(AssetTransform->WorldSpaceOrientation[1]);
			break;
		case ActiveHandle::Translate_Z:
			ReturnPosition = glm::vec3(GetOrigin().x, GetOrigin().y, RequestedPosition.z);
			OffsetAxis = 1.62f * glm::vec3(AssetTransform->WorldSpaceOrientation[2]);
			break;
		default:
			break;
	}

	return (ReturnPosition - GetOrigin()) - OffsetAxis;
}

void Gizmo::AttachAsset(Asset* InAsset)
{ 
	AttachedAsset = InAsset;
	AssetTransform = AttachedAsset->GetAssetTransforms();
	UpdateComponentOrientation();
}

bool Gizmo::IsGizmoSelected() { return (ActiveAxis != ActiveHandle::None); }
