#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <string>

#include <imgui.h>
#include "imgui_impl_sdl_gl3.h"

class World;
class Manager;

class Layout {
public:
	Layout(SDL_Window* InWindow, ImVec2 InWindowDimensions, std::string path = "");
	~Layout();

	bool RenderLayout(GLuint TextureColorBuffer);
	ImVec2 GetSceneDimensions();
	void SetWorld(World* InWorld);
	void SetManager(Manager* m);

	bool GetSceneHovering();

private:
	SDL_Window* Window;
	World* world;
	ImVec2 WindowDimensions;
	ImVec2 SceneDimensions;
	ImVec2 SceneSizeModifier;
	ImVec2 WindowPos;
	bool bIsHoveringScene;
	Manager* MyManager;

	void SetDefaultStyle(std::string path);
	void ImportAsset();
	void CreatePrimative(std::string name);

	bool MainMenu();
	void AssetEditor();
	void SceneWindow(GLuint TextureColorBuffer);
};

