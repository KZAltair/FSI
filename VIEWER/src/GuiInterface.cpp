#include "GuiInterface.h"
#include "Core/Application.h"
#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>


GuiInterface::GuiInterface()
	: Layer("Example")
{
	fsicore::Application* app = &fsicore::Application::Get();
	occtLayer = app->GetOcctLayer();
	p_ModelsContainer = app->GetSceneContainer();

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
			
				mCurrentFile = "< ... >";

				mFileDialog.SetTitle("Open ifc project");
				mFileDialog.SetFileFilters({ ".ifc" });

				set_mesh_load_callback(
					[this](std::string filepath) { load(filepath); });
				mFileDialog.Open();
			}
			if (ImGui::MenuItem("Save", "CTRL+S")) {}
			if (ImGui::MenuItem("Save as...", "CTRL+ALT+S", false, false)) {}  // Disabled item
			if (ImGui::MenuItem("Exit", "CTRL+E")) {}
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
		p_ModelsContainer->ClearAllObjects();
	}
	if (ImGui::CollapsingHeader("Object Nodes", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Separator();
		ImGui::Text("Default Node");
	}

	if (ImGui::CollapsingHeader("Object Material"))
	{
		//ImGui::ColorPicker3("Color", (float*)&mesh->mColor, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);
		//ImGui::SliderFloat("Roughness", &mesh->mRoughness, 0.0f, 1.0f);
		//ImGui::SliderFloat("Metallic", &mesh->mMetallic, 0.0f, 1.0f);
	}

	if (ImGui::CollapsingHeader("Scene Light"))
	{

		ImGui::Separator();
		ImGui::Text("Position");
		ImGui::Separator();
		OnImGuiDrawWidget();
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

/*
void GuiInterface::OnEvent(fsicore::Event& event)
{
	if (event.GetEventType() == fsicore::EventType::KeyPressed)
	{
		fsicore::KeyPressedEvent& e = (fsicore::KeyPressedEvent&)event;
		if (e.GetKeyCode() == FSI_KEY_TAB)
			FSI_TRACE("Tab key is pressed (event)!");
		FSI_TRACE("{0}", (char)e.GetKeyCode());
	}
}
*/