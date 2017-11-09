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
	Region(Layout* InLayout, TreeNode* InOwningNode);
	~Region();

	virtual bool Render();

	Layout* GetOwningLayout();
	ImVec2 GetSceneSize();
	ImVec2 GetScenePosition();
	ImGuiWindowFlags GetStyleFlags();
	int GetRegionID();

	bool IsSceneHovered();

protected:
	ImVec2 SceneSize;
	ImVec2 ScenePosition;


	std::vector<RegionTypes> TypeList;
	Layout* OwningLayout;
	TreeNode* OwningNode;
	bool bIsSceneHovered;

	FrameBuffer* SceneFrame;
	FrameBuffer* PickerFrame;
	GLuint RenderFrame;

	ImGuiWindowFlags ContainerStyleFlags; //@TODO: implement
};

class Container : public Region
{
public:
	Container(Layout* InLayout, TreeNode* InOwningNode, RegionTypes InType = RegionTypes::None);

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
	void AssetEditorRegion();

	/* Switch between Editors Panel Types */
	void PanelSwitcher();

	/* Splits a region into two child regions either horizontally or vertically. */
	void WindowSpliter();
	void SplitRegion(bool bVertical);
};

class Splitter : public Region
{
public:
	Splitter(ImVec2 InSize, ImVec2 InPosition, Layout* InLayout, TreeNode* InOwningNode, bool InOrientation);

	virtual bool Render() override;
	void ResizeRegions(ImVec2 ResizeDelta);
	void ResizeSplitter(ImVec2 InAmount);

	bool GetOrientation();

private:
	bool bIsVertical;
	ImVec2 SplitterSize;

	void HorizontalSplit();
	void VerticalSplit();
	void ResizeRecursion(TreeNode* InNode, bool OwnerIsVertical, bool LeftSideTraversal, float InResizeDelta);
};