#include "Layout.h"
#include "System/FrameBuffer.h"
#include "System/World.h"
#include "System/Manager.h"
#include "System/Input.h"
#include "Models/Asset.h"
#include "ModelData/Mesh.h"
#include "ModelData/Curve.h"
#include "System/Timer.h"
#include <Windows.h>
#include <vector>
#include <tchar.h>
#include "imgui_internal.h"

Layout::Layout(SDL_Window* InWindow, Manager* InManager, World* InWorld, Input* InInput, std::string path)
{
	Window = InWindow;
	MyManager = InManager;
	MyWorld = InWorld;
	MyInput = InInput;

	ResizingNode = ResizingData();
	HoveredRegion = NULL;
	bQuitLayout = false;

	UpdateWindowSize();
	ImGui_ImplSdlGL3_Init(Window);
	SetDefaultStyle(path);
	GenerateDefaultLayout();
}

Layout::~Layout()
{
	LayoutRoot->~TreeNode();
}

// @TODO: Clean up; design system for easy saving and creating custom layouts
void Layout::GenerateDefaultLayout()
{
	// @TODO: account for border spacing (SplitSpacing).
	RegionData RegionList = RegionData(RegionTypes::Stats,	WindowDimensions, ImVec2(0, 0), false);
	LayoutRoot = new TreeNode(0, RegionList, this);
	RegionCount = 1;
}

bool Layout::RenderLayout()
{
	UpdateWindowSize();
	ImGui_ImplSdlGL3_NewFrame(Window);

	bool bIsRunning = !MasterWindow();
	ImGui::Render();
	return bIsRunning;
}

World* Layout::GetWorld() { return MyWorld; }
Input* Layout::GetInput() { return MyInput; }
Manager* Layout::GetManager() { return MyManager; }
Region* Layout::GetHoveredRegion() {  return HoveredRegion;  }
void Layout::SetHoveredRegion(Region* InRegion) { HoveredRegion = InRegion; }
bool Layout::IsSceneClicked() { return bSceneClicked; }
int Layout::GetPolledRegion() { return PolledRegion; }
void Layout::ShutDown() { bQuitLayout = true; }

void Layout::SetDefaultStyle(std::string path)
{ 
	ImGui::StyleColorsDark(); 
	SplitSpacing = 4.0f;
}

void Layout::ImportAsset()
{
	//if (world) {
	//	world->GetTimer()->Stop();
	//	OPENFILENAME ofn;

	//	ZeroMemory(&ofn, sizeof(ofn));

	//	ofn.lStructSize = sizeof(ofn);
	//	ofn.hwndOwner = 0;
	//	ofn.lpstrDefExt = 0;
	//	ofn.lpstrFile  = new TCHAR[MAX_PATH];
	//	ofn.lpstrFile[0] = '\0';
	//	ofn.nMaxFile = MAX_PATH;
	//	ofn.lpstrFilter = "Object Files (*.obj)\0*.obj\0All Files (*.*)\0*.*\0\0";
	//	ofn.nFilterIndex = 1;
	//	ofn.lpstrInitialDir = _T("C:\\");
	//	ofn.lpstrTitle = _T("Import");
	//	ofn.Flags = OFN_SHOWHELP;

	//	GetOpenFileName(&ofn);
	//	MyManager->BuildAsset(ofn.lpstrFile);
	//	world->GetTimer()->Start();
	//} else {
	//	printf("Current UI has no Access to World*");
	//}
}

