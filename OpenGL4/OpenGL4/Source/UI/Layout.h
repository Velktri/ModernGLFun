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
class TreeNode;
class Layout;

struct RegionData
{
	RegionTypes Type;
	ImVec2 Size;
	ImVec2 Position;
	bool bIsElastic;

	RegionData(RegionTypes InType = RegionTypes::None, 
			   ImVec2 InSize = ImVec2(0, 0), 
			   ImVec2 InPosition = ImVec2(0, 0), 
			   bool InElastic = true)
	{
		Type = InType;
		Size = InSize;
		Position = InPosition;
		bIsElastic = InElastic;
	}
};

struct ResizingData
{
	TreeNode* Node;
	ImVec2 ResizeAmount;

	ResizingData(TreeNode* InNode = NULL, ImVec2 InAmount = ImVec2())
	{
		Node = InNode;
		ResizeAmount = InAmount;
	}
};

class TreeNode
{
public:
	TreeNode(RegionData InData, Layout* InOwningLayout);
	~TreeNode();

	bool IsLeaf();
	void Render();
	void BuildSplitter(ImVec2 InSize, ImVec2 InPosition, bool InOrientation);

	int GetNodeID();
	Region* GetContents();
	Layout* GetOwningLayout();
	ImVec2 GetRegionSize();
	ImVec2 GetRegionPosition();
	void ResizeNode(ImVec2 InAmount);
	void NewSize(ImVec2 InAmount);


	TreeNode* RightNode;
	TreeNode* LeftNode;

private:
	/** Helpers */
	void BeginRegionChild(int InRegionID, ImVec2 InSize, ImGuiWindowFlags flags);
	void EndRegionChild();
	Region* CreateContents();

	int NodeID;
	RegionData Data;
	Layout* OwningLayout;
	Region* Contents;
};


class Layout
{
public:
	Layout(SDL_Window* InWindow, Manager* InManager, World* InWorld, Input* InInput, std::string path = "");
	~Layout();

	bool RenderLayout();
	void ShutDown();

	World* GetWorld();
	Input* GetInput();
	Manager* GetManager();
	Region* GetHoveredRegion();
	void SetHoveredRegion(Region* InRegion);
	bool IsSceneClicked();
	int GetPolledRegion();
	int GetGlobalMinSize();

	ResizingData ResizingNode;

	float SplitSpacing;
	int AddAndGetRegionCount();

	TreeNode* GenerateLayout(char* FilePath);
	void SaveLayout();

private:
	SDL_Window* Window;
	World* MyWorld;
	ImVec2 WindowDimensions;
	Region* HoveredRegion;
	Manager* MyManager;
	Input* MyInput;
	TreeNode* LayoutRoot;

	// @TODO: Add name/ID for multiple Layout instances.

	bool bQuitLayout;
	int PolledRegion;
	bool bSceneClicked;
	int RegionCount;
	int GlobalMinRegionSize;

	void LoadDefaultStyle(std::string path);
	void ImportAsset();

	/** Creates all the regions when the Layout is created. */
	void LoadDefaultLayout();

	bool MasterWindow();
	void RenderRegions();

	/** Update layout when application is resized. */
	void UpdateWindowSize();

	/** Update every region's size and position. */
	void ResizeRegions();

	ImVec2 RefreshContainerSizes(TreeNode* InNode);

	TreeNode* BuildNode(std::string NodeString);
};
