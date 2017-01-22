#pragma once
#include <string>
#include <vector>
#include <SDL.h>
#include <GL/glew.h>
#include "Panel.h"

#include <imgui.h>
#include "imgui_impl_sdl_gl3.h"

class Layout {
public:
	Layout(SDL_Window* InWindow, ImVec2 InWindowDimensions, std::string path = "");
	~Layout();

	void RenderLayout(GLuint TextureColorBuffer);
	ImVec2 GetSceneDimensions();

private:
	SDL_Window* Window;
	ImVec2 WindowDimensions;
	ImVec2 SceneDimensions;
	void BuildLayout(std::string path);
	void BuildDefaultLayout();
	void SetDefaultStyle(std::string path);
	std::vector<Panel*> PanelList;
};

