#pragma once
#include <string>

class Panel {
public:
	Panel(std::string name);
	~Panel();

	std::string GetName();

private:
	std::string Name;
};

