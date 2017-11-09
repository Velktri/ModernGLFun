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

enum RegionTypes
{
	None = 0,
	MainMenu = 1,
	Scene = 2,
	Outliner = 3,
	Test = 4,
	Stats = 5,
	Spacer = 6,
	AssetEditor = 7
};

enum Primatives
{
	Cube = 0,	
	Plane = 1,
	Sphere = 2,
	Cylinder = 3,
	Curve = 4,
	Smooth = 5 // @TODO: temp, change later when working with normals.
};