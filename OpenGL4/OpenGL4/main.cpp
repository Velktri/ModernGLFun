#include "Engine.h"

int main(int argc, char *argv[])
{
	if (!Engine::getInstance().Init())
	{ 
		return -1; 
	}

	Engine::getInstance().Run();
	Engine::getInstance().CleanUp();
	return 0;
}