#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <string>
#include <vector>
#include "HelperTypes.h"

#include <imgui.h>
#include "imgui_impl_sdl_gl3.h"

class World;
class Manager;
class Region;


struct RegionData
{
	RegionTypes Type;
	ImVec2 Size;
	ImVec2 Position;
	bool bIsElastic;

	RegionData(RegionTypes InType = RegionTypes::None, ImVec2 InSize = ImVec2(0, 0), ImVec2 InPosition = ImVec2(0, 0), bool InElastic = true)
	{
		Type = InType;
		Size = InSize;
		Position = InPosition;
		bIsElastic = InElastic;
	}
};


class Layout {
public:
	Layout(SDL_Window* InWindow, std::string path = "");
	~Layout();

	bool RenderLayout(GLuint InTextureColorBuffer);
	ImVec2 GetSceneDimensions();
	void SetWorld(World* InWorld);
	void SetManager(Manager* InManager);

	bool GetSceneHovering();
	GLuint GetTextureColorBuffer();
	void SetQuit(bool InQuit);

private:
	SDL_Window* Window;
	World* MyWorld;
	ImVec2 WindowDimensions;
	bool bIsHoveringScene;
	Manager* MyManager;
	float DefaultSpacing;
	std::vector<std::vector<Region*>> ChildWindowGrid;
	GLuint TextureColorBuffer;
	bool bQuitLayout;

	void SetDefaultStyle(std::string path);
	void ImportAsset();
	void CreatePrimative(std::string name);

	/** Creates all the regions when the Layout is created. */
	void GenerateDefaultLayout();

	void AssetEditor();

	bool MasterWindow();
	void RenderRegions();

	/** Update layout when application is resized. */
	void UpdateWindowSize();

	/** Update every region's size and position. */
	void ResizeRegions();

	void HSpliter(const char* Name, float* X, float* Y);
	void VSpliter(const char* Name, float* X, float* Y);
};

class Region
{
public:
	Region(ImVec2 InSize, ImVec2 InPosition, RegionTypes InType, Layout* InLayout);
	~Region();

	bool Render();
	void ReSize(ImVec2 InSize, ImVec2 InPosition);

	int RegionID;

	Layout* GetOwningLayout();
	ImVec2 GetSize();
private:
	ImVec2 Size;
	ImVec2 Position;
	RegionTypes Type;
	std::vector<RegionTypes> TypeList;

	void TestRegion();
	void StatsRegion();
	void SceneRegion();
	void OutlinerRegion();
	void MainMenuRegion();

	bool bIsRegionHovered;
	Layout* OwningLayout;

	/* Switch between Editors Panel Types */
	void PanelSwitcher();

	void BeginRegionChild(char* RegionName, ImGuiWindowFlags flags);
	void EndRegionChild();
};