void Layout::AssetEditor()
{
	ImGui::SetNextWindowSize(ImVec2(420, 450), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(WindowDimensions.x - 420, 20), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Asset editor", false);
	Asset* SelectedAsset = MyManager->GetSelectedAsset();
	ImGui::BeginChild("left pane", ImVec2(150, 0), true);
	for each (Asset* a in MyManager->GetAssets())
	{
		if (ImGui::Selectable(a->Name.c_str(), SelectedAsset == a)) { MyManager->SetSelectedAsset(a); }
	}
	ImGui::EndChild();
	ImGui::SameLine();
	if (SelectedAsset)
	{
		//if (SelectedAsset->GetMesh()->GetType() == CURVE) {
		//	Curve* curve = (Curve*) SelectedAsset->GetMesh();
		//	ImGui::BeginGroup();
		//		ImVec2 i = ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing());
		//			ImGui::BeginChild("Item view", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()));
		//				ImGui::Text("P0");
		//				ImGui::Separator();
		//										if (ImGui::Button("Move X")) { curve->UpdateControlPoint(1, glm::vec3(1.0f, 0.0f, 0.0f)); }
		//				ImGui::SameLine();		if (ImGui::Button("Move Y")) { curve->UpdateControlPoint(1, glm::vec3(0.0f, 1.0f, 0.0f)); }
		//				ImGui::SameLine();		if (ImGui::Button("Move Z")) { curve->UpdateControlPoint(1, glm::vec3(0.0f, 0.0f, 1.0f)); }

		//										if (ImGui::Button("Move -X")) { curve->UpdateControlPoint(1, glm::vec3(-1.0f, 0.0f, 0.0f)); }
		//				ImGui::SameLine();		if (ImGui::Button("Move -Y")) { curve->UpdateControlPoint(1, glm::vec3(0.0f, -1.0f, 0.0f)); }
		//				ImGui::SameLine();		if (ImGui::Button("Move -Z")) { curve->UpdateControlPoint(1, glm::vec3(0.0f, 0.0f, -1.0f)); }

		//				ImGui::Text("P1");
		//				ImGui::Separator();
		//										if (ImGui::Button("Move X2")) { curve->UpdateControlPoint(2, glm::vec3(1.0f, 0.0f, 0.0f)); }
		//				ImGui::SameLine();		if (ImGui::Button("Move Y2")) { curve->UpdateControlPoint(2, glm::vec3(0.0f, 1.0f, 0.0f)); }
		//				ImGui::SameLine();		if (ImGui::Button("Move Z2")) { curve->UpdateControlPoint(2, glm::vec3(0.0f, 0.0f, 1.0f)); }

		//										if (ImGui::Button("Move -X2")) { curve->UpdateControlPoint(2, glm::vec3(-1.0f, 0.0f, 0.0f)); }
		//				ImGui::SameLine();		if (ImGui::Button("Move -Y2")) { curve->UpdateControlPoint(2, glm::vec3(0.0f, -1.0f, 0.0f)); }
		//				ImGui::SameLine();		if (ImGui::Button("Move -Z2")) { curve->UpdateControlPoint(2, glm::vec3(0.0f, 0.0f, -1.0f)); }

		//				ImGui::Text("P2");
		//				ImGui::Separator();
		//										if (ImGui::Button("Move X3")) { curve->UpdateControlPoint(3, glm::vec3(1.0f, 0.0f, 0.0f)); }
		//				ImGui::SameLine();		if (ImGui::Button("Move Y3")) { curve->UpdateControlPoint(3, glm::vec3(0.0f, 1.0f, 0.0f)); }
		//				ImGui::SameLine();		if (ImGui::Button("Move Z3")) { curve->UpdateControlPoint(3, glm::vec3(0.0f, 0.0f, 1.0f)); }

		//										if (ImGui::Button("Move -X3")) { curve->UpdateControlPoint(3, glm::vec3(-1.0f, 0.0f, 0.0f)); }
		//				ImGui::SameLine();		if (ImGui::Button("Move -Y3")) { curve->UpdateControlPoint(3, glm::vec3(0.0f, -1.0f, 0.0f)); }
		//				ImGui::SameLine();		if (ImGui::Button("Move -Z3")) { curve->UpdateControlPoint(3, glm::vec3(0.0f, 0.0f, -1.0f)); }

		//				ImGui::Separator();
		//			ImGui::EndChild();
		//	ImGui::EndGroup();
		//} else if (SelectedAsset->GetMesh()->GetType() == MESH) {
		//	ImGui::BeginGroup();
		//	ImVec2 i = ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing());
		//		ImGui::BeginChild("Item view", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()));
		//			ImGui::Text(SelectedAsset->Name.c_str());
		//			ImGui::Separator();
		//									if (ImGui::Button("Move X")) { SelectedAsset->TranslateAsset(1, 0, 0); }
		//			ImGui::SameLine();		if (ImGui::Button("Move Y")) { SelectedAsset->TranslateAsset(0, 1, 0); }
		//			ImGui::SameLine();		if (ImGui::Button("Move Z")) { SelectedAsset->TranslateAsset(0, 0, 1); }

		//									if (ImGui::Button("Move -X")) { SelectedAsset->TranslateAsset(-1, 0, 0); }
		//			ImGui::SameLine();		if (ImGui::Button("Move -Y")) { SelectedAsset->TranslateAsset(0, -1, 0); }
		//			ImGui::SameLine();		if (ImGui::Button("Move -Z")) { SelectedAsset->TranslateAsset(0, 0, -1); }

		//			ImGui::Separator();

		//			ImGui::Text("Mesh Properties");
		//			ImGui::Separator();

		//			static int selection = -1;
		//			ImGui::Text(MyManager->GetSelectedAsset()->GetMesh() ? MyManager->GetSelectedAsset()->GetMesh()->GetName().c_str() : "None");

		//			ImGui::SameLine();		if (ImGui::Button("Set Mesh")) { ImGui::OpenPopup("select"); }

		//			if (ImGui::BeginPopup("select")) {
		//				std::vector<Element*> meshList = MyManager->GetMeshList();
		//				for (int i = 0; i < meshList.size(); i++) {
		//					if (ImGui::Selectable(meshList[i]->GetName().c_str())) { selection = i; SelectedAsset->SetMesh(meshList.at(i)); }
		//				}

		//				ImGui::EndPopup();
		//			}
		//		ImGui::EndChild();
		//		ImGui::BeginChild("buttons");
		//									if (ImGui::Button("Revert")) {}
		//			ImGui::SameLine();		if (ImGui::Button("Save")) {}
		//		ImGui::EndChild();
		//	ImGui::EndGroup();
		//}
	}
	ImGui::End();
}

bool Layout::MasterWindow()
{
	//ImGuiIO& io = ImGui::GetIO();
	//io.MouseDrawCursor = true;

	ImGui::SetNextWindowSize(WindowDimensions, ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2());
	ImGui::Begin("MasterLayout", false, ImGuiWindowFlags_NoMove |
										 ImGuiWindowFlags_NoCollapse |
										 ImGuiWindowFlags_NoResize |
										 ImGuiWindowFlags_NoTitleBar |
										 ImGuiWindowFlags_NoScrollbar |
										 ImGuiWindowFlags_NoSavedSettings);
		
		RenderRegions();
	ImGui::End();
	ImGui::PopStyleVar(3);
	return bQuitLayout;
}

void Layout::RenderRegions()
{
	bSceneClicked = (MyInput && MyInput->PollSelectionRequest(&PolledRegion)) ? true : false;
	LayoutRoot->Render();

	if (ResizingNode.Node)
	{
		Splitter* split = dynamic_cast<Splitter*>(ResizingNode.Node->Contents);
		if (split)
		{
			split->ResizeRegions(ResizingNode.ResizeAmount);
		}

		ResizingNode.Node = NULL;
		ResizingNode.ResizeAmount = ImVec2();
	}
}

//@TODO: Look into maybe cleaning up potential overhead
void Layout::UpdateWindowSize()
{
	int W, H;
	SDL_GetWindowSize(Window, &W, &H);
	if (WindowDimensions.x != W || WindowDimensions.y != H)
	{
		WindowDimensions = ImVec2(W, H);
		ResizeRegions();
	}
}

void Layout::ResizeRegions()
{
	// @TODO: try getting the growth/shrink by % and changing regions by that amount,
}





// @TODO: Give proper styling to dropdown menus.
static void ShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(450.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

Region::Region(Layout* InLayout, TreeNode* InOwningNode)
{
	OwningNode = InOwningNode;
	OwningLayout = InLayout;
	bIsSceneHovered = false;
	RenderFrame = 0;

	TypeList.push_back(RegionTypes::None);
	TypeList.push_back(RegionTypes::MainMenu);
	TypeList.push_back(RegionTypes::Scene);
	TypeList.push_back(RegionTypes::Outliner);
	TypeList.push_back(RegionTypes::Test);
	TypeList.push_back(RegionTypes::Stats);

	ContainerStyleFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar;
}

Region::~Region()
{
	if (SceneFrame) { SceneFrame->~FrameBuffer(); }
	if (PickerFrame) { PickerFrame->~FrameBuffer(); }
}

bool Region::Render() { return true; }
Layout* Region::GetOwningLayout() { return OwningLayout; }
ImVec2 Region::GetSceneSize() { return SceneSize; }
ImVec2 Region::GetScenePosition() { return ScenePosition; }
ImGuiWindowFlags Region::GetStyleFlags() { return ContainerStyleFlags; }
bool Region::IsSceneHovered() { return bIsSceneHovered; }








Container::Container(Layout* InLayout, TreeNode* InOwningNode, RegionTypes InType) : Region(InLayout, InOwningNode)
{
	Type = InType;
}

bool Container::Render()
{
	switch (Type)
	{
		case Scene:
			SceneRegion();
			break;
		case Outliner:
			OutlinerRegion();
			break;
		case Test:
			TestRegion();
			break;
		case Stats:
			StatsRegion();
			break;
		case MainMenu:
			MainMenuRegion();
			break;
		default:
			PanelSwitcher();
			break;
	}

	return true;
}

void Container::StatsRegion()
{
	/* @TODO: Add Stats like FPS, Memory, etc. */

	//BeginRegionChild("Stats", ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(3.0f, 0.0f));
	ImGui::BeginMenuBar();
	PanelSwitcher();
	WindowSpliter();
	ImGui::EndMenuBar();

	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, (ImVec4) ImColor(200, 200, 0));
	std::string newName = "Stats_" + std::to_string(OwningNode->GetNodeID());
	ImGui::BeginChild(newName.c_str(), OwningNode->GetRegionSize(), true);

		ImGui::TextColored(ImVec4(0.7f, 0.0f, 0.7f, 1.0f), "NODE: %d", OwningNode->GetNodeID());
		ImGui::TextColored(ImVec4(0.7f, 0.0f, 0.7f, 1.0f), "NODE SIZE: %f, %f", OwningNode->GetRegionSize().x, OwningNode->GetRegionSize().y);

	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
}

void Container::SceneRegion()
{
	//BeginRegionChild("Scene", ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar);
	ImGui::Spacing();

	ImGui::BeginMenuBar();
	PanelSwitcher();
	ImGui::Indent(15.0f);

	ImGui::SameLine();		 if (ImGui::Button("SceneFrame")) { RenderFrame = SceneFrame->GetFrameTexture(); };
	ImGui::SameLine();		 if (ImGui::Button("PickerFrame")) { RenderFrame = PickerFrame->GetFrameTexture(); };

	ImGui::SameLine();		 if (ImGui::Button("Cube")) { OwningLayout->GetManager()->BuildPrimative(Primatives::Cube); };
	ImGui::SameLine();		 if (ImGui::Button("Plane")) { OwningLayout->GetManager()->BuildPrimative(Primatives::Plane); };
	ImGui::SameLine();		 if (ImGui::Button("Sphere")) { OwningLayout->GetManager()->BuildPrimative(Primatives::Sphere); };
	ImGui::SameLine();		 if (ImGui::Button("Cylinder")) { OwningLayout->GetManager()->BuildPrimative(Primatives::Cylinder); };
	ImGui::SameLine();		 if (ImGui::Button("SmoothTest")) { OwningLayout->GetManager()->BuildPrimative(Primatives::Smooth); }; // @TODO: change with enum
	ImGui::SameLine();		 if (ImGui::Button("Curve")) { OwningLayout->GetManager()->BuildPrimative(Primatives::Curve); };

	ImGui::SameLine();		 if (ImGui::Button("Clear Lines")) { OwningLayout->GetWorld()->ClearLines(); };

	float MenuSize = ImGui::GetCurrentWindow()->MenuBarHeight();
	ImGui::EndMenuBar();

	SceneSize = OwningNode->GetRegionSize();
	ScenePosition = OwningNode->GetRegionPosition();
	SceneSize.y -= MenuSize;
	ScenePosition.y += MenuSize;

	ImGui::BeginGroup();
	if (!SceneFrame || !PickerFrame)
	{
		if (!SceneFrame)
		{
			SceneFrame = new FrameBuffer(SceneSize.x, SceneSize.y);
			RenderFrame = SceneFrame->GetFrameTexture();
		}

		if (!PickerFrame) { PickerFrame = new FrameBuffer(SceneSize.x, SceneSize.y); } // @TODO: PickerFrame spawned upside down, need further real time testing
	}
	else
	{
		glViewport(0, 0, SceneSize.x, SceneSize.y);

		if (OwningLayout->IsSceneClicked() && OwningLayout->GetPolledRegion() == OwningNode->GetNodeID())
		{
			glm::vec2 coords = OwningLayout->GetInput()->StartSelectionCoods;
			coords.x -= ScenePosition.x;
			coords.y -= ScenePosition.y;
			printf("%f, %f\n", coords.x, coords.y);
			OwningLayout->GetManager()->CheckForSelection(PickerFrame->RenderColorPick(OwningLayout->GetWorld(), glm::vec2(SceneSize.x, SceneSize.y), coords));
		}

		SceneFrame->RenderWorldFrame(OwningLayout->GetWorld(), glm::vec2(SceneSize.x, SceneSize.y));
	}

	ImGui::Image((GLuint*) RenderFrame, SceneSize, ImVec2(0, 1), ImVec2(1, 0), ImColor(255, 255, 255, 255), ImVec4(0, 0, 0, 0));
	bIsSceneHovered = false;
	if (ImGui::IsItemHovered()) { bIsSceneHovered = true; }
	ImGui::EndGroup();
	//EndRegionChild();
}

void Container::OutlinerRegion()
{
	/* @TODO: Add Scene hierarchy here. */
	//BeginRegionChild("Outliner", ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar);
	ImGui::BeginMenuBar();
	PanelSwitcher();
	ImGui::EndMenuBar();
	//EndRegionChild();



	//ImGui::Begin("Splitter test");

	////static float w = 200.0f;
	////static float h = 300.0f;

	//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));
	//ImGui::BeginChild("child1", Size, true);
	//ImGui::EndChild();

	//ImGui::SameLine();
	//ImGui::InvisibleButton("vsplitter", ImVec2(1.0f, Size.y));
	//if (ImGui::IsItemActive())
	//Size.x += ImGui::GetIO().MouseDelta.x;
	//ImGui::SameLine();

	//ImGui::BeginChild("child2", ImVec2(0, Size.y), true);
	//ImGui::EndChild();

	//ImGui::InvisibleButton("hsplitter", ImVec2(-1, 1.0f));
	//if (ImGui::IsItemActive())
	//Size.y += ImGui::GetIO().MouseDelta.y;

	//ImGui::BeginChild("child3", ImVec2(0,0), true);
	//ImGui::EndChild();
	//ImGui::PopStyleVar();

	//ImGui::End();
	//

}

void Container::MainMenuRegion()
{
	//BeginRegionChild("MenuBar", ImGuiWindowFlags_NoScrollbar);
	PanelSwitcher(); ImGui::SameLine();

	if (ImGui::Button("File")) { ImGui::OpenPopup("File"); } ImGui::SameLine();
	if (ImGui::BeginPopup("File"))
	{
		if (ImGui::Selectable("Quit")) { OwningLayout->ShutDown(); }
		ImGui::EndPopup();

	}

	if (ImGui::Button("Edit")) { ImGui::OpenPopup("Edit"); }
	if (ImGui::BeginPopup("Edit"))
	{
		if (ImGui::Selectable("Undo")) { printf("Pressed Undo!\n"); }
		if (ImGui::Selectable("Redo", false, ImGuiSelectableFlags_Disabled)) { /* Disabled item */ }
		ImGui::Separator();
		if (ImGui::Selectable("Cut")) { printf("Pressed Cut!\n"); }
		if (ImGui::Selectable("Copy")) { printf("Pressed Copy!\n"); }
		if (ImGui::Selectable("Paste")) { printf("Pressed Paste!\n"); }
		ImGui::EndPopup();
	}
	//EndRegionChild();
}

void Container::TestRegion()
{
	static bool show_app_style_editor = false;
	static bool show_app_metrics = false;
	static bool show_app_about = false;

	if (show_app_metrics) ImGui::ShowMetricsWindow(&show_app_metrics);
	if (show_app_style_editor) { ImGui::Begin("Style Editor", &show_app_style_editor); ImGui::ShowStyleEditor(); ImGui::End(); }
	if (show_app_about)
	{
		ImGui::Begin("About ImGui", &show_app_about, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("dear imgui, %s", ImGui::GetVersion());
		ImGui::Separator();
		ImGui::Text("By Omar Cornut and all github contributors.");
		ImGui::Text("ImGui is licensed under the MIT License, see LICENSE for more information.");
		ImGui::End();
	}

	static bool no_border = true;
	static bool no_scrollbar = false;
	static bool no_menu = false;

	ImGuiWindowFlags window_flags = 0;
	if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
	if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;

	char buf[256];
	sprintf_s(buf, "Test_%d", GetRegionID());
	ImGui::BeginChild(buf, OwningNode->GetRegionSize()/*Size*/, false, window_flags);
	// Menu
	if (ImGui::BeginMenuBar())
	{
		PanelSwitcher();

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("Metrics", NULL, &show_app_metrics);
			ImGui::MenuItem("Style Editor", NULL, &show_app_style_editor);
			ImGui::MenuItem("About ImGui", NULL, &show_app_about);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::Spacing();
	ImGui::Text("dear imgui says hello. (%s)", IMGUI_VERSION);
	ImGui::Spacing();

	if (ImGui::CollapsingHeader("Help"))
	{
		ImGui::TextWrapped("This window is being created by the ShowTestWindow() function. Please refer to the code for programming reference.\n\nUser Guide:");
		ImGui::ShowUserGuide();
	}

	if (ImGui::CollapsingHeader("Window options"))
	{
		ImGui::Checkbox("No menu", &no_menu); ImGui::SameLine();
		ImGui::Checkbox("No border", &no_border); ImGui::SameLine();
		ImGui::Checkbox("No scrollbar", &no_scrollbar);	

		if (ImGui::TreeNode("Style"))
		{
			ImGui::ShowStyleEditor();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Logging"))
		{
			ImGui::TextWrapped("The logging API redirects all text output so you can easily capture the content of a window or a block. Tree nodes can be automatically expanded. You can also call ImGui::LogText() to output directly to the log without a visual output.");
			ImGui::LogButtons();
			ImGui::TreePop();
		}
	}

	if (ImGui::CollapsingHeader("Widgets"))
	{
		if (ImGui::TreeNode("Basic"))
		{
			static int clicked = 0;
			if (ImGui::Button("Button")) 
				clicked++;
			if (clicked & 1)
			{
				ImGui::SameLine();
				ImGui::Text("Thanks for clicking me!");
			}

			static bool check = true;
			ImGui::Checkbox("checkbox", &check);

			static int e = 0;
			ImGui::RadioButton("radio a", &e, 0); ImGui::SameLine();
			ImGui::RadioButton("radio b", &e, 1); ImGui::SameLine();
			ImGui::RadioButton("radio c", &e, 2);

			// Color buttons, demonstrate using PushID() to add unique identifier in the ID stack, and changing style.
			for (int i = 0; i < 7; i++)
			{
				if (i > 0) ImGui::SameLine();
				ImGui::PushID(i);
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i/7.0f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i/7.0f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i/7.0f, 0.8f, 0.8f));
				ImGui::Button("Click");
				ImGui::PopStyleColor(3);
				ImGui::PopID();
			}

			ImGui::Text("Hover over me");
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("I am a tooltip");

			ImGui::SameLine();
			ImGui::Text("- or me");
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("I am a fancy tooltip");
				static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
				ImGui::PlotLines("Curve", arr, IM_ARRAYSIZE(arr));
				ImGui::EndTooltip();
			}

			// Testing ImGuiOnceUponAFrame helper.
			//static ImGuiOnceUponAFrame once;
			//for (int i = 0; i < 5; i++)
			//    if (once)
			//        ImGui::Text("This will be displayed only once.");

			ImGui::Separator();

			ImGui::LabelText("label", "Value");

			static int item = 1;
			ImGui::Combo("combo", &item, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");   // Combo using values packed in a single constant string (for really quick combo)

			const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK" };
			static int item2 = -1;
			ImGui::Combo("combo scroll", &item2, items, IM_ARRAYSIZE(items));   // Combo using proper array. You can also pass a callback to retrieve array value, no need to create/copy an array just for that.

			{
				static char str0[128] = "Hello, world!";
				static int i0=123;
				static float f0=0.001f;
				ImGui::InputText("input text", str0, IM_ARRAYSIZE(str0));
				ImGui::SameLine(); ShowHelpMarker("Hold SHIFT or use mouse to select text.\n" "CTRL+Left/Right to word jump.\n" "CTRL+A or double-click to select all.\n" "CTRL+X,CTRL+C,CTRL+V clipboard.\n" "CTRL+Z,CTRL+Y undo/redo.\n" "ESCAPE to revert.\n");

				ImGui::InputInt("input int", &i0);
				ImGui::SameLine(); ShowHelpMarker("You can apply arithmetic operators +,*,/ on numerical values.\n  e.g. [ 100 ], input \'*2\', result becomes [ 200 ]\nUse +- to subtract.\n");

				ImGui::InputFloat("input float", &f0, 0.01f, 1.0f);

				static float vec4a[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
				ImGui::InputFloat3("input float3", vec4a);
			}

			{
				static int i1=50, i2=42;
				ImGui::DragInt("drag int", &i1, 1);
				ImGui::SameLine(); ShowHelpMarker("Click and drag to edit value.\nHold SHIFT/ALT for faster/slower edit.\nDouble-click or CTRL+click to input value.");

				ImGui::DragInt("drag int 0..100", &i2, 1, 0, 100, "%.0f%%");

				static float f1=1.00f, f2=0.0067f;
				ImGui::DragFloat("drag float", &f1, 0.005f);
				ImGui::DragFloat("drag small float", &f2, 0.0001f, 0.0f, 0.0f, "%.06f ns");
			}

			{
				static int i1=0;
				ImGui::SliderInt("slider int", &i1, -1, 3);
				ImGui::SameLine(); ShowHelpMarker("CTRL+click to input value.");

				static float f1=0.123f, f2=0.0f;
				ImGui::SliderFloat("slider float", &f1, 0.0f, 1.0f, "ratio = %.3f");
				ImGui::SliderFloat("slider log float", &f2, -10.0f, 10.0f, "%.4f", 3.0f);
				static float angle = 0.0f;
				ImGui::SliderAngle("slider angle", &angle);
			}

			static float col1[3] = { 1.0f,0.0f,0.2f };
			static float col2[4] = { 0.4f,0.7f,0.0f,0.5f };
			ImGui::ColorEdit3("color 1", col1);
			ImGui::SameLine(); ShowHelpMarker("Click on the colored square to open a color picker.\nRight-click on the colored square to show options.\nCTRL+click on individual component to input value.\n");

			ImGui::ColorEdit4("color 2", col2);

			const char* listbox_items[] = { "Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon" };
			static int listbox_item_current = 1;
			ImGui::ListBox("listbox\n(single select)", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items), 4);

			//static int listbox_item_current2 = 2;
			//ImGui::PushItemWidth(-1);
			//ImGui::ListBox("##listbox2", &listbox_item_current2, listbox_items, IM_ARRAYSIZE(listbox_items), 4);
			//ImGui::PopItemWidth();

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Trees"))
		{
			if (ImGui::TreeNode("Basic trees"))
			{
				for (int i = 0; i < 5; i++)
					if (ImGui::TreeNode((void*)(intptr_t)i, "Child %d", i))
					{
						ImGui::Text("blah blah");
						ImGui::SameLine(); 
						if (ImGui::SmallButton("print")) printf("Child %d pressed", i);
						ImGui::TreePop();
					}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Advanced, with Selectable nodes"))
			{
				ShowHelpMarker("This is a more standard looking tree with selectable nodes.\nClick to select, CTRL+Click to toggle, click on arrows or double-click to open.");
				static bool align_label_with_current_x_position = false;
				ImGui::Checkbox("Align label with current X position)", &align_label_with_current_x_position);
				ImGui::Text("Hello!");
				if (align_label_with_current_x_position)
					ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

				static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
				int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
				ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize()*3); // Increase spacing to differentiate leaves from expanded contents.
				for (int i = 0; i < 6; i++)
				{
					// Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
					ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0);
					if (i < 3)
					{
						// Node
						bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
						if (ImGui::IsItemClicked()) 
							node_clicked = i;
						if (node_open)
						{
							ImGui::Text("Blah blah\nBlah Blah");
							ImGui::TreePop();
						}
					}
					else
					{
						// Leaf: The only reason we have a TreeNode at all is to allow selection of the leaf. Otherwise we can use BulletText() or TreeAdvanceToLabelPos()+Text().
						node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
						ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Leaf %d", i);
						if (ImGui::IsItemClicked()) 
							node_clicked = i;
					}
				}
				if (node_clicked != -1)
				{
					// Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
					if (ImGui::GetIO().KeyCtrl)
						selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
					else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
						selection_mask = (1 << node_clicked);           // Click to single-select
				}
				ImGui::PopStyleVar();
				if (align_label_with_current_x_position)
					ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Collapsing Headers"))
		{
			static bool closable_group = true;
			ImGui::Checkbox("Enable extra group", &closable_group);
			if (ImGui::CollapsingHeader("Header"))
			{
				ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
				for (int i = 0; i < 5; i++)
					ImGui::Text("Some content %d", i);
			}
			if (ImGui::CollapsingHeader("Header with a close button", &closable_group))
			{
				ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
				for (int i = 0; i < 5; i++)
					ImGui::Text("More content %d", i);
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Bullets"))
		{
			ImGui::BulletText("Bullet point 1");
			ImGui::BulletText("Bullet point 2\nOn multiple lines");
			ImGui::Bullet(); ImGui::Text("Bullet point 3 (two calls)");
			ImGui::Bullet(); ImGui::SmallButton("Button");
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Text"))
		{
			if (ImGui::TreeNode("Colored Text"))
			{
				// Using shortcut. You can use PushStyleColor()/PopStyleColor() for more flexibility.
				ImGui::TextColored(ImVec4(1.0f,0.0f,1.0f,1.0f), "Pink");
				ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "Yellow");
				ImGui::TextDisabled("Disabled");
				ImGui::SameLine(); ShowHelpMarker("The TextDisabled color is stored in ImGuiStyle.");
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Word Wrapping"))
			{
				// Using shortcut. You can use PushTextWrapPos()/PopTextWrapPos() for more flexibility.
				ImGui::TextWrapped("This text should automatically wrap on the edge of the window. The current implementation for text wrapping follows simple rules suitable for English and possibly other languages.");
				ImGui::Spacing();

				static float wrap_width = 200.0f;
				ImGui::SliderFloat("Wrap width", &wrap_width, -20, 600, "%.0f");

				ImGui::Text("Test paragraph 1:");
				ImVec2 pos = ImGui::GetCursorScreenPos();
				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x + wrap_width, pos.y), ImVec2(pos.x + wrap_width + 10, pos.y + ImGui::GetTextLineHeight()), IM_COL32(255,0,255,255));
				ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
				ImGui::Text("The lazy dog is a good dog. This paragraph is made to fit within %.0f pixels. Testing a 1 character word. The quick brown fox jumps over the lazy dog.", wrap_width);
				ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255,255,0,255));
				ImGui::PopTextWrapPos();

				ImGui::Text("Test paragraph 2:");
				pos = ImGui::GetCursorScreenPos();
				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x + wrap_width, pos.y), ImVec2(pos.x + wrap_width + 10, pos.y + ImGui::GetTextLineHeight()), IM_COL32(255,0,255,255));
				ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
				ImGui::Text("aaaaaaaa bbbbbbbb, c cccccccc,dddddddd. d eeeeeeee   ffffffff. gggggggg!hhhhhhhh");
				ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255,255,0,255));
				ImGui::PopTextWrapPos();

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("UTF-8 Text"))
			{
				// UTF-8 test with Japanese characters
				// (needs a suitable font, try Arial Unicode or M+ fonts http://mplus-fonts.sourceforge.jp/mplus-outline-fonts/index-en.html)
				// - From C++11 you can use the u8"my text" syntax to encode literal strings as UTF-8
				// - For earlier compiler, you may be able to encode your sources as UTF-8 (e.g. Visual Studio save your file as 'UTF-8 without signature')
				// - HOWEVER, FOR THIS DEMO FILE, BECAUSE WE WANT TO SUPPORT COMPILER, WE ARE *NOT* INCLUDING RAW UTF-8 CHARACTERS IN THIS SOURCE FILE.
				//   Instead we are encoding a few string with hexadecimal constants. Don't do this in your application!
				// Note that characters values are preserved even by InputText() if the font cannot be displayed, so you can safely copy & paste garbled characters into another application.
				ImGui::TextWrapped("CJK text will only appears if the font was loaded with the appropriate CJK character ranges. Call io.Font->LoadFromFileTTF() manually to load extra character ranges.");
				ImGui::Text("Hiragana: \xe3\x81\x8b\xe3\x81\x8d\xe3\x81\x8f\xe3\x81\x91\xe3\x81\x93 (kakikukeko)");
				ImGui::Text("Kanjis: \xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e (nihongo)");
				static char buf[32] = "\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e"; // "nihongo"
				ImGui::InputText("UTF-8 input", buf, IM_ARRAYSIZE(buf));
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Images"))
		{
			ImGui::TextWrapped("Below we are displaying the font texture (which is the only texture we have access to in this demo). Use the 'ImTextureID' type as storage to pass pointers or identifier to your own texture data. Hover the texture for a zoomed view!");
			ImVec2 tex_screen_pos = ImGui::GetCursorScreenPos();
			float tex_w = (float)ImGui::GetIO().Fonts->TexWidth;
			float tex_h = (float)ImGui::GetIO().Fonts->TexHeight;
			ImTextureID tex_id = ImGui::GetIO().Fonts->TexID;
			ImGui::Text("%.0fx%.0f", tex_w, tex_h);
			ImGui::Image(tex_id, ImVec2(tex_w, tex_h), ImVec2(0,0), ImVec2(1,1), ImColor(255,255,255,255), ImColor(255,255,255,128));
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				float focus_sz = 32.0f;
				float focus_x = ImGui::GetMousePos().x - tex_screen_pos.x - focus_sz * 0.5f; if (focus_x < 0.0f) focus_x = 0.0f; else if (focus_x > tex_w - focus_sz) focus_x = tex_w - focus_sz;
				float focus_y = ImGui::GetMousePos().y - tex_screen_pos.y - focus_sz * 0.5f; if (focus_y < 0.0f) focus_y = 0.0f; else if (focus_y > tex_h - focus_sz) focus_y = tex_h - focus_sz;
				ImGui::Text("Min: (%.2f, %.2f)", focus_x, focus_y);
				ImGui::Text("Max: (%.2f, %.2f)", focus_x + focus_sz, focus_y + focus_sz);
				ImVec2 uv0 = ImVec2((focus_x) / tex_w, (focus_y) / tex_h);
				ImVec2 uv1 = ImVec2((focus_x + focus_sz) / tex_w, (focus_y + focus_sz) / tex_h);
				ImGui::Image(tex_id, ImVec2(128,128), uv0, uv1, ImColor(255,255,255,255), ImColor(255,255,255,128));
				ImGui::EndTooltip();
			}
			ImGui::TextWrapped("And now some textured buttons..");
			static int pressed_count = 0;
			for (int i = 0; i < 8; i++)
			{
				ImGui::PushID(i);
				int frame_padding = -1 + i;     // -1 = uses default padding
				if (ImGui::ImageButton(tex_id, ImVec2(32,32), ImVec2(0,0), ImVec2(32.0f/tex_w,32/tex_h), frame_padding, ImColor(0,0,0,255)))
					pressed_count += 1;
				ImGui::PopID();
				ImGui::SameLine();
			}
			ImGui::NewLine();
			ImGui::Text("Pressed %d times.", pressed_count);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Selectables"))
		{
			if (ImGui::TreeNode("Basic"))
			{
				static bool selected[4] = { false, true, false, false };
				ImGui::Selectable("1. I am selectable", &selected[0]);
				ImGui::Selectable("2. I am selectable", &selected[1]);
				ImGui::Text("3. I am not selectable");
				ImGui::Selectable("4. I am selectable", &selected[2]);
				if (ImGui::Selectable("5. I am double clickable", selected[3], ImGuiSelectableFlags_AllowDoubleClick))
					if (ImGui::IsMouseDoubleClicked(0))
						selected[3] = !selected[3];
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Rendering more text into the same block"))
			{
				static bool selected[3] = { false, false, false };
				ImGui::Selectable("main.c", &selected[0]);    ImGui::SameLine(300); ImGui::Text(" 2,345 bytes");
				ImGui::Selectable("Hello.cpp", &selected[1]); ImGui::SameLine(300); ImGui::Text("12,345 bytes");
				ImGui::Selectable("Hello.h", &selected[2]);   ImGui::SameLine(300); ImGui::Text(" 2,345 bytes");
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("In columns"))
			{
				ImGui::Columns(3, NULL, false);
				static bool selected[16] = { 0 };
				for (int i = 0; i < 16; i++)
				{
					char label[32]; sprintf_s(label, "Item %d", i);
					if (ImGui::Selectable(label, &selected[i])) {}
					ImGui::NextColumn();
				}
				ImGui::Columns(1);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Grid"))
			{
				static bool selected[16] = { true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true };
				for (int i = 0; i < 16; i++)
				{
					ImGui::PushID(i);
					if (ImGui::Selectable("Sailor", &selected[i], 0, ImVec2(50,50)))
					{
						int x = i % 4, y = i / 4;
						if (x > 0) selected[i - 1] ^= 1;
						if (x < 3) selected[i + 1] ^= 1;
						if (y > 0) selected[i - 4] ^= 1;
						if (y < 3) selected[i + 4] ^= 1;
					}
					if ((i % 4) < 3) ImGui::SameLine();
					ImGui::PopID();
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Filtered Text Input"))
		{
			static char buf1[64] = ""; ImGui::InputText("default", buf1, 64);
			static char buf2[64] = ""; ImGui::InputText("decimal", buf2, 64, ImGuiInputTextFlags_CharsDecimal);
			static char buf3[64] = ""; ImGui::InputText("hexadecimal", buf3, 64, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase);
			static char buf4[64] = ""; ImGui::InputText("uppercase", buf4, 64, ImGuiInputTextFlags_CharsUppercase);
			static char buf5[64] = ""; ImGui::InputText("no blank", buf5, 64, ImGuiInputTextFlags_CharsNoBlank);
			struct TextFilters { static int FilterImGuiLetters(ImGuiTextEditCallbackData* data) { if (data->EventChar < 256 && strchr("imgui", (char)data->EventChar)) return 0; return 1; } };
			static char buf6[64] = ""; ImGui::InputText("\"imgui\" letters", buf6, 64, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);

			ImGui::Text("Password input");
			static char bufpass[64] = "password123";
			ImGui::InputText("password", bufpass, 64, ImGuiInputTextFlags_Password | ImGuiInputTextFlags_CharsNoBlank);
			ImGui::SameLine(); ShowHelpMarker("Display all characters as '*'.\nDisable clipboard cut and copy.\nDisable logging.\n");
			ImGui::InputText("password (clear)", bufpass, 64, ImGuiInputTextFlags_CharsNoBlank);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Multi-line Text Input"))
		{
			static bool read_only = false;
			static char text[1024*16] =
				"/*\n"
				" The Pentium F00F bug, shorthand for F0 0F C7 C8,\n"
				" the hexadecimal encoding of one offending instruction,\n"
				" more formally, the invalid operand with locked CMPXCHG8B\n"
				" instruction bug, is a design flaw in the majority of\n"
				" Intel Pentium, Pentium MMX, and Pentium OverDrive\n"
				" processors (all in the P5 microarchitecture).\n"
				"*/\n\n"
				"label:\n"
				"\tlock cmpxchg8b eax\n";

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0,0));
			ImGui::Checkbox("Read-only", &read_only);
			ImGui::PopStyleVar();
			ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_AllowTabInput | (read_only ? ImGuiInputTextFlags_ReadOnly : 0));
			ImGui::TreePop();
		}


		if (ImGui::TreeNode("Plots widgets"))
		{
			static bool animate = true;
			ImGui::Checkbox("Animate", &animate);

			static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
			ImGui::PlotLines("Frame Times", arr, IM_ARRAYSIZE(arr));

			// Create a dummy array of contiguous float values to plot
			// Tip: If your float aren't contiguous but part of a structure, you can pass a pointer to your first float and the sizeof() of your structure in the Stride parameter.
			static float values[90] = { 0 };
			static int values_offset = 0;
			static float refresh_time = 0.0f;
			if (!animate || refresh_time == 0.0f)
				refresh_time = ImGui::GetTime();
			while (refresh_time < ImGui::GetTime()) // Create dummy data at fixed 60 hz rate for the demo
			{
				static float phase = 0.0f;
				values[values_offset] = cosf(phase);
				values_offset = (values_offset+1) % IM_ARRAYSIZE(values);
				phase += 0.10f*values_offset;
				refresh_time += 1.0f/60.0f;
			}
			ImGui::PlotLines("Lines", values, IM_ARRAYSIZE(values), values_offset, "avg 0.0", -1.0f, 1.0f, ImVec2(0,80));
			ImGui::PlotHistogram("Histogram", arr, IM_ARRAYSIZE(arr), 0, NULL, 0.0f, 1.0f, ImVec2(0,80));

			// Use functions to generate output
			// FIXME: This is rather awkward because current plot API only pass in indices. We probably want an API passing floats and user provide sample rate/count.
			struct Funcs
			{
				static float Sin(void*, int i) { return sinf(i * 0.1f); }
				static float Saw(void*, int i) { return (i & 1) ? 1.0f : -1.0f; }
			};
			static int func_type = 0, display_count = 70;
			ImGui::Separator();
			ImGui::PushItemWidth(100); ImGui::Combo("func", &func_type, "Sin\0Saw\0"); ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::SliderInt("Sample count", &display_count, 1, 400);
			float (*func)(void*, int) = (func_type == 0) ? Funcs::Sin : Funcs::Saw;
			ImGui::PlotLines("Lines", func, NULL, display_count, 0, NULL, -1.0f, 1.0f, ImVec2(0,80));
			ImGui::PlotHistogram("Histogram", func, NULL, display_count, 0, NULL, -1.0f, 1.0f, ImVec2(0,80));
			ImGui::Separator();

			// Animate a simple progress bar
			static float progress = 0.0f, progress_dir = 1.0f;
			if (animate)
			{
				progress += progress_dir * 0.4f * ImGui::GetIO().DeltaTime;
				if (progress >= +1.1f) { progress = +1.1f; progress_dir *= -1.0f; }
				if (progress <= -0.1f) { progress = -0.1f; progress_dir *= -1.0f; }
			}

			// Typically we would use ImVec2(-1.0f,0.0f) to use all available width, or ImVec2(width,0.0f) for a specified width. ImVec2(0.0f,0.0f) uses ItemWidth.
			ImGui::ProgressBar(progress, ImVec2(0.0f,0.0f));
			ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
			ImGui::Text("Progress Bar");

			float progress_saturated = (progress < 0.0f) ? 0.0f : (progress > 1.0f) ? 1.0f : progress;
			char buf[32];
			sprintf_s(buf, "%d/%d", (int)(progress_saturated*1753), 1753);
			ImGui::ProgressBar(progress, ImVec2(0.f,0.f), buf);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Color/Picker Widgets"))
		{
			static ImVec4 color = ImColor(114, 144, 154, 200);

			static bool hdr = false;
			static bool alpha_preview = true;
			static bool alpha_half_preview = false;
			static bool options_menu = true;
			ImGui::Checkbox("With HDR", &hdr); ImGui::SameLine(); ShowHelpMarker("Currently all this does is to lift the 0..1 limits on dragging widgets.");
			ImGui::Checkbox("With Alpha Preview", &alpha_preview);
			ImGui::Checkbox("With Half Alpha Preview", &alpha_half_preview);
			ImGui::Checkbox("With Options Menu", &options_menu); ImGui::SameLine(); ShowHelpMarker("Right-click on the individual color widget to show options.");
			int misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

			ImGui::Text("Color widget:");
			ImGui::SameLine(); ShowHelpMarker("Click on the colored square to open a color picker.\nCTRL+click on individual component to input value.\n");
			ImGui::ColorEdit3("MyColor##1", (float*)&color, misc_flags);

			ImGui::Text("Color widget HSV with Alpha:");
			ImGui::ColorEdit4("MyColor##2", (float*)&color, ImGuiColorEditFlags_HSV | misc_flags);

			ImGui::Text("Color widget with Float Display:");
			ImGui::ColorEdit4("MyColor##2f", (float*)&color, ImGuiColorEditFlags_Float | misc_flags);

			ImGui::Text("Color button with Picker:");
			ImGui::SameLine(); ShowHelpMarker("With the ImGuiColorEditFlags_NoInputs flag you can hide all the slider/text inputs.\nWith the ImGuiColorEditFlags_NoLabel flag you can pass a non-empty label which will only be used for the tooltip and picker popup.");
			ImGui::ColorEdit4("MyColor##3", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

			ImGui::Text("Color button with Custom Picker Popup:");
			static bool saved_palette_inited = false;
			static ImVec4 saved_palette[32];
			static ImVec4 backup_color;
			if (!saved_palette_inited)
				for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
					ImGui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f, saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
			bool open_popup = ImGui::ColorButton("MyColor##3b", color, misc_flags);
			ImGui::SameLine();
			open_popup |= ImGui::Button("Palette");
			if (open_popup)
			{
				ImGui::OpenPopup("mypicker");
				backup_color = color;
			}
			if (ImGui::BeginPopup("mypicker"))
			{
				// FIXME: Adding a drag and drop example here would be perfect!
				ImGui::Text("MY CUSTOM COLOR PICKER WITH AN AMAZING PALETTE!");
				ImGui::Separator();
				ImGui::ColorPicker4("##picker", (float*)&color, misc_flags | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
				ImGui::SameLine();
				ImGui::BeginGroup();
				ImGui::Text("Current");
				ImGui::ColorButton("##current", color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60,40));
				ImGui::Text("Previous");
				if (ImGui::ColorButton("##previous", backup_color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60,40)))
					color = backup_color;
				ImGui::Separator();
				ImGui::Text("Palette");
				for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
				{
					ImGui::PushID(n);
					if ((n % 8) != 0)
						ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);
					if (ImGui::ColorButton("##palette", saved_palette[n], ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip, ImVec2(20,20)))
						color = ImVec4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, color.w); // Preserve alpha!
					ImGui::PopID();
				}
				ImGui::EndGroup();
				ImGui::EndPopup();
			}

			ImGui::Text("Color button only:");
			ImGui::ColorButton("MyColor##3c", *(ImVec4*)&color, misc_flags, ImVec2(80,80));

			ImGui::Text("Color picker:");
			static bool alpha = true;
			static bool alpha_bar = true;
			static bool side_preview = true;
			static bool ref_color = false;
			static ImVec4 ref_color_v(1.0f,0.0f,1.0f,0.5f);
			static int inputs_mode = 2;
			static int picker_mode = 0;
			ImGui::Checkbox("With Alpha", &alpha);
			ImGui::Checkbox("With Alpha Bar", &alpha_bar);
			ImGui::Checkbox("With Side Preview", &side_preview);
			if (side_preview)
			{
				ImGui::SameLine();
				ImGui::Checkbox("With Ref Color", &ref_color);
				if (ref_color)
				{
					ImGui::SameLine();
					ImGui::ColorEdit4("##RefColor", &ref_color_v.x, ImGuiColorEditFlags_NoInputs | misc_flags);
				}
			}
			ImGui::Combo("Inputs Mode", &inputs_mode, "All Inputs\0No Inputs\0RGB Input\0HSV Input\0HEX Input\0");
			ImGui::Combo("Picker Mode", &picker_mode, "Auto/Current\0Hue bar + SV rect\0Hue wheel + SV triangle\0");
			ImGui::SameLine(); ShowHelpMarker("User can right-click the picker to change mode.");
			ImGuiColorEditFlags flags = misc_flags;
			if (!alpha) flags |= ImGuiColorEditFlags_NoAlpha; // This is by default if you call ColorPicker3() instead of ColorPicker4()
			if (alpha_bar) flags |= ImGuiColorEditFlags_AlphaBar;
			if (!side_preview) flags |= ImGuiColorEditFlags_NoSidePreview;
			if (picker_mode == 1) flags |= ImGuiColorEditFlags_PickerHueBar;
			if (picker_mode == 2) flags |= ImGuiColorEditFlags_PickerHueWheel;
			if (inputs_mode == 1) flags |= ImGuiColorEditFlags_NoInputs;
			if (inputs_mode == 2) flags |= ImGuiColorEditFlags_RGB;
			if (inputs_mode == 3) flags |= ImGuiColorEditFlags_HSV;
			if (inputs_mode == 4) flags |= ImGuiColorEditFlags_HEX;
			ImGui::ColorPicker4("MyColor##4", (float*)&color, flags, ref_color ? &ref_color_v.x : NULL);

			ImGui::Text("Programmatically set defaults/options:");
			ImGui::SameLine(); ShowHelpMarker("SetColorEditOptions() is designed to allow you to set boot-time default.\nWe don't have Push/Pop functions because you can force options on a per-widget basis if needed, and the user can change non-forced ones with the options menu.\nWe don't have a getter to avoid encouraging you to persistently save values that aren't forward-compatible.");
			if (ImGui::Button("Uint8 + HSV"))
				ImGui::SetColorEditOptions(ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_HSV);
			ImGui::SameLine();
			if (ImGui::Button("Float + HDR"))
				ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float | ImGuiColorEditFlags_RGB);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Range Widgets"))
		{
			static float begin = 10, end = 90;
			static int begin_i = 100, end_i = 1000;
			ImGui::DragFloatRange2("range", &begin, &end, 0.25f, 0.0f, 100.0f, "Min: %.1f %%", "Max: %.1f %%");
			ImGui::DragIntRange2("range int (no bounds)", &begin_i, &end_i, 5, 0, 0, "Min: %.0f units", "Max: %.0f units");
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Multi-component Widgets"))
		{
			static float vec4f[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
			static int vec4i[4] = { 1, 5, 100, 255 };

			ImGui::InputFloat2("input float2", vec4f);
			ImGui::DragFloat2("drag float2", vec4f, 0.01f, 0.0f, 1.0f);
			ImGui::SliderFloat2("slider float2", vec4f, 0.0f, 1.0f);
			ImGui::DragInt2("drag int2", vec4i, 1, 0, 255);
			ImGui::InputInt2("input int2", vec4i);
			ImGui::SliderInt2("slider int2", vec4i, 0, 255);
			ImGui::Spacing();

			ImGui::InputFloat3("input float3", vec4f);
			ImGui::DragFloat3("drag float3", vec4f, 0.01f, 0.0f, 1.0f);
			ImGui::SliderFloat3("slider float3", vec4f, 0.0f, 1.0f);
			ImGui::DragInt3("drag int3", vec4i, 1, 0, 255);
			ImGui::InputInt3("input int3", vec4i);
			ImGui::SliderInt3("slider int3", vec4i, 0, 255);
			ImGui::Spacing();

			ImGui::InputFloat4("input float4", vec4f);
			ImGui::DragFloat4("drag float4", vec4f, 0.01f, 0.0f, 1.0f);
			ImGui::SliderFloat4("slider float4", vec4f, 0.0f, 1.0f);
			ImGui::InputInt4("input int4", vec4i);
			ImGui::DragInt4("drag int4", vec4i, 1, 0, 255);
			ImGui::SliderInt4("slider int4", vec4i, 0, 255);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Vertical Sliders"))
		{
			const float spacing = 4;
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, spacing));

			static int int_value = 0;
			ImGui::VSliderInt("##int", ImVec2(18,160), &int_value, 0, 5);
			ImGui::SameLine();

			static float values[7] = { 0.0f, 0.60f, 0.35f, 0.9f, 0.70f, 0.20f, 0.0f };
			ImGui::PushID("set1");
			for (int i = 0; i < 7; i++)
			{
				if (i > 0) ImGui::SameLine();
				ImGui::PushID(i);
				ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(i/7.0f, 0.5f, 0.5f));
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(i/7.0f, 0.6f, 0.5f));
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(i/7.0f, 0.7f, 0.5f));
				ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(i/7.0f, 0.9f, 0.9f));
				ImGui::VSliderFloat("##v", ImVec2(18,160), &values[i], 0.0f, 1.0f, "");
				if (ImGui::IsItemActive() || ImGui::IsItemHovered())
					ImGui::SetTooltip("%.3f", values[i]);
				ImGui::PopStyleColor(4);
				ImGui::PopID();
			}
			ImGui::PopID();

			ImGui::SameLine();
			ImGui::PushID("set2");
			static float values2[4] = { 0.20f, 0.80f, 0.40f, 0.25f };
			const int rows = 3;
			const ImVec2 small_slider_size(18, (160.0f-(rows-1)*spacing)/rows);
			for (int nx = 0; nx < 4; nx++)
			{
				if (nx > 0) ImGui::SameLine();
				ImGui::BeginGroup();
				for (int ny = 0; ny < rows; ny++)
				{
					ImGui::PushID(nx*rows+ny);
					ImGui::VSliderFloat("##v", small_slider_size, &values2[nx], 0.0f, 1.0f, "");
					if (ImGui::IsItemActive() || ImGui::IsItemHovered())
						ImGui::SetTooltip("%.3f", values2[nx]);
					ImGui::PopID();
				}
				ImGui::EndGroup();
			}
			ImGui::PopID();

			ImGui::SameLine();
			ImGui::PushID("set3");
			for (int i = 0; i < 4; i++)
			{
				if (i > 0) ImGui::SameLine();
				ImGui::PushID(i);
				ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 40);
				ImGui::VSliderFloat("##v", ImVec2(40,160), &values[i], 0.0f, 1.0f, "%.2f\nsec");
				ImGui::PopStyleVar();
				ImGui::PopID();
			}
			ImGui::PopID();
			ImGui::PopStyleVar();
			ImGui::TreePop();
		}
	}

	if (ImGui::CollapsingHeader("Layout"))
	{
		if (ImGui::TreeNode("Child regions"))
		{
			ImGui::Text("Without border");
			static int line = 50;
			bool goto_line = ImGui::Button("Goto");
			ImGui::SameLine();
			ImGui::PushItemWidth(100);
			goto_line |= ImGui::InputInt("##Line", &line, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::PopItemWidth();
			ImGui::BeginChild("Sub1", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f,300), false, ImGuiWindowFlags_HorizontalScrollbar);
			for (int i = 0; i < 100; i++)
			{
				ImGui::Text("%04d: scrollable region", i);
				if (goto_line && line == i)
					ImGui::SetScrollHere();
			}
			if (goto_line && line >= 100)
				ImGui::SetScrollHere();
			ImGui::EndChild();

			ImGui::SameLine();

			ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
			ImGui::BeginChild("Sub2", ImVec2(0,300), true);
			ImGui::Text("With border");
			ImGui::Columns(2);
			for (int i = 0; i < 100; i++)
			{
				if (i == 50)
					ImGui::NextColumn();
				char buf[32];
				sprintf_s(buf, "%08x", i*5731);
				ImGui::Button(buf, ImVec2(-1.0f, 0.0f));
			}
			ImGui::EndChild();
			ImGui::PopStyleVar();

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Widgets Width"))
		{
			static float f = 0.0f;
			ImGui::Text("PushItemWidth(100)");
			ImGui::SameLine(); ShowHelpMarker("Fixed width.");
			ImGui::PushItemWidth(100);
			ImGui::DragFloat("float##1", &f);
			ImGui::PopItemWidth();

			ImGui::Text("PushItemWidth(GetWindowWidth() * 0.5f)");
			ImGui::SameLine(); ShowHelpMarker("Half of window width.");
			ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.5f);
			ImGui::DragFloat("float##2", &f);
			ImGui::PopItemWidth();

			ImGui::Text("PushItemWidth(GetContentRegionAvailWidth() * 0.5f)");
			ImGui::SameLine(); ShowHelpMarker("Half of available width.\n(~ right-cursor_pos)\n(works within a column set)");
			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() * 0.5f);
			ImGui::DragFloat("float##3", &f);
			ImGui::PopItemWidth();

			ImGui::Text("PushItemWidth(-100)");
			ImGui::SameLine(); ShowHelpMarker("Align to right edge minus 100");
			ImGui::PushItemWidth(-100);
			ImGui::DragFloat("float##4", &f);
			ImGui::PopItemWidth();

			ImGui::Text("PushItemWidth(-1)");
			ImGui::SameLine(); ShowHelpMarker("Align to right edge");
			ImGui::PushItemWidth(-1);
			ImGui::DragFloat("float##5", &f);
			ImGui::PopItemWidth();

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Basic Horizontal Layout"))
		{
			ImGui::TextWrapped("(Use ImGui::SameLine() to keep adding items to the right of the preceding item)");

			// Text
			ImGui::Text("Two items: Hello"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1,1,0,1), "Sailor");

			// Adjust spacing
			ImGui::Text("More spacing: Hello"); ImGui::SameLine(0, 20);
			ImGui::TextColored(ImVec4(1,1,0,1), "Sailor");

			// Button
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Normal buttons"); ImGui::SameLine();
			ImGui::Button("Banana"); ImGui::SameLine();
			ImGui::Button("Apple"); ImGui::SameLine();
			ImGui::Button("Corniflower");

			// Button
			ImGui::Text("Small buttons"); ImGui::SameLine();
			ImGui::SmallButton("Like this one"); ImGui::SameLine();
			ImGui::Text("can fit within a text block.");

			// Aligned to arbitrary position. Easy/cheap column.
			ImGui::Text("Aligned");
			ImGui::SameLine(150); ImGui::Text("x=150");
			ImGui::SameLine(300); ImGui::Text("x=300");
			ImGui::Text("Aligned");
			ImGui::SameLine(150); ImGui::SmallButton("x=150");
			ImGui::SameLine(300); ImGui::SmallButton("x=300");

			// Checkbox
			static bool c1=false,c2=false,c3=false,c4=false;
			ImGui::Checkbox("My", &c1); ImGui::SameLine();
			ImGui::Checkbox("Tailor", &c2); ImGui::SameLine();
			ImGui::Checkbox("Is", &c3); ImGui::SameLine();
			ImGui::Checkbox("Rich", &c4);

			// Various
			static float f0=1.0f, f1=2.0f, f2=3.0f;
			ImGui::PushItemWidth(80);
			const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD" };
			static int item = -1;
			ImGui::Combo("Combo", &item, items, IM_ARRAYSIZE(items)); ImGui::SameLine();
			ImGui::SliderFloat("X", &f0, 0.0f,5.0f); ImGui::SameLine();
			ImGui::SliderFloat("Y", &f1, 0.0f,5.0f); ImGui::SameLine();
			ImGui::SliderFloat("Z", &f2, 0.0f,5.0f);
			ImGui::PopItemWidth();

			ImGui::PushItemWidth(80);
			ImGui::Text("Lists:");
			static int selection[4] = { 0, 1, 2, 3 };
			for (int i = 0; i < 4; i++)
			{
				if (i > 0) ImGui::SameLine();
				ImGui::PushID(i);
				ImGui::ListBox("", &selection[i], items, IM_ARRAYSIZE(items));
				ImGui::PopID();
				//if (ImGui::IsItemHovered()) ImGui::SetTooltip("ListBox %d hovered", i);
			}
			ImGui::PopItemWidth();

			// Dummy
			ImVec2 sz(30,30);
			ImGui::Button("A", sz); ImGui::SameLine();
			ImGui::Dummy(sz); ImGui::SameLine();
			ImGui::Button("B", sz);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Groups"))
		{
			ImGui::TextWrapped("(Using ImGui::BeginGroup()/EndGroup() to layout items. BeginGroup() basically locks the horizontal position. EndGroup() bundles the whole group so that you can use functions such as IsItemHovered() on it.)");
			ImGui::BeginGroup();
			{
				ImGui::BeginGroup();
				ImGui::Button("AAA");
				ImGui::SameLine();
				ImGui::Button("BBB");
				ImGui::SameLine();
				ImGui::BeginGroup();
				ImGui::Button("CCC");
				ImGui::Button("DDD");
				ImGui::EndGroup();
				ImGui::SameLine();
				ImGui::Button("EEE");
				ImGui::EndGroup();
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("First group hovered");
			}
			// Capture the group size and create widgets using the same size
			ImVec2 size = ImGui::GetItemRectSize();
			const float values[5] = { 0.5f, 0.20f, 0.80f, 0.60f, 0.25f };
			ImGui::PlotHistogram("##values", values, IM_ARRAYSIZE(values), 0, NULL, 0.0f, 1.0f, size);

			ImGui::Button("ACTION", ImVec2((size.x - ImGui::GetStyle().ItemSpacing.x)*0.5f,size.y));
			ImGui::SameLine();
			ImGui::Button("REACTION", ImVec2((size.x - ImGui::GetStyle().ItemSpacing.x)*0.5f,size.y));
			ImGui::EndGroup();
			ImGui::SameLine();

			ImGui::Button("LEVERAGE\nBUZZWORD", size);
			ImGui::SameLine();

			ImGui::ListBoxHeader("List", size);
			ImGui::Selectable("Selected", true);
			ImGui::Selectable("Not Selected", false);
			ImGui::ListBoxFooter();

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Text Baseline Alignment"))
		{
			ImGui::TextWrapped("(This is testing the vertical alignment that occurs on text to keep it at the same baseline as widgets. Lines only composed of text or \"small\" widgets fit in less vertical spaces than lines with normal widgets)");

			ImGui::Text("One\nTwo\nThree"); ImGui::SameLine();
			ImGui::Text("Hello\nWorld"); ImGui::SameLine();
			ImGui::Text("Banana");

			ImGui::Text("Banana"); ImGui::SameLine();
			ImGui::Text("Hello\nWorld"); ImGui::SameLine();
			ImGui::Text("One\nTwo\nThree");

			ImGui::Button("HOP##1"); ImGui::SameLine();
			ImGui::Text("Banana"); ImGui::SameLine();
			ImGui::Text("Hello\nWorld"); ImGui::SameLine();
			ImGui::Text("Banana");

			ImGui::Button("HOP##2"); ImGui::SameLine();
			ImGui::Text("Hello\nWorld"); ImGui::SameLine();
			ImGui::Text("Banana");

			ImGui::Button("TEST##1"); ImGui::SameLine();
			ImGui::Text("TEST"); ImGui::SameLine();
			ImGui::SmallButton("TEST##2");

			ImGui::AlignTextToFramePadding(); // If your line starts with text, call this to align it to upcoming widgets.
			ImGui::Text("Text aligned to Widget"); ImGui::SameLine();
			ImGui::Button("Widget##1"); ImGui::SameLine();
			ImGui::Text("Widget"); ImGui::SameLine();
			ImGui::SmallButton("Widget##2"); ImGui::SameLine();
			ImGui::Button("Widget##3");

			// Tree
			const float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
			ImGui::Button("Button##1");
			ImGui::SameLine(0.0f, spacing);
			if (ImGui::TreeNode("Node##1")) { for (int i = 0; i < 6; i++) ImGui::BulletText("Item %d..", i); ImGui::TreePop(); }    // Dummy tree data

			ImGui::AlignTextToFramePadding();         // Vertically align text node a bit lower so it'll be vertically centered with upcoming widget. Otherwise you can use SmallButton (smaller fit).
			bool node_open = ImGui::TreeNode("Node##2");  // Common mistake to avoid: if we want to SameLine after TreeNode we need to do it before we add child content.
			ImGui::SameLine(0.0f, spacing); ImGui::Button("Button##2");
			if (node_open) { for (int i = 0; i < 6; i++) ImGui::BulletText("Item %d..", i); ImGui::TreePop(); }   // Dummy tree data

																												  // Bullet
			ImGui::Button("Button##3");
			ImGui::SameLine(0.0f, spacing);
			ImGui::BulletText("Bullet text");

			ImGui::AlignTextToFramePadding();
			ImGui::BulletText("Node");
			ImGui::SameLine(0.0f, spacing); ImGui::Button("Button##4");

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Scrolling"))
		{
			ImGui::TextWrapped("(Use SetScrollHere() or SetScrollFromPosY() to scroll to a given position.)");
			static bool track = true;
			static int track_line = 50, scroll_to_px = 200;
			ImGui::Checkbox("Track", &track);
			ImGui::PushItemWidth(100);
			ImGui::SameLine(130); track |= ImGui::DragInt("##line", &track_line, 0.25f, 0, 99, "Line = %.0f");
			bool scroll_to = ImGui::Button("Scroll To Pos");
			ImGui::SameLine(130); scroll_to |= ImGui::DragInt("##pos_y", &scroll_to_px, 1.00f, 0, 9999, "Y = %.0f px");
			ImGui::PopItemWidth();
			if (scroll_to) track = false;

			for (int i = 0; i < 5; i++)
			{
				if (i > 0) ImGui::SameLine();
				ImGui::BeginGroup();
				ImGui::Text("%s", i == 0 ? "Top" : i == 1 ? "25%" : i == 2 ? "Center" : i == 3 ? "75%" : "Bottom");
				ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)i), ImVec2(ImGui::GetWindowWidth() * 0.17f, 200.0f), true);
				if (scroll_to)
					ImGui::SetScrollFromPosY(ImGui::GetCursorStartPos().y + scroll_to_px, i * 0.25f);
				for (int line = 0; line < 100; line++)
				{
					if (track && line == track_line)
					{
						ImGui::TextColored(ImColor(255,255,0), "Line %d", line);
						ImGui::SetScrollHere(i * 0.25f); // 0.0f:top, 0.5f:center, 1.0f:bottom
					}
					else
					{
						ImGui::Text("Line %d", line);
					}
				}
				float scroll_y = ImGui::GetScrollY(), scroll_max_y = ImGui::GetScrollMaxY();
				ImGui::EndChild();
				ImGui::Text("%.0f/%0.f", scroll_y, scroll_max_y);
				ImGui::EndGroup();
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Horizontal Scrolling"))
		{
			ImGui::Bullet(); ImGui::TextWrapped("Horizontal scrolling for a window has to be enabled explicitly via the ImGuiWindowFlags_HorizontalScrollbar flag.");
			ImGui::Bullet(); ImGui::TextWrapped("You may want to explicitly specify content width by calling SetNextWindowContentWidth() before Begin().");
			static int lines = 7;
			ImGui::SliderInt("Lines", &lines, 1, 15);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));
			ImGui::BeginChild("scrolling", ImVec2(0, ImGui::GetItemsLineHeightWithSpacing()*7 + 30), true, ImGuiWindowFlags_HorizontalScrollbar);
			for (int line = 0; line < lines; line++)
			{
				// Display random stuff (for the sake of this trivial demo we are using basic Button+SameLine. If you want to create your own time line for a real application you may be better off 
				// manipulating the cursor position yourself, aka using SetCursorPos/SetCursorScreenPos to position the widgets yourself. You may also want to use the lower-level ImDrawList API)
				int num_buttons = 10 + ((line & 1) ? line * 9 : line * 3);
				for (int n = 0; n < num_buttons; n++)
				{
					if (n > 0) ImGui::SameLine();
					ImGui::PushID(n + line * 1000);
					char num_buf[16];
					const char* label = (!(n%15)) ? "FizzBuzz" : (!(n%3)) ? "Fizz" : (!(n%5)) ? "Buzz" : (sprintf_s(num_buf, "%d", n), num_buf);
					float hue = n*0.05f;
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue, 0.6f, 0.6f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue, 0.7f, 0.7f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue, 0.8f, 0.8f));
					ImGui::Button(label, ImVec2(40.0f + sinf((float)(line + n)) * 20.0f, 0.0f));
					ImGui::PopStyleColor(3);
					ImGui::PopID();
				}
			}
			float scroll_x = ImGui::GetScrollX(), scroll_max_x = ImGui::GetScrollMaxX();
			ImGui::EndChild();
			ImGui::PopStyleVar(2);
			float scroll_x_delta = 0.0f;
			ImGui::SmallButton("<<"); if (ImGui::IsItemActive()) scroll_x_delta = -ImGui::GetIO().DeltaTime * 1000.0f; ImGui::SameLine(); 
			ImGui::Text("Scroll from code"); ImGui::SameLine();
			ImGui::SmallButton(">>"); if (ImGui::IsItemActive()) scroll_x_delta = +ImGui::GetIO().DeltaTime * 1000.0f; ImGui::SameLine(); 
			ImGui::Text("%.0f/%.0f", scroll_x, scroll_max_x);
			if (scroll_x_delta != 0.0f)
			{
				ImGui::BeginChild("scrolling"); // Demonstrate a trick: you can use Begin to set yourself in the context of another window (here we are already out of your child window)
				ImGui::SetScrollX(ImGui::GetScrollX() + scroll_x_delta);
				ImGui::End();
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Clipping"))
		{
			static ImVec2 size(100, 100), offset(50, 20);
			ImGui::TextWrapped("On a per-widget basis we are occasionally clipping text CPU-side if it won't fit in its frame. Otherwise we are doing coarser clipping + passing a scissor rectangle to the renderer. The system is designed to try minimizing both execution and CPU/GPU rendering cost.");
			ImGui::DragFloat2("size", (float*)&size, 0.5f, 0.0f, 200.0f, "%.0f");
			ImGui::TextWrapped("(Click and drag)");
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec4 clip_rect(pos.x, pos.y, pos.x+size.x, pos.y+size.y);
			ImGui::InvisibleButton("##dummy", size);
			if (ImGui::IsItemActive() && ImGui::IsMouseDragging()) { offset.x += ImGui::GetIO().MouseDelta.x; offset.y += ImGui::GetIO().MouseDelta.y; }
			ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2(pos.x+size.x,pos.y+size.y), ImColor(90,90,120,255));
			ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize()*2.0f, ImVec2(pos.x+offset.x,pos.y+offset.y), ImColor(255,255,255,255), "Line 1 hello\nLine 2 clip me!", NULL, 0.0f, &clip_rect);
			ImGui::TreePop();
		}
	}

	if (ImGui::CollapsingHeader("Popups & Modal windows"))
	{
		if (ImGui::TreeNode("Popups"))
		{
			ImGui::TextWrapped("When a popup is active, it inhibits interacting with windows that are behind the popup. Clicking outside the popup closes it.");

			static int selected_fish = -1;
			const char* names[] = { "Bream", "Haddock", "Mackerel", "Pollock", "Tilefish" };
			static bool toggles[] = { true, false, false, false, false };

			// Simple selection popup
			// (If you want to show the current selection inside the Button itself, you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
			if (ImGui::Button("Select.."))
				ImGui::OpenPopup("select");
			ImGui::SameLine();
			ImGui::Text(selected_fish == -1 ? "<None>" : names[selected_fish]);
			if (ImGui::BeginPopup("select"))
			{
				ImGui::Text("Aquarium");
				ImGui::Separator();
				for (int i = 0; i < IM_ARRAYSIZE(names); i++)
					if (ImGui::Selectable(names[i]))
						selected_fish = i;
				ImGui::EndPopup();
			}

			// Showing a menu with toggles
			if (ImGui::Button("Toggle.."))
				ImGui::OpenPopup("toggle");
			if (ImGui::BeginPopup("toggle"))
			{
				for (int i = 0; i < IM_ARRAYSIZE(names); i++)
					ImGui::MenuItem(names[i], "", &toggles[i]);
				if (ImGui::BeginMenu("Sub-menu"))
				{
					ImGui::MenuItem("Click me");
					ImGui::EndMenu();
				}

				ImGui::Separator();
				ImGui::Text("Tooltip here");
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("I am a tooltip over a popup");

				if (ImGui::Button("Stacked Popup"))
					ImGui::OpenPopup("another popup");
				if (ImGui::BeginPopup("another popup"))
				{
					for (int i = 0; i < IM_ARRAYSIZE(names); i++)
						ImGui::MenuItem(names[i], "", &toggles[i]);
					if (ImGui::BeginMenu("Sub-menu"))
					{
						ImGui::MenuItem("Click me");
						ImGui::EndMenu();
					}
					ImGui::EndPopup();
				}
				ImGui::EndPopup();
			}

			//if (ImGui::Button("Popup Menu.."))
			//	ImGui::OpenPopup("FilePopup");
			//if (ImGui::BeginPopup("FilePopup"))
			//{
			//	ShowExampleMenuFile();
			//	ImGui::EndPopup();
			//}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Context menus"))
		{
			// BeginPopupContextItem() is a helper to provide common/simple popup behavior of essentially doing:
			//    if (IsItemHovered() && IsMouseClicked(0))
			//       OpenPopup(id);
			//    return BeginPopup(id);
			// For more advanced uses you may want to replicate and cuztomize this code. This the comments inside BeginPopupContextItem() implementation.
			static float value = 0.5f;
			ImGui::Text("Value = %.3f (<-- right-click here)", value);
			if (ImGui::BeginPopupContextItem("item context menu"))
			{
				if (ImGui::Selectable("Set to zero")) value = 0.0f;
				if (ImGui::Selectable("Set to PI")) value = 3.1415f;
				ImGui::PushItemWidth(-1);
				ImGui::DragFloat("##Value", &value, 0.1f, 0.0f, 0.0f);
				ImGui::PopItemWidth();
				ImGui::EndPopup();
			}

			static char name[32] = "Label1";
			char buf[64]; sprintf_s(buf, "Button: %s###Button", name); // ### operator override ID ignoring the preceding label
			ImGui::Button(buf);
			if (ImGui::BeginPopupContextItem()) // When used after an item that has an ID (here the Button), we can skip providing an ID to BeginPopupContextItem().
			{
				ImGui::Text("Edit name:");
				ImGui::InputText("##edit", name, IM_ARRAYSIZE(name));
				if (ImGui::Button("Close"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
			ImGui::SameLine(); ImGui::Text("(<-- right-click here)");

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Modals"))
		{
			ImGui::TextWrapped("Modal windows are like popups but the user cannot close them by clicking outside the window.");

			if (ImGui::Button("Delete.."))
				ImGui::OpenPopup("Delete?");
			if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");
				ImGui::Separator();

				//static int dummy_i = 0;
				//ImGui::Combo("Combo", &dummy_i, "Delete\0Delete harder\0");

				static bool dont_ask_me_next_time = false;
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0,0));
				ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
				ImGui::PopStyleVar();

				if (ImGui::Button("OK", ImVec2(120,0))) { ImGui::CloseCurrentPopup(); }
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120,0))) { ImGui::CloseCurrentPopup(); }
				ImGui::EndPopup();
			}

			if (ImGui::Button("Stacked modals.."))
				ImGui::OpenPopup("Stacked 1");
			if (ImGui::BeginPopupModal("Stacked 1"))
			{
				ImGui::Text("Hello from Stacked The First\nUsing style.Colors[ImGuiCol_ModalWindowDarkening] for darkening.");
				static int item = 1;
				ImGui::Combo("Combo", &item, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");
				static float color[4] = { 0.4f,0.7f,0.0f,0.5f };
				ImGui::ColorEdit4("color", color);  // This is to test behavior of stacked regular popups over a modal

				if (ImGui::Button("Add another modal.."))
					ImGui::OpenPopup("Stacked 2");
				if (ImGui::BeginPopupModal("Stacked 2"))
				{
					ImGui::Text("Hello from Stacked The Second");
					if (ImGui::Button("Close"))
						ImGui::CloseCurrentPopup();
					ImGui::EndPopup();
				}

				if (ImGui::Button("Close"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Menus inside a regular window"))
		{
			ImGui::TextWrapped("Below we are testing adding menu items to a regular window. It's rather unusual but should work!");
			ImGui::Separator();
			// NB: As a quirk in this very specific example, we want to differentiate the parent of this menu from the parent of the various popup menus above.
			// To do so we are encloding the items in a PushID()/PopID() block to make them two different menusets. If we don't, opening any popup above and hovering our menu here
			// would open it. This is because once a menu is active, we allow to switch to a sibling menu by just hovering on it, which is the desired behavior for regular menus.
			ImGui::PushID("foo");
			ImGui::MenuItem("Menu item", "CTRL+M");
			if (ImGui::BeginMenu("Menu inside a regular window"))
			{
				//ShowExampleMenuFile();
				ImGui::EndMenu();
			}
			ImGui::PopID();
			ImGui::Separator();
			ImGui::TreePop();
		}
	}

	if (ImGui::CollapsingHeader("Columns"))
	{
		ImGui::PushID("Columns");

		// Basic columns
		if (ImGui::TreeNode("Basic"))
		{
			ImGui::Text("Without border:");
			ImGui::Columns(3, "mycolumns3", false);  // 3-ways, no border
			ImGui::Separator();
			for (int n = 0; n < 14; n++)
			{
				char label[32];
				sprintf_s(label, "Item %d", n);
				if (ImGui::Selectable(label)) {}
				//if (ImGui::Button(label, ImVec2(-1,0))) {}
				ImGui::NextColumn();
			}
			ImGui::Columns(1);
			ImGui::Separator();

			ImGui::Text("With border:");
			ImGui::Columns(4, "mycolumns"); // 4-ways, with border
			ImGui::Separator();
			ImGui::Text("ID"); ImGui::NextColumn();
			ImGui::Text("Name"); ImGui::NextColumn();
			ImGui::Text("Path"); ImGui::NextColumn();
			ImGui::Text("Hovered"); ImGui::NextColumn();
			ImGui::Separator();
			const char* names[3] = { "One", "Two", "Three" };
			const char* paths[3] = { "/path/one", "/path/two", "/path/three" };
			static int selected = -1;
			for (int i = 0; i < 3; i++)
			{
				char label[32];
				sprintf_s(label, "%04d", i);
				if (ImGui::Selectable(label, selected == i, ImGuiSelectableFlags_SpanAllColumns))
					selected = i;
				bool hovered = ImGui::IsItemHovered();
				ImGui::NextColumn();
				ImGui::Text(names[i]); ImGui::NextColumn();
				ImGui::Text(paths[i]); ImGui::NextColumn();
				ImGui::Text("%d", hovered); ImGui::NextColumn();
			}
			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::TreePop();
		}

		// Create multiple items in a same cell before switching to next column
		if (ImGui::TreeNode("Mixed items"))
		{
			ImGui::Columns(3, "mixed");
			ImGui::Separator();

			ImGui::Text("Hello");
			ImGui::Button("Banana");
			ImGui::NextColumn();

			ImGui::Text("ImGui");
			ImGui::Button("Apple");
			static float foo = 1.0f;
			ImGui::InputFloat("red", &foo, 0.05f, 0, 3);
			ImGui::Text("An extra line here.");
			ImGui::NextColumn();

			ImGui::Text("Sailor");
			ImGui::Button("Corniflower");
			static float bar = 1.0f;
			ImGui::InputFloat("blue", &bar, 0.05f, 0, 3);
			ImGui::NextColumn();

			if (ImGui::CollapsingHeader("Category A")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
			if (ImGui::CollapsingHeader("Category B")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
			if (ImGui::CollapsingHeader("Category C")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::TreePop();
		}

		// Word wrapping
		if (ImGui::TreeNode("Word-wrapping"))
		{
			ImGui::Columns(2, "word-wrapping");
			ImGui::Separator();
			ImGui::TextWrapped("The quick brown fox jumps over the lazy dog.");
			ImGui::TextWrapped("Hello Left");
			ImGui::NextColumn();
			ImGui::TextWrapped("The quick brown fox jumps over the lazy dog.");
			ImGui::TextWrapped("Hello Right");
			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Borders"))
		{
			// NB: Future columns API should allow automatic horizontal borders.
			static bool h_borders = true;
			static bool v_borders = true;
			ImGui::Checkbox("horizontal", &h_borders);
			ImGui::SameLine();
			ImGui::Checkbox("vertical", &v_borders);
			ImGui::Columns(4, NULL, v_borders);
			for (int i = 0; i < 4*3; i++)
			{
				if (h_borders && ImGui::GetColumnIndex() == 0)
					ImGui::Separator();
				ImGui::Text("%c%c%c", 'a'+i, 'a'+i, 'a'+i);
				ImGui::Text("Width %.2f\nOffset %.2f", ImGui::GetColumnWidth(), ImGui::GetColumnOffset());
				ImGui::NextColumn();
			}
			ImGui::Columns(1);
			if (h_borders)
				ImGui::Separator();
			ImGui::TreePop();
		}

		// Scrolling columns
		/*
		if (ImGui::TreeNode("Vertical Scrolling"))
		{
		ImGui::BeginChild("##header", ImVec2(0, ImGui::GetTextLineHeightWithSpacing()+ImGui::GetStyle().ItemSpacing.y));
		ImGui::Columns(3);
		ImGui::Text("ID"); ImGui::NextColumn();
		ImGui::Text("Name"); ImGui::NextColumn();
		ImGui::Text("Path"); ImGui::NextColumn();
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::EndChild();
		ImGui::BeginChild("##scrollingregion", ImVec2(0, 60));
		ImGui::Columns(3);
		for (int i = 0; i < 10; i++)
		{
		ImGui::Text("%04d", i); ImGui::NextColumn();
		ImGui::Text("Foobar"); ImGui::NextColumn();
		ImGui::Text("/path/foobar/%04d/", i); ImGui::NextColumn();
		}
		ImGui::Columns(1);
		ImGui::EndChild();
		ImGui::TreePop();
		}
		*/

		if (ImGui::TreeNode("Horizontal Scrolling"))
		{
			ImGui::SetNextWindowContentWidth(1500);
			ImGui::BeginChild("##scrollingregion", ImVec2(0, 120), false, ImGuiWindowFlags_HorizontalScrollbar);
			ImGui::Columns(10);
			for (int i = 0; i < 20; i++)
				for (int j = 0; j < 10; j++)
				{
					ImGui::Text("Line %d Column %d...", i, j);
					ImGui::NextColumn();
				}
			ImGui::Columns(1);
			ImGui::EndChild();
			ImGui::TreePop();
		}

		bool node_open = ImGui::TreeNode("Tree within single cell");
		ImGui::SameLine(); ShowHelpMarker("NB: Tree node must be poped before ending the cell. There's no storage of state per-cell.");
		if (node_open)
		{
			ImGui::Columns(2, "tree items");
			ImGui::Separator();
			if (ImGui::TreeNode("Hello")) { ImGui::BulletText("Sailor"); ImGui::TreePop(); } ImGui::NextColumn();
			if (ImGui::TreeNode("Bonjour")) { ImGui::BulletText("Marin"); ImGui::TreePop(); } ImGui::NextColumn();
			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::TreePop();
		}
		ImGui::PopID();
	}

	if (ImGui::CollapsingHeader("Filtering"))
	{
		static ImGuiTextFilter filter;
		ImGui::Text("Filter usage:\n"
					"  \"\"         display all lines\n"
					"  \"xxx\"      display lines containing \"xxx\"\n"
					"  \"xxx,yyy\"  display lines containing \"xxx\" or \"yyy\"\n"
					"  \"-xxx\"     hide lines containing \"xxx\"");
		filter.Draw();
		const char* lines[] = { "aaa1.c", "bbb1.c", "ccc1.c", "aaa2.cpp", "bbb2.cpp", "ccc2.cpp", "abc.h", "hello, world" };
		for (int i = 0; i < IM_ARRAYSIZE(lines); i++)
			if (filter.PassFilter(lines[i]))
				ImGui::BulletText("%s", lines[i]);
	}

	if (ImGui::CollapsingHeader("Inputs & Focus"))
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Checkbox("io.MouseDrawCursor", &io.MouseDrawCursor);
		ImGui::SameLine(); ShowHelpMarker("Request ImGui to render a mouse cursor for you in software. Note that a mouse cursor rendered via regular GPU rendering will feel more laggy than hardware cursor, but will be more in sync with your other visuals.");

		ImGui::Text("WantCaptureMouse: %d", io.WantCaptureMouse);
		ImGui::Text("WantCaptureKeyboard: %d", io.WantCaptureKeyboard);
		ImGui::Text("WantTextInput: %d", io.WantTextInput);
		ImGui::Text("WantMoveMouse: %d", io.WantMoveMouse);

		if (ImGui::TreeNode("Keyboard & Mouse State"))
		{
			ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
			ImGui::Text("Mouse down:");     for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (io.MouseDownDuration[i] >= 0.0f)   { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
			ImGui::Text("Mouse clicked:");  for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseClicked(i))          { ImGui::SameLine(); ImGui::Text("b%d", i); }
			ImGui::Text("Mouse dbl-clicked:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDoubleClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
			ImGui::Text("Mouse released:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseReleased(i))         { ImGui::SameLine(); ImGui::Text("b%d", i); }
			ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);

			ImGui::Text("Keys down:");      for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (io.KeysDownDuration[i] >= 0.0f)     { ImGui::SameLine(); ImGui::Text("%d (%.02f secs)", i, io.KeysDownDuration[i]); }
			ImGui::Text("Keys pressed:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyPressed(i))             { ImGui::SameLine(); ImGui::Text("%d", i); }
			ImGui::Text("Keys release:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyReleased(i))            { ImGui::SameLine(); ImGui::Text("%d", i); }
			ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");


			ImGui::Button("Hovering me sets the\nkeyboard capture flag");
			if (ImGui::IsItemHovered())
				ImGui::CaptureKeyboardFromApp(true);
			ImGui::SameLine();
			ImGui::Button("Holding me clears the\nthe keyboard capture flag");
			if (ImGui::IsItemActive())
				ImGui::CaptureKeyboardFromApp(false);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Tabbing"))
		{
			ImGui::Text("Use TAB/SHIFT+TAB to cycle through keyboard editable fields.");
			static char buf[32] = "dummy";
			ImGui::InputText("1", buf, IM_ARRAYSIZE(buf));
			ImGui::InputText("2", buf, IM_ARRAYSIZE(buf));
			ImGui::InputText("3", buf, IM_ARRAYSIZE(buf));
			ImGui::PushAllowKeyboardFocus(false);
			ImGui::InputText("4 (tab skip)", buf, IM_ARRAYSIZE(buf));
			//ImGui::SameLine(); ShowHelperMarker("Use ImGui::PushAllowKeyboardFocus(bool)\nto disable tabbing through certain widgets.");
			ImGui::PopAllowKeyboardFocus();
			ImGui::InputText("5", buf, IM_ARRAYSIZE(buf));
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Focus from code"))
		{
			bool focus_1 = ImGui::Button("Focus on 1"); ImGui::SameLine();
			bool focus_2 = ImGui::Button("Focus on 2"); ImGui::SameLine();
			bool focus_3 = ImGui::Button("Focus on 3");
			int has_focus = 0;
			static char buf[128] = "click on a button to set focus";

			if (focus_1) ImGui::SetKeyboardFocusHere();
			ImGui::InputText("1", buf, IM_ARRAYSIZE(buf));
			if (ImGui::IsItemActive()) has_focus = 1;

			if (focus_2) ImGui::SetKeyboardFocusHere();
			ImGui::InputText("2", buf, IM_ARRAYSIZE(buf));
			if (ImGui::IsItemActive()) has_focus = 2;

			ImGui::PushAllowKeyboardFocus(false);
			if (focus_3) ImGui::SetKeyboardFocusHere();
			ImGui::InputText("3 (tab skip)", buf, IM_ARRAYSIZE(buf));
			if (ImGui::IsItemActive()) has_focus = 3;
			ImGui::PopAllowKeyboardFocus();
			if (has_focus)
				ImGui::Text("Item with focus: %d", has_focus);
			else
				ImGui::Text("Item with focus: <none>");
			ImGui::TextWrapped("Cursor & selection are preserved when refocusing last used item in code.");
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Hovering"))
		{
			// Testing IsWindowHovered() function
			ImGui::BulletText(
				"IsWindowHovered() = %d\n"
				"IsWindowHovered(_AllowWhenBlockedByPopup) = %d\n"
				"IsWindowHovered(_AllowWhenBlockedByActiveItem) = %d\n",
				ImGui::IsWindowHovered(),
				ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup),
				ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem));

			// Testing IsItemHovered() function (because BulletText is an item itself and that would affect the output of IsItemHovered, we pass all lines in a single items to shorten the code)
			ImGui::Button("ITEM");
			ImGui::BulletText(
				"IsItemHovered() = %d\n"
				"IsItemHovered(_AllowWhenBlockedByPopup) = %d\n"
				"IsItemHovered(_AllowWhenBlockedByActiveItem) = %d\n"
				"IsItemHovered(_AllowWhenOverlapped) = %d\n"
				"IsItemhovered(_RectOnly) = %d\n",
				ImGui::IsItemHovered(),
				ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup),
				ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem),
				ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped),
				ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly));

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Dragging"))
		{
			ImGui::TextWrapped("You can use ImGui::GetMouseDragDelta(0) to query for the dragged amount on any widget.");
			ImGui::Button("Drag Me");
			if (ImGui::IsItemActive())
			{
				// Draw a line between the button and the mouse cursor
				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				draw_list->PushClipRectFullScreen();
				draw_list->AddLine(ImGui::CalcItemRectClosestPoint(io.MousePos, true, -2.0f), io.MousePos, ImColor(ImGui::GetStyle().Colors[ImGuiCol_Button]), 4.0f);
				draw_list->PopClipRect();
				ImVec2 value_raw = ImGui::GetMouseDragDelta(0, 0.0f);
				ImVec2 value_with_lock_threshold = ImGui::GetMouseDragDelta(0);
				ImVec2 mouse_delta = io.MouseDelta;
				ImGui::SameLine(); ImGui::Text("Raw (%.1f, %.1f), WithLockThresold (%.1f, %.1f), MouseDelta (%.1f, %.1f)", value_raw.x, value_raw.y, value_with_lock_threshold.x, value_with_lock_threshold.y, mouse_delta.x, mouse_delta.y);
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Mouse cursors"))
		{
			ImGui::Text("Hover to see mouse cursors:");
			ImGui::SameLine(); ShowHelpMarker("Your application can render a different mouse cursor based on what ImGui::GetMouseCursor() returns. If software cursor rendering (io.MouseDrawCursor) is set ImGui will draw the right cursor for you, otherwise your backend needs to handle it.");
			for (int i = 0; i < ImGuiMouseCursor_Count_; i++)
			{
				char label[32];
				sprintf_s(label, "Mouse cursor %d", i);
				ImGui::Bullet(); ImGui::Selectable(label, false);
				if (ImGui::IsItemHovered())
					ImGui::SetMouseCursor(i);
			}
			ImGui::TreePop();
		}
	}
	ImGui::EndChild();
}

void Container::PanelSwitcher()
{
	/* TEMP */
	const char* names[] = { "None", "MainMenu", "Scene", "Outliner", "Test", "Stats" };
	/**     */
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor(17, 189, 2, 255));
	if (ImGui::Button(names[Type])) { ImGui::OpenPopup("TypeList"); }
	if (ImGui::BeginPopup("TypeList"))
	{
		ImGui::Text("Panel Types");
		ImGui::Separator();
		for (int i = 0; i < TypeList.size(); i++)
		{
			if (ImGui::Selectable(names[i]))
			{
				Type = TypeList[i];
			}
		}
		ImGui::EndPopup();
	}
	ImGui::PopStyleColor();
}

void Container::WindowSpliter()
{
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor(148, 18, 2, 255));
	ImGui::SameLine();		 if (ImGui::Button("Split Horizontally")) { SplitRegion(false); };
	ImGui::SameLine();		 if (ImGui::Button("Split Vertically")) { SplitRegion(true); };
	ImGui::PopStyleColor();
}

void Container::SplitRegion(bool bVertical)
{
	/*
	 get node holding the container

	 create new treenode for the left
	 copy contents to the left node

	 create new treenode for the right node
	 create new container class for right node

	 change holding node contents to a new splitter class
	*/

	ImVec2 TempSize = OwningNode->GetRegionSize();
	ImVec2 ChildSize = (bVertical) ? ImVec2(TempSize.x / 2 - 2, TempSize.y) : ImVec2(TempSize.x, TempSize.y / 2 - 2);  // @TODO: integrate in border spacing. hardcoded as "- 2"
	RegionTypes OwnerType = dynamic_cast<Container*>(OwningNode->Contents)->GetType();

	RegionData LeftData = RegionData(OwnerType, ChildSize, OwningNode->GetRegionPosition(), false);
	OwningNode->LeftNode = new TreeNode(OwningLayout->RegionCount + 1, LeftData, OwningLayout);


	ImVec2 ChildPosition = (bVertical) ? ImVec2(ChildSize.x + OwningNode->GetRegionPosition().x + 4, OwningNode->GetRegionPosition().y) : ImVec2(OwningNode->GetRegionPosition().x, ChildSize.y + OwningNode->GetRegionPosition().y + 4);
	RegionData RightData = RegionData(OwnerType, ChildSize, ChildPosition, false);
	OwningNode->RightNode = new TreeNode(OwningLayout->RegionCount + 2, RightData, OwningLayout);



	ImVec2 SpacerSize = (bVertical) ? ImVec2(OwningLayout->SplitSpacing, OwningNode->GetRegionSize().y) : ImVec2(OwningNode->GetRegionSize().x, OwningLayout->SplitSpacing);
	ImVec2 SplitterPosition = (bVertical) ? ImVec2(ChildSize.x + OwningNode->GetRegionPosition().x, OwningNode->GetRegionPosition().y) : ImVec2(OwningNode->GetRegionPosition().x, ChildSize.y + OwningNode->GetRegionPosition().y);
	OwningNode->BuildSplitter(SpacerSize, SplitterPosition, bVertical);


	OwningLayout->RegionCount += 2;
}


RegionTypes Container::GetType() { return Type; }
int Region::GetRegionID() { return OwningNode->GetNodeID(); }









Splitter::Splitter(ImVec2 InSize, ImVec2 InPosition, Layout* InLayout, TreeNode* InOwningNode, bool InOrientation) : Region(InLayout, InOwningNode)
{
	bIsVertical = InOrientation;
	ContainerStyleFlags = 0;
	SplitterSize = InSize;
}

bool Splitter::Render()
{
	if (bIsVertical) { ImGui::SameLine(); }

	std::string id = "splitter_" + std::to_string(GetRegionID());
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, (ImVec4) ImColor(255, 255, 255));
	ImGui::BeginChild(id.c_str(), SplitterSize, false);
	(bIsVertical) ? VerticalSplit() : HorizontalSplit();
	ImGui::EndChild();
	ImGui::PopStyleColor();

	if (bIsVertical) { ImGui::SameLine(); }

	return true;
}

void Splitter::VerticalSplit()
{
	float Movement = 0.0f;
	std::string Name = "Vsplit_" + std::to_string(GetRegionID());
	ImGui::InvisibleButton(Name.c_str(), SplitterSize);
	//if (ImGui::IsItemHovered()) { ImGui::SetMouseCursor(ImGuiMouseCursor_::ImGuiMouseCursor_ResizeEW); }
	if (ImGui::IsItemActive())
	{ 
		OwningLayout->ResizingNode.Node = OwningNode;
		OwningLayout->ResizingNode.ResizeAmount = ImVec2(ImGui::GetIO().MouseDelta.x, 0);
	}
}

void Splitter::HorizontalSplit()
{
	std::string Name = "Hsplit_" + std::to_string(GetRegionID());
	ImGui::InvisibleButton(Name.c_str(), SplitterSize);
	//if (ImGui::IsItemHovered()) { ImGui::SetMouseCursor(ImGuiMouseCursor_::ImGuiMouseCursor_ResizeNS); }
	if (ImGui::IsItemActive()) 
	{ 
		OwningLayout->ResizingNode.Node = OwningNode;
		OwningLayout->ResizingNode.ResizeAmount = ImVec2(0, ImGui::GetIO().MouseDelta.y);
	}
}

void Splitter::ResizeRegions(ImVec2 ResizeDelta)
{
	bool OwnerIsVertical = (ResizeDelta.y == 0) ? true : false;
	float Amount = (OwnerIsVertical) ? ResizeDelta.x : ResizeDelta. y;
	ResizeRecursion(OwningNode->LeftNode, OwnerIsVertical, true, Amount);
	ResizeRecursion(OwningNode->RightNode, OwnerIsVertical, false, -1 * Amount);
}

void Splitter::ResizeSplitter(ImVec2 InAmount)
{
	if (bIsVertical)
	{
		SplitterSize.y += InAmount.y;
	}
	else
	{
		SplitterSize.x += InAmount.x;
	}
}

void Splitter::ResizeRecursion(TreeNode* InNode, bool OwnerIsVertical, bool LeftSideTraversal, float InResizeDelta)
{
	if (InNode->IsLeaf())
	{
		ImVec2 Resized = (OwnerIsVertical) ? ImVec2(InResizeDelta, 0) : ImVec2(0, InResizeDelta);
		InNode->ResizeNode(Resized);
		return;
	}

	if (LeftSideTraversal)
	{
		if (dynamic_cast<Splitter*>(InNode->Contents)->bIsVertical != OwnerIsVertical)
		{
			ImVec2 Resized = (OwnerIsVertical) ? ImVec2(InResizeDelta, 0) : ImVec2(0, InResizeDelta);
			InNode->ResizeNode(Resized);

			ResizeRecursion(InNode->LeftNode, OwnerIsVertical, LeftSideTraversal, InResizeDelta);
		}
		ResizeRecursion(InNode->RightNode, OwnerIsVertical, LeftSideTraversal, InResizeDelta);
	}
	else
	{
		if (dynamic_cast<Splitter*>(InNode->Contents)->bIsVertical != OwnerIsVertical)
		{
			ImVec2 Resized = (OwnerIsVertical) ? ImVec2(InResizeDelta, 0) : ImVec2(0, InResizeDelta);
			InNode->ResizeNode(Resized);

			ResizeRecursion(InNode->RightNode, OwnerIsVertical, LeftSideTraversal, InResizeDelta);
		}
		ResizeRecursion(InNode->LeftNode, OwnerIsVertical, LeftSideTraversal, InResizeDelta);
	}
}


TreeNode::TreeNode(int InNodeID, RegionData InData, Layout* InOwningLayout)
{
	NodeID = InNodeID;
	OwningLayout = InOwningLayout;
	Data = InData;

	Contents = CreateContents();
	RightNode = NULL;
	LeftNode = NULL;
}

TreeNode::~TreeNode()
{
	if (RightNode)
	{
		if (RightNode->Contents) { RightNode->Contents->~Region(); }
		RightNode->~TreeNode(); 
	}

	if (LeftNode) 
	{ 
		if (LeftNode->Contents) { LeftNode->Contents->~Region(); }
		LeftNode->~TreeNode(); 
	}
}

void TreeNode::Render()
{
	if (Contents)
	{
		BeginRegionChild(NodeID, Data.Size, Contents->GetStyleFlags()); // @TODO: Add content style flags.

		if (IsLeaf())
		{
			if (ImGui::IsItemHovered()) { Contents->GetOwningLayout()->SetHoveredRegion(Contents); };
			Contents->Render();
			EndRegionChild();
			return;
		}

		if (LeftNode) { LeftNode->Render(); }
		Contents->Render();
		if (RightNode) { RightNode->Render(); }

		EndRegionChild();
	}
}

void TreeNode::BuildSplitter(ImVec2 InSize, ImVec2 InPosition, bool InOrientation)
{
	Contents->~Region();
	Contents = new Splitter(InSize, InPosition, OwningLayout, this, InOrientation);
	Data.Type = RegionTypes::Spacer;
}

void TreeNode::BeginRegionChild(int InRegionID, ImVec2 InSize, ImGuiWindowFlags flags)
{
	std::string Name = "RegionChild_" + std::to_string(InRegionID);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8.0f, 6.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
	ImGui::BeginChild(Name.c_str(), InSize, false, flags);
}

void TreeNode::EndRegionChild()
{
	ImGui::EndChild();
	ImGui::PopStyleVar(4);
}

Region* TreeNode::CreateContents()
{
	if (Data.Type != RegionTypes::Spacer)
	{
		return new Container(OwningLayout, this, Data.Type);
	}

	return NULL;
}

void TreeNode::ResizeNode(ImVec2 InAmount)
{
	Data.Size.x += InAmount.x;
	Data.Size.y += InAmount.y;

	if (Data.Type == RegionTypes::Spacer)
	{
		Splitter* split = dynamic_cast<Splitter*>(Contents);
		if (split) { split->ResizeSplitter(InAmount); }
	}
}

int TreeNode::GetNodeID() { return NodeID; }
Layout* TreeNode::GetOwningLayout() { return OwningLayout; }
ImVec2 TreeNode::GetRegionSize() { return Data.Size; }
ImVec2 TreeNode::GetRegionPosition() { return Data.Position; }
bool TreeNode::IsLeaf() { return (RightNode || LeftNode) ? false : true; }