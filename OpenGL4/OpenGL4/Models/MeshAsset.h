#pragma once
#include "Asset.h"

class Manager;

class MeshAsset : public Asset 
{
public:
	MeshAsset(GLuint InAssetID, Manager* InManager, std::string path = "");
	~MeshAsset();
};

