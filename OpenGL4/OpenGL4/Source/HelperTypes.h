#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

enum class RegionTypes
{
	None,
	MainMenu,
	Scene,
	Outliner,
	Test,
	Stats,
	AssetEditor,
	Spacer
};

enum class Primatives
{
	Cube,	
	Plane,
	Sphere,
	Cylinder,
	ECurve,
	Smooth // @TODO: temp, change later when working with normals.
};
enum class EFrameTypes
{
	None,
	Scene,
	ColorPicker,
	VR_LeftEye,
	VR_RightEye
};