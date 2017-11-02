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
class FrameBuffer;
class Input;


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
	Layout(SDL_Window* InWindow, Manager* InManager, World* InWorld, Input* InInput, std::string path = "");
	~Layout();

	bool RenderLayout();

	World* GetWorld();
	Input* GetInput();
	Manager* GetManager();
	Region* GetHoveredRegion();
	void SetHoveredRegion(Region* InRegion);
	void ShutDown();

private:
	SDL_Window* Window;
	World* MyWorld;
	ImVec2 WindowDimensions;
	Region* HoveredRegion;
	Manager* MyManager;
	Input* MyInput;
	float DefaultSpacing;
	std::vector<std::vector<Region*>> ChildWindowGrid;
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
	Region(ImVec2 InSize, ImVec2 InPosition, Layout* InLayout, RegionTypes InType = RegionTypes::None);
	~Region();

	bool Render();
	void ReSize(ImVec2 InSize, ImVec2 InPosition);

	int RegionID;

	Layout* GetOwningLayout();
	ImVec2 GetSize();
	ImVec2 GetSceneSize();
	ImVec2 GetPosition();
	ImVec2 GetScenePosition();
	RegionTypes GetType();

private:
	ImVec2 Size;
	ImVec2 SceneSize;
	ImVec2 Position;
	ImVec2 ScenePosition;
	RegionTypes Type;
	std::vector<RegionTypes> TypeList;
	Layout* OwningLayout;
	bool bIsRegionHovered;

	FrameBuffer* SceneFrame;
	FrameBuffer* PickerFrame;
	GLuint RenderFrame;

	/* Region Types */
	void TestRegion();
	void StatsRegion();
	void SceneRegion();
	void OutlinerRegion();
	void MainMenuRegion();


	/** Helpers */
	/* Switch between Editors Panel Types */
	void PanelSwitcher();
	void BeginRegionChild(char* RegionName, ImGuiWindowFlags flags);
	void EndRegionChild();
};

