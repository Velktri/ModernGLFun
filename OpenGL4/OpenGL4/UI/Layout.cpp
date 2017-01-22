#include "Layout.h"



Layout::Layout(SDL_Window* InWindow, ImVec2 InWindowDimensions, std::string path) {
	WindowDimensions = InWindowDimensions;
	Window = InWindow;
	SceneDimensions = ImVec2(1280, 720);

	ImGui_ImplSdlGL3_Init(Window);

	BuildLayout(path);
	SetDefaultStyle(path);

}


Layout::~Layout() {
}

void Layout::RenderLayout(GLuint TextureColorBuffer) {
	ImGui_ImplSdlGL3_NewFrame(Window);
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			//ShowExampleMenuFile();
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

	ImGui::ShowTestWindow();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(SceneDimensions, ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("test", false, ImGuiWindowFlags_NoMove | 
								ImGuiWindowFlags_NoCollapse |
								ImGuiWindowFlags_NoResize |
								ImGuiWindowFlags_NoTitleBar |
								ImGuiWindowFlags_NoScrollbar |
								ImGuiWindowFlags_NoSavedSettings);

	ImGui::Image((GLuint*) TextureColorBuffer, SceneDimensions, ImVec2(0, 1), ImVec2(1, 0), ImColor(255, 255, 255, 255), ImVec4(0, 0, 0, 0));
	ImGui::End();
	ImGui::PopStyleVar();

	glViewport(0, 0, SceneDimensions.x, SceneDimensions.y);
	ImGui::Render();
}

ImVec2 Layout::GetSceneDimensions() {
	return SceneDimensions;
}

void Layout::BuildLayout(std::string path) {
	if (path.compare("") == 0) {
		BuildDefaultLayout();
	}
}

void Layout::BuildDefaultLayout() {
	//PanelList.push_back(new Panel("Main Menu"));
}

void Layout::SetDefaultStyle(std::string path) {
	//ImGuiStyle * style = &ImGui::GetStyle();

	//style->WindowPadding = ImVec2(15, 15);
	//style->WindowRounding = 5.0f;
	//style->FramePadding = ImVec2(5, 5);
	//style->FrameRounding = 4.0f;
	//style->ItemSpacing = ImVec2(12, 8);
	//style->ItemInnerSpacing = ImVec2(8, 6);
	//style->IndentSpacing = 25.0f;
	//style->ScrollbarSize = 15.0f;
	//style->ScrollbarRounding = 9.0f;
	//style->GrabMinSize = 5.0f;
	//style->GrabRounding = 3.0f;

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

//ImGui_ImplSdlGL3_NewFrame(window);
//// 1. Show a simple window
//// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
//{
//	static float f = 0.0f;
//	ImGui::Text("Hello, world!");
//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
//	ImGui::ColorEdit3("clear color", (float*)&clear_color);
//	if (ImGui::Button("Test Window")) show_test_window ^= 1;
//	if (ImGui::Button("Another Window")) show_another_window ^= 1;
//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//}
//
//// 2. Show another simple window, this time using an explicit Begin/End pair
//if (show_another_window) {
//	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
//	ImGui::Begin("Another Window", &show_another_window);
//	ImGui::Text("Hello");
//	ImGui::End();
//}
//
//// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
//if (show_test_window) {
//	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
//	ImGui::ShowTestWindow(&show_test_window);
//}
//
//ImVec2 NewSize = ImVec2((int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
//
//ImGui::SetNextWindowSize(ImVec2(1200, 900), ImGuiSetCond_FirstUseEver);
//ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiSetCond_FirstUseEver);
//ImGui::Begin("test", false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
//ImGui::Image((GLuint*)textureColorbuffer, NewSize, ImVec2(0, 1), ImVec2(1, 0), ImColor(255, 255, 255, 255), ImVec4(0, 0, 0, 0));
//ImGui::End();
//
//glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
//ImGui::Render();