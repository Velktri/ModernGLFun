#include "World.h"
#include "Input.h"
#include <imgui.h>
#include "imgui_impl_sdl_gl3.h"

/* Constants */
static const GLuint WIDTH = 1920;
static const GLuint HEIGHT = 1080;

int main(int argc, char *argv[]) {
	bool bIsRunning = true;
	/* SDL Init */
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_Window* window = SDL_CreateWindow("GeoViewer", 760, 180, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	/* GLEW Init */
	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_DEPTH_TEST);

	ImGui_ImplSdlGL3_Init(window);
	bool show_test_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImColor(114, 144, 154);

	World* MyWorld = new World(WIDTH, HEIGHT);
	Input* MyInput = new Input(MyWorld);


	MyWorld->StartClock();
	while (bIsRunning) {
		MyWorld->UpdateClock();
		MyWorld->GetLights()[0]->Translate(sin(MyWorld->GetTime()) / 20.0f, 0, cos(MyWorld->GetTime()) / 20.0f);

		MyInput->UpdateInput();
		bIsRunning = MyInput->ExecuteInput();

		ImGui_ImplSdlGL3_NewFrame(window);
		// 1. Show a simple window
		// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
		{
			static float f = 0.0f;
			ImGui::Text("Hello, world!");
			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
			ImGui::ColorEdit3("clear color", (float*)&clear_color);
			if (ImGui::Button("Test Window")) show_test_window ^= 1;
			if (ImGui::Button("Another Window")) show_another_window ^= 1;
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

		// 2. Show another simple window, this time using an explicit Begin/End pair
		if (show_another_window) {
			ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Another Window", &show_another_window);
			ImGui::Text("Hello");
			ImGui::End();
		}

		// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
		if (show_test_window) {
			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
			ImGui::ShowTestWindow(&show_test_window);
		}
		glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);


		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MyWorld->RenderWorld();
		ImGui::Render();

		SDL_GL_SwapWindow(window);
	}

#pragma region Clean Up
	/* Clean up */
	MyWorld->~World();

	SDL_GL_DeleteContext(context);
	SDL_Quit();
#pragma endregion 

	return 0;
}