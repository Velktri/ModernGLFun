#include "World.h"
#include "Input.h"
#include "UI\Layout.h"
#include "FrameBuffer.h"
#include "Engine.h"
#include <imgui.h>
#include "imgui_impl_sdl_gl3.h"

int main(int argc, char *argv[]) {

	Engine* engine = new Engine();
	if (!engine->Init()) {
		return -1;
	}

	engine->Run();

	engine->CleanUp();

	return 0;
}