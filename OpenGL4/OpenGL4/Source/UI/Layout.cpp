#include "Layout.h"
#include "System/Input.h"
#include "Shared/GeoString.h"
#include "Shared/GeoTree.h"
#include "Region.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <queue>


Layout::Layout(SDL_Window* InWindow, Manager* InManager, Universe* InUniverse, Input* InInput, std::string path)
{
	Window = InWindow;
	MyManager = InManager;
	MyUniverse = InUniverse;
	MyInput = InInput;

	ResizingNode = ResizingData();
	HoveredRegion = NULL;
	bQuitLayout = false;
	RegionCount = 0;

	UpdateWindowSize();
	ImGui_ImplSdlGL3_Init(Window);
	LoadDefaultStyle(path);
	LoadDefaultLayout();
}

Layout::~Layout()
{
	if (LayoutRoot) { delete LayoutRoot; }
}

void Layout::LoadDefaultLayout()
{
	LayoutRoot = GenerateLayout("Preferences/Default/DefaultLayout.lo"); // @TODO: change this to a setting in a config file.
	if (LayoutRoot)
	{
		RefreshContainerSizes(LayoutRoot);
	}
	else
	{ 
		printf("Error: Could not create Layout Root.\n"); 
	}
}

bool Layout::RenderLayout()
{
	UpdateWindowSize();
	ImGui_ImplSdlGL3_NewFrame(Window);

	bool bIsRunning = !MasterWindow();
	ImGui::Render();
	return bIsRunning;
}

void Layout::LoadDefaultStyle(std::string path)
{ 
	ImGui::StyleColorsDark(); 
	SplitSpacing = 2.0f;
	GlobalMinRegionSize = 25;
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
										 ImGuiWindowFlags_NoSavedSettings | 
										 ImGuiWindowFlags_NoBringToFrontOnFocus);
		
		RenderRegions();
	ImGui::End();
	ImGui::PopStyleVar(3);
	bool t = true;
	ImGui::ShowTestWindow(&t);
	return bQuitLayout;
}

void Layout::RenderRegions()
{
	bSceneClicked = (MyInput && MyInput->PollSelectionRequest(&PolledRegion)) ? true : false;

	if (LayoutRoot)
	{
		LayoutRoot->Render();

		if (ResizingNode.Node)
		{
			Splitter* split = dynamic_cast<Splitter*>(ResizingNode.Node->GetContents());

			if (split && split->ResizeRegions(ResizingNode.ResizeAmount))
			{
				RefreshContainerSizes(LayoutRoot);
			}

			ResizingNode.Node = NULL;
			ResizingNode.ResizeAmount = ImVec2();
		}
	}
}

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
	// @TODO: destroy and recreate camera frame buffers when resizing.
}

ImVec2 Layout::RefreshContainerSizes(TreeNode* InNode)
{
	if (InNode)
	{
		if (InNode->RightNode == NULL && InNode->LeftNode == NULL)
		{
			return InNode->GetRegionSize();
		}

		ImVec2 RightSize = RefreshContainerSizes(InNode->RightNode);
		ImVec2 LeftSize = RefreshContainerSizes(InNode->LeftNode);

		Splitter* split = dynamic_cast<Splitter*>(InNode->GetContents());
		bool bIsVertical = true;
		if (split) { bIsVertical = split->GetOrientation(); }

		ImVec2 ResizeAmount = (bIsVertical) ? ImVec2(RightSize.x + LeftSize.x + SplitSpacing, InNode->GetRegionSize().y) : ImVec2(InNode->GetRegionSize().x, RightSize.y + LeftSize.y + SplitSpacing);
		InNode->NewSize(ResizeAmount);

		return InNode->GetRegionSize();
	}
	return ImVec2();
}

TreeNode* Layout::GenerateLayout(char* FilePath)
{
	std::string content;
	std::ifstream fileStream(FilePath, std::ios::in);
	if (!fileStream.is_open())
	{
		std::cerr << "Could not read file " << FilePath << ". File does not exist." << std::endl;
		return NULL;
	}

	TreeNode* Root = NULL;
	std::queue<TreeNode*> NodeStack;
	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");

		if (line != "")
		{
			(line == "#") ? NodeStack.push(NULL) : NodeStack.push(BuildNode(line));
		}
	}
	fileStream.close();

	// convert to tree.
	GeoTree::StackToTree<TreeNode>(NodeStack, Root);
	return Root;
}

void Layout::SaveLayout()
{
	std::vector<TreeNode*> NodeArray;
	GeoTree::TreeToArray<TreeNode>(LayoutRoot, &NodeArray);

	std::ofstream OutFile;
	OutFile.open("Preferences/Default/SavedLayout.lo");
	for (TreeNode* node : NodeArray)
	{
		if (node)
		{

			OutFile << static_cast<int>(node->GetContents()->GetType()) << ","
					<< node->GetRegionSize().x << ","
					<< node->GetRegionSize().y << ","
					<< "0";
					
			if (RegionTypes(node->GetContents()->GetType()) == RegionTypes::Spacer)
			{
				Splitter* split = dynamic_cast<Splitter*>(node->GetContents());
				OutFile << "," << split->GetSplitterSize().x << ","
							   << split->GetSplitterSize().y << ","
							   << split->GetOrientation();
			}
					
			OutFile	<< std::endl;
		}
		else
		{
			OutFile << "#" << std::endl;
		}
	}

	OutFile.close();
}

TreeNode* Layout::BuildNode(std::string NodeString)
{
	std::istringstream iss(NodeString);
	std::string item;
	std::vector<std::string> Data;
	while (std::getline(iss, item, ','))
	{
		GeoString::trim(item);
		Data.push_back(item);
	}
	
	RegionData NodeData = RegionData(RegionTypes(std::stoi(Data[0])),
									 ImVec2(std::stoi(Data[1]), std::stoi(Data[2])),
									 GeoString::ParseBool(Data[3])); // @TODO fix array sizes since position node was deleted.

	TreeNode* NewNode = new TreeNode(NodeData, this);
	if (static_cast<RegionTypes>(NodeData.Type) == RegionTypes::Spacer)
	{
		bool VOrientation = GeoString::ParseBool(Data[6]);
		ImVec2 SpaceSize = (VOrientation) ? ImVec2(SplitSpacing, std::stoi(Data[5])) : ImVec2(std::stoi(Data[4]), SplitSpacing);

		NewNode->BuildSplitter(SpaceSize, VOrientation);
	}

	return NewNode;
}

Universe* Layout::GetUniverse() { return MyUniverse; }
Input* Layout::GetInput() { return MyInput; }
Manager* Layout::GetManager() { return MyManager; }
Region* Layout::GetHoveredRegion() { return HoveredRegion; }
void Layout::SetHoveredRegion(Region* InRegion) { HoveredRegion = InRegion; }
bool Layout::IsSceneClicked() { return bSceneClicked; }
int Layout::GetPolledRegion() { return PolledRegion; }
int Layout::GetGlobalMinSize() { return GlobalMinRegionSize; }
int Layout::AddAndGetRegionCount() { return RegionCount++; }
void Layout::ShutDown() { bQuitLayout = true; }











TreeNode::TreeNode(RegionData InData, Layout* InOwningLayout)
{
	OwningLayout = InOwningLayout;
	NodeID = OwningLayout->AddAndGetRegionCount();
	Data = InData;

	Contents = CreateContents();
	RightNode = NULL;
	LeftNode = NULL;
}

TreeNode::~TreeNode()
{
	if (RightNode)
	{
		if (RightNode->Contents) { delete RightNode->Contents; }
		delete RightNode; 
	}

	if (LeftNode) 
	{ 
		if (LeftNode->Contents) { delete LeftNode->Contents; }
		delete LeftNode; 
	}
}

void TreeNode::Render()
{
	if (Contents)
	{
		BeginRegionChild(NodeID, Data.Size, Contents->GetStyleFlags());

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

void TreeNode::BuildSplitter(ImVec2 InSize, bool InOrientation)
{
	if (Contents) {	delete Contents; }
	Contents = new Splitter(InSize, OwningLayout, this, InOrientation);
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
Region* TreeNode::GetContents() { return Contents; }
Layout* TreeNode::GetOwningLayout() { return OwningLayout; }
ImVec2 TreeNode::GetRegionSize() { return Data.Size; }
bool TreeNode::IsLeaf() { return (RightNode || LeftNode) ? false : true; }
void TreeNode::NewSize(ImVec2 InAmount) { Data.Size = InAmount; }
