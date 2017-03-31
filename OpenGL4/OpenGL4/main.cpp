#include "Engine.h"

int main(int argc, char *argv[]) {
	Engine* engine = new Engine();
	if (!engine->Init()) {
		return -1;
	}

	engine->Run();
	engine->CleanUp();
	return 0;
}