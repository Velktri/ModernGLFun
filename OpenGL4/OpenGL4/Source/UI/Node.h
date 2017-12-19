#pragma once
#include <imgui.h>
#include <vector>

enum ConnectionType
{
	ConnectionType_Color,
	ConnectionType_Vec3,
	ConnectionType_Float,
	ConnectionType_Int,
};

struct Connection
{
	ImVec2 Position;
	ConnectionType Type;

	inline Connection()
	{
		Position.x = Position.y = 0.0f;
		input = 0;
	}

	union
	{
		float v3[3];
		float v;
		int i;
	};

	struct Connection* input;
	std::vector<Connection*> output;
};

class Node
{
	ImVec2 Position;
	ImVec2 Size;
	int Id;
	std::string Name;
	std::vector<Connection*> InputConnections;
	std::vector<Connection*> OutputConnections;
};