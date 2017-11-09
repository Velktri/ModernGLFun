#include "Engine.h"

// @TODO: Add comments to all relevant functions in the programs.
int main(int argc, char *argv[])
{
	if (!Engine::getInstance().Init()) { return -1; }
	Engine::getInstance().Run();
	Engine::getInstance().CleanUp();
	return 0;
}