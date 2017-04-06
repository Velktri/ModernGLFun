#include "Layout.h"
#include "../World.h"
#include "../Manager.h"
#include "../Models/Asset.h"
#include "../Models/Mesh.h"
#include <Windows.h>
#include <vector>
#include <tchar.h>


Layout::Layout(SDL_Window* InWindow, ImVec2 InWindowDimensions, std::string path) {
	WindowDimensions = InWindowDimensions;
	Window = InWindow;
	bIsHoveringScene = false;
	SceneSizeModifier = ImVec2(0.78f, 0.78f);
	SceneDimensions = ImVec2(WindowDimensions.x * SceneSizeModifier.x, WindowDimensions.y * SceneSizeModifier.y);

	ImGui_ImplSdlGL3_Init(Window);
	SetDefaultStyle(path);
}

Layout::~Layout() {
}

void Layout::SetManager(Manager* m) {
	MyManager = m;
}

bool Layout::RenderLayout(GLuint TextureColorBuffer) {
	bool bIsRunning = true;
	bIsHoveringScene = false;
	ImGui_ImplSdlGL3_NewFrame(Window);

	bIsRunning = MainMenu();
	AssetEditor();
	SceneWindow(TextureColorBuffer);

	ImGui::SetNextWindowSize(ImVec2(WindowDimensions.x - 1500, WindowDimensions.y - 470), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(1500, 470), ImGuiSetCond_FirstUseEver);
	ImGui::ShowTestWindow();

	glViewport(0, 0, SceneDimensions.x, SceneDimensions.y);
	ImGui::Render();
	return bIsRunning;
}

ImVec2 Layout::GetSceneDimensions() {
	return SceneDimensions;
}

void Layout::SetWorld(World* InWorld) {
	world = InWorld;
}

bool Layout::GetSceneHovering() {
	return bIsHoveringScene;
}

void Layout::SetDefaultStyle(std::string path) {
	ImGuiStyle* style = &ImGui::GetStyle();

	//style->WindowPadding = ImVec2(15, 15);
	//style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 3);
	style->FrameRounding = 16.0f;
	style->ItemSpacing = ImVec2(6, 6);
	style->ItemInnerSpacing = ImVec2(6, 4);
	//style->IndentSpacing = 25.0f;
	//style->ScrollbarSize = 15.0f;
	//style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 20.0f;
	style->GrabRounding = 16.0f;

	//style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	//style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	//style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	//style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	//style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	//style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	//style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	//style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	//style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	//style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	//style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	//style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	//style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	//style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	//style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	//style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	//style->Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	//style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	//style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	//style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	//style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	//style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	//style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	//style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	//style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	//style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	//style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	//style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
	//style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
	//style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
	//style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	//style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	//style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	//style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	//style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	//style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
}

void Layout::ImportAsset() {
	if (world) {
		world->StopClock();
		OPENFILENAME ofn;

		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = 0;
		ofn.lpstrDefExt = 0;
		ofn.lpstrFile  = new TCHAR[MAX_PATH];
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrFilter = "Object Files (*.obj)\0*.obj\0All Files (*.*)\0*.*\0\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrInitialDir = _T("C:\\");
		ofn.lpstrTitle = _T("Import");
		ofn.Flags = OFN_SHOWHELP;

		GetOpenFileName(&ofn);
		MyManager->BuildAsset(ofn.lpstrFile);
		world->StartClock();
	} else {
		printf("Current UI has no Access to World*");
	}
}

void Layout::CreatePrimative(std::string name) {
	if (name.compare("Empty") == 0) {
		MyManager->BuildAsset();
	} else if (name.compare("Cube") == 0) {
		MyManager->BuildAsset("assets/Models/Primitives/cube.obj");
	} else if (name.compare("Plane") == 0) {
		MyManager->BuildAsset("assets/Models/Primitives/plane.obj");
	} else if (name.compare("Sphere") == 0) {
		MyManager->BuildAsset("assets/Models/Primitives/Sphere.obj");
	} else if (name.compare("Cylinder") == 0) {
		MyManager->BuildAsset("assets/Models/Primitives/cylinder.obj");
	} else if (name.compare("SmoothTest") == 0) {
		MyManager->BuildAsset("assets/Models/Primitives/smoothSphere.obj");
	}
}

bool Layout::MainMenu() {
	bool check = true;
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Quit", NULL)) { check = false; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Undo", "CTRL+Z")) { printf("Pressed Undo!"); }
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) { printf("Pressed Cut!"); }
			if (ImGui::MenuItem("Copy", "CTRL+C")) { printf("Pressed Copy!"); }
			if (ImGui::MenuItem("Paste", "CTRL+V")) { printf("Pressed Paste!"); }
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	return check;
}

void Layout::AssetEditor() {
	ImGui::SetNextWindowSize(ImVec2(420, 450), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(WindowDimensions.x - 420, 20), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Asset editor", false);
		Asset* SelectedAsset = MyManager->GetSelectedAsset();
		ImGui::BeginChild("left pane", ImVec2(150, 0), true);
			for each (Asset* a in MyManager->GetAssets()) {
				if (ImGui::Selectable(a->Name.c_str(), SelectedAsset == a)) { MyManager->SetSelectedAsset(a); }
			}
		ImGui::EndChild();
	ImGui::SameLine();

	if (SelectedAsset != NULL) {
		ImGui::BeginGroup();
		ImVec2 i = ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing());
			ImGui::BeginChild("Item view", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()));
				ImGui::Text(SelectedAsset->Name.c_str());
				ImGui::Separator();
										if (ImGui::Button("Move X")) { SelectedAsset->TranslateAsset(1, 0, 0); }
				ImGui::SameLine();		if (ImGui::Button("Move Y")) { SelectedAsset->TranslateAsset(0, 1, 0); }
				ImGui::SameLine();		if (ImGui::Button("Move Z")) { SelectedAsset->TranslateAsset(0, 0, 1); }

										if (ImGui::Button("Move -X")) { SelectedAsset->TranslateAsset(-1, 0, 0); }
				ImGui::SameLine();		if (ImGui::Button("Move -Y")) { SelectedAsset->TranslateAsset(0, -1, 0); }
				ImGui::SameLine();		if (ImGui::Button("Move -Z")) { SelectedAsset->TranslateAsset(0, 0, -1); }

				ImGui::Separator();

				ImGui::Text("Mesh Properties");
				ImGui::Separator();

				static int selection = -1;
				ImGui::Text(MyManager->GetSelectedAsset()->GetMesh() ? MyManager->GetSelectedAsset()->GetMesh()->GetName().c_str() : "None");

				ImGui::SameLine();		if (ImGui::Button("Set Mesh")) { ImGui::OpenPopup("select"); }

				if (ImGui::BeginPopup("select")) {
					std::vector<Mesh*> meshList = MyManager->GetMeshList();
					for (int i = 0; i < meshList.size(); i++) {
						if (ImGui::Selectable(meshList[i]->GetName().c_str())) { selection = i; SelectedAsset->SetMesh(meshList.at(i)); }
					}

					ImGui::EndPopup();
				}
			ImGui::EndChild();


			ImGui::BeginChild("buttons");
										if (ImGui::Button("Revert")) {}
				ImGui::SameLine();		if (ImGui::Button("Save")) {}
			ImGui::EndChild();
		ImGui::EndGroup();
	}
	ImGui::End();
}

void Layout::SceneWindow(GLuint TextureColorBuffer) {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::SetNextWindowSize(ImVec2(SceneDimensions.x, SceneDimensions.y + 50.0f), ImGuiSetCond_FirstUseEver);

	WindowPos = ImVec2(0.0f, WindowDimensions.y - SceneDimensions.y - 49.f);
	ImGui::SetNextWindowPos(WindowPos, ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Scene", false, ImGuiWindowFlags_NoMove |
								 ImGuiWindowFlags_NoCollapse |
								 ImGuiWindowFlags_NoResize |
								 ImGuiWindowFlags_NoTitleBar |
								 ImGuiWindowFlags_NoScrollbar |
								 ImGuiWindowFlags_NoSavedSettings |
								 ImGuiWindowFlags_MenuBar);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 8.0f));
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("Menu")) {
				//ShowExampleMenuFile();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Examples")) {
				ImGui::MenuItem("NULL", NULL, false); { };
				ImGui::MenuItem("NULL", NULL, false); { };
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help")) {
				ImGui::MenuItem("NULL", NULL, false); { };
				ImGui::MenuItem("NULL", NULL, false); { };
				ImGui::MenuItem("NULL", NULL, false); { };
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::PopStyleVar(1);

		ImGui::Spacing();
		ImGui::BeginGroup();
			ImGui::Indent(15.0f);

									 if (ImGui::Button("Import"))		{ ImportAsset(); };
			ImGui::SameLine();		 if (ImGui::Button("Empty Asset"))  { CreatePrimative("Empty"); };
			ImGui::SameLine();		 if (ImGui::Button("Cube"))			{ CreatePrimative("Cube"); };
			ImGui::SameLine();		 if (ImGui::Button("Plane"))		{ CreatePrimative("Plane"); };
			ImGui::SameLine();		 if (ImGui::Button("Sphere"))		{ CreatePrimative("Sphere"); };
			ImGui::SameLine();		 if (ImGui::Button("Cylinder"))		{ CreatePrimative("Cylinder"); };
			ImGui::SameLine();		 if (ImGui::Button("SmoothTest"))	{ CreatePrimative("SmoothTest"); };
		ImGui::EndGroup();

		ImGui::BeginGroup();
			ImGui::Image((GLuint*)TextureColorBuffer, SceneDimensions, ImVec2(0, 1), ImVec2(1, 0), ImColor(255, 255, 255, 255), ImVec4(0, 0, 0, 0));
		ImGui::EndGroup();
		if (ImGui::IsItemHovered()) { bIsHoveringScene = true; };
	ImGui::End();
	ImGui::PopStyleVar(2);
}

