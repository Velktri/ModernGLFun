#pragma once
#include "HelperTypes.h"
#include <SDL.h>
#include <GL/glew.h>
#include <vector>
#include <imgui.h>
#include "imgui_impl_sdl_gl3.h"

class Layout;
class TreeNode;
class FrameBuffer;


class Region
{
public:
	Region(Layout* InLayout, TreeNode* InOwningNode, RegionTypes InType = RegionTypes::None);
	~Region();

	virtual bool Render();

	Layout* GetOwningLayout();
	ImVec2 GetSceneSize();
	ImVec2 GetScenePosition();
	ImGuiWindowFlags GetStyleFlags();
	int GetRegionID();
	RegionTypes GetType();

	bool IsSceneHovered();

protected:
	ImVec2 SceneSize;
	ImVec2 ScenePosition;


	std::vector<RegionTypes> TypeList;
	Layout* OwningLayout;
	TreeNode* OwningNode;
	bool bIsSceneHovered;
	RegionTypes Type;

	FrameBuffer* SceneFrame;
	FrameBuffer* PickerFrame;
	GLuint RenderFrame;

	ImGuiWindowFlags ContainerStyleFlags;
};

class Container : public Region
{
public:
	Container(Layout* InLayout, TreeNode* InOwningNode, RegionTypes InType = RegionTypes::None);

	virtual bool Render() override;



private:
	/* Region Types */
	void TestRegion();
	void StatsRegion();
	void SceneRegion();
	void OutlinerRegion();
	void MainMenuRegion();
	void AssetEditorRegion();

	/* Switch between Editors Panel Types */
	void PanelSwitcher();

	/* Splits a region into two child regions either horizontally or vertically. */
	void WindowSpliter();
	void SplitRegion(bool bVertical);

	/* Region Styling */
	void BeginStyledMenuBar();
	void EndStyledMenuBar();
};

class Splitter : public Region
{
public:
	Splitter(ImVec2 InSize, Layout* InLayout, TreeNode* InOwningNode, bool InOrientation);

	virtual bool Render() override;

	/* Return true is region was resized. */
	bool ResizeRegions(ImVec2 ResizeDelta);

	void ResizeSplitter(ImVec2 InAmount);

	bool GetOrientation();
	ImVec2 GetSplitterSize();

private:
	bool bIsVertical;
	ImVec2 SplitterSize;

	void HorizontalSplit();
	void VerticalSplit();
	void ResizeRecursion(TreeNode* InNode, bool OwnerIsVertical, float InAmount, bool LeftSideTraversal, std::vector<TreeNode*> &NodeArray, bool &bCanResize);
	bool CheckNodeSize(TreeNode* InNode, bool bVertical, float InAmount);
};