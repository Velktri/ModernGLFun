#pragma once
#include "Asset.h"

class MeshComponent;

enum class ActiveHandle
{
	None,
	Translate_X,
	Translate_Y,
	Translate_Z
};

class Gizmo : public Asset {
public:
	Gizmo(GLuint InAssetID, Manager* InManager);
	~Gizmo();

	ActiveHandle ActiveAxis;

	virtual void Render(Shader* shader) override;
	void CalculateGizmoMovement(glm::vec3 CameraPosition, glm::vec3 RayCastDirection, glm::vec3 CameraLookDir);
	void AttachAsset(Asset* InAsset);

	/* GETTERS */
	bool IsGizmoSelected();

private:
	MeshComponent* TranslateX;
	MeshComponent* TranslateY;
	MeshComponent* TranslateZ;
	Asset* AttachedAsset;

	void GetAxisPlane(glm::vec3 InAxis, glm::vec3 CameraLookDir, glm::vec3 &PlaneNormal, glm::vec3 &NormalToRemove);
	glm::vec3 GetMovementVector(glm::vec3 CameraPosition, glm::vec3 RayCastDirection, glm::vec3 PlaneNormal, glm::vec3 NormalToRemove);
};

