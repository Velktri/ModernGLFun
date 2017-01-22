#include "Panel.h"



Panel::Panel(std::string InName) {
	Name = InName;
}


Panel::~Panel() {
}

std::string Panel::GetName() {
	return Name;
}
