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
	Scene = 1,
	Outliner = 2,
	Test = 3,
	Stats = 4
};