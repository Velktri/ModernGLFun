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


struct RegionData
{
	/*
		@TODO: Consider Min and Max size.
	*/

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
	TreeNode(int InNodeID, ImVec2 InRegionSize, Region* InRegion = NULL, TreeNode* InRight = NULL, TreeNode* InLeft = NULL);
	~TreeNode();

	bool IsLeaf();
	void Render();

	int GetNodeID();
	ImVec2 GetRegionSize();
	void ResizeNode(ImVec2 InAmount);

	Region* Contents;
	TreeNode* RightNode;
	TreeNode* LeftNode;


private:
	/** Helpers */
	void BeginRegionChild(int InRegionID, ImVec2 InSize, ImGuiWindowFlags flags);
	void EndRegionChild();

	int NodeID;
	ImVec2 RegionSize;
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
	ResizingData ResizingNode;
	int RegionCount;


	float SplitSpacing;

private:
	SDL_Window* Window;
	World* MyWorld;
	ImVec2 WindowDimensions;
	Region* HoveredRegion;
	Manager* MyManager;
	Input* MyInput;
	TreeNode* LayoutRoot;

	bool bQuitLayout;
	int PolledRegion;
	bool bSceneClicked;

	void SetDefaultStyle(std::string path);
	void ImportAsset();

	/** Creates all the regions when the Layout is created. */
	void GenerateDefaultLayout();

	void AssetEditor();

	bool MasterWindow();
	void RenderRegions();

	/** Update layout when application is resized. */
	void UpdateWindowSize();

	/** Update every region's size and position. */
	void ResizeRegions();
};

class Region
{
public:
	Region(ImVec2 InSize, ImVec2 InPosition, Layout* InLayout);
	~Region();

	virtual bool Render();
	void ReSize(ImVec2 InSize, ImVec2 InPosition);
	void ResizeByValue(ImVec2 InAmount);

	int RegionID;
	TreeNode* OwningNode;

	Layout* GetOwningLayout();
	ImVec2 GetSize();
	ImVec2 GetSceneSize();
	ImVec2 GetPosition();
	ImVec2 GetScenePosition();
	ImGuiWindowFlags GetStyleFlags();

	bool IsSceneHovered();

protected:
	ImVec2 Size;
	ImVec2 SceneSize;
	ImVec2 Position;
	ImVec2 ScenePosition;


	std::vector<RegionTypes> TypeList;
	Layout* OwningLayout;
	bool bIsSceneHovered;

	FrameBuffer* SceneFrame;
	FrameBuffer* PickerFrame;
	GLuint RenderFrame;

	ImGuiWindowFlags ContainerStyleFlags; //@TODO: implement
};

class Container : public Region
{
public:
	Container(ImVec2 InSize, ImVec2 InPosition, Layout* InLayout, RegionTypes InType = RegionTypes::None);

	virtual bool Render() override;

	RegionTypes GetType();

private:
	RegionTypes Type;

	/* Region Types */
	void TestRegion();
	void StatsRegion();
	void SceneRegion();
	void OutlinerRegion();
	void MainMenuRegion();

	/* Switch between Editors Panel Types */
	void PanelSwitcher();

	/* Splits a region into two child regions either horizontally or vertically. */
	void WindowSpliter();
	void SplitRegion(bool bVertical);
};

class Splitter : public Region
{
public:
	Splitter(ImVec2 InSize, ImVec2 InPosition, Layout* InLayout, bool InOrientation);

	virtual bool Render() override;
	void ResizeRegions(ImVec2 ResizeDelta);

private:
	bool bIsVertical;

	void HorizontalSplit();
	void VerticalSplit();
	void ResizeRecursion(TreeNode* InNode, bool OwnerIsVertical, bool LeftSideTraversal, float InResizeDelta);
};