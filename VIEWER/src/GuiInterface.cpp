#include "GuiInterface.h"
#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>



GuiInterface::GuiInterface()
	: Layer("Example")
{
	app = &fsicore::Application::Get();
	occtLayer = app->GetOcctLayer();
	p_ModelsContainer = app->GetSceneContainer();
}

GuiInterface::~GuiInterface()
{
}

void GuiInterface::OnUpdate()
{
	if (fsicore::Input::IsKeyPressed(FSI_KEY_TAB))
		FSI_TRACE("Tab key is pressed (poll)!");

}

void GuiInterface::OnImGuiRender()
{
	
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O")) {
			
				if (p_ModelsContainer->GetScenePtr())
				{
					mSelectionContext = -1;
					p_ModelsContainer->ClearAllObjects();
				}

				mCurrentFile = "< ... >";

				mFileDialog.SetTitle("Open ifc project");
				mFileDialog.SetFileFilters({ ".ifc" });

				set_mesh_load_callback(
					[this](std::string filepath) { load(filepath); });
				mFileDialog.Open();
			}
			if (ImGui::MenuItem("Save", "CTRL+S")) {}
			if (ImGui::MenuItem("Save as...", "CTRL+ALT+S", false, false)) {}  // Disabled item
			if (ImGui::MenuItem("Exit", "CTRL+E")) {
				
				app->GetWindow().SetWindowClosed();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	mFileDialog.Display();
	
	ImGui::Begin("Property");
	/*
	if (ImGui::Button("Open..."))
	{
		mCurrentFile = "< ... >";

		mFileDialog.SetTitle("Open ifc project");
		mFileDialog.SetFileFilters({ ".ifc" });

		set_mesh_load_callback(
			[this](std::string filepath) { load(filepath); });
		mFileDialog.Open();
	}
	*/

	if (mFileDialog.HasSelected())
	{
		auto file_path = mFileDialog.GetSelected().string();
		mCurrentFile = file_path.substr(file_path.find_last_of("/\\") + 1);

		mMeshLoadCallback(file_path);

		mFileDialog.ClearSelected();
	}
	if (ImGui::Button("Generate"))
	{
		
	}
	if (ImGui::Button("Reset"))
	{
		mSelectionContext = -1;
		occt_empty_scene_callback(
			[this](bool flag) { occtEmptyScene(flag); });
		fEmptySceneCallback(true);

		p_ModelsContainer->ClearAllObjects();
	}
	ImGui::Text("Select floors");
	if (ImGui::TreeNodeEx((void*)typeid(int).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Floors Filter"))
	{
		if (p_ModelsContainer->GetScenePtr())
		{
			int index = 0;
			auto m_objects = p_ModelsContainer->GetScenePtr()->getObjects();
			for (int obj_i = 0; obj_i < (int)m_objects.size(); obj_i++)
			{
				auto floor = std::dynamic_pointer_cast<fsi::BuildingStorey>(m_objects.at(obj_i));
				if (floor)
				{
				
					label = "Floor_" + std::to_string(index);
					lables.push_back(label);
					//CreateNodesList("Object", index);
					//CreateNode("Object", index);
					index++;
				}
			}
			const char* currentSelection = lables.at(0).c_str();
			if (ImGui::BeginCombo("F", currentSelection))
			{
				for (int i = 0; i < index; i++)
				{
					bool isSelected = currentSelection == lables.at(i).c_str();
					if (ImGui::Selectable(lables.at(i).c_str(), isSelected))
					{
						currentSelection = lables.at(i).c_str();
					}
				}
				ImGui::EndCombo();
			}
		}
		ImGui::TreePop();
	}
	if (ImGui::CollapsingHeader("Object Nodes", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (p_ModelsContainer->GetScenePtr())
		{
			
			if (ImGui::Checkbox("Hide all objects", &showHideAllobjects))
			{
				allobjects_showhide_callback(
					[this](bool flag) { showHideAllObjects(flag); });
				mObjectShowHideCallback(showHideAllobjects);
				checkBoxes.clear();
			}
			int index = 0;
			auto m_objects = p_ModelsContainer->GetScenePtr()->getObjects();
			for (int obj_i = 0; obj_i < (int)m_objects.size(); obj_i++)
			{
				auto pGeomObj = std::dynamic_pointer_cast<fsi::GeometryObject>(m_objects.at(obj_i));
				if (pGeomObj)
				{
					if (pGeomObj->m_shape)
					{
						//CreateNodesList("Object", index);
						CreateNode("Object", index);
						index++;
					}
				}
			}
			for (int i = 0; i < index; i++)
			{
				if (checkBoxes.empty() || checkBoxes.size() < index)
				{
					checkBoxes.push_back(flag);
				}
			}
		}
		
	}
	ImGui::End();

	//Floor contents
	ImGui::Begin("Floor property");
	if (mSelectionContext >= 0)
	{
		DrawPropertyPannel();
	}
	ImGui::End();
}

void GuiInterface::OnImGuiDrawWidget()
{
	//temp
	float value = 0.0f;

	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];


	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushFont(boldFont);
	ImGui::Button("X", buttonSize);
	ImGui::PopFont();

	ImGui::SameLine();
	ImGui::DragFloat("##X", &value, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushFont(boldFont);
	ImGui::Button("Y", buttonSize);
	ImGui::PopFont();

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &value, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushFont(boldFont);
	ImGui::Button("Z", buttonSize);
	ImGui::PopFont();

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &value, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();
}


void GuiInterface::CreateNode(const char* prefix, int uid)
{
	
	ImGuiTreeNodeFlags flags = ((mSelectionContext == uid) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

	//bool node_open = ImGui::TreeNode("Object", "%s_%u", prefix, uid);

	bool node_open = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)uid, flags, "Object_%u", uid);
	if (ImGui::IsItemClicked())
	{
		mSelectionContext = uid;
		
	}
	if (node_open)
	{
		/*
			{
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
				ImGui::TreeNodeEx("Object Layer", flags, "Field_%d", 0);
				
				bool hide = checkBoxes.at(uid);
				if(ImGui::Checkbox("Hide object", &hide))
				{
					single_object_showhide_callback(
						[this](int uid, bool flag) { showHideObject(uid, flag); });
					mSingleObjectShowHideCallback(uid, hide);
				}
				checkBoxes.at(uid) = hide;
			}
			*/
		ImGui::TreePop();
	}
}

void GuiInterface::DrawPropertyPannel()
{
	bool hide = checkBoxes.at(mSelectionContext);
	if (ImGui::Checkbox("Hide object", &hide))
	{
		single_object_showhide_callback(
			[this](int uid, bool flag) { showHideObject(uid, flag); });
		mSingleObjectShowHideCallback(mSelectionContext, hide);
	}
	checkBoxes.at(mSelectionContext) = hide;
}


void GuiInterface::OnEvent(fsicore::Event& event)
{
	/*
	if (event.GetEventType() == fsicore::EventType::KeyPressed)
	{
		fsicore::KeyPressedEvent& e = (fsicore::KeyPressedEvent&)event;
		if (e.GetKeyCode() == FSI_KEY_TAB)
			FSI_TRACE("Tab key is pressed (event)!");
		FSI_TRACE("{0}", (char)e.GetKeyCode());
	}
	*/
	

	
}