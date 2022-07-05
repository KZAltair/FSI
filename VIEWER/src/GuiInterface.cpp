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
	mCurrentFile = "< ... >";

	mFileDialog.SetTitle("Open ifc project");
	mFileDialog.SetFileFilters({ ".ifc" });

	set_mesh_load_callback(
		[this](std::string filepath) { load(filepath); });

}

void GuiInterface::OnUpdate()
{
	if (fsicore::Input::IsKeyPressed(FSI_KEY_TAB))
		FSI_TRACE("Tab key is pressed (poll)!");
}
void GuiInterface::OnImGuiRender()
{
	/*
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O")) {
			
				
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
	*/
	
	ImGui::Begin("Property");
	if (ImGui::Button("Open..."))
	{
		mFileDialog.Open();
	}

	mFileDialog.Display();
	if (mFileDialog.HasSelected())
	{
		auto file_path = mFileDialog.GetSelected().string();
		mCurrentFile = file_path.substr(file_path.find_last_of("/\\") + 1);

		mMeshLoadCallback(file_path);

		mFileDialog.ClearSelected();
	}
	if (ImGui::Button("Generate"))
	{
		mFileDialog.Open();
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

void GuiInterface::OnEvent(fsicore::Event& e)
{
	fsicore::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<fsicore::MouseScrolledEvent>(FSI_BIND_EVENT_FN(GuiInterface::OnMouseScrolled));
	dispatcher.Dispatch<fsicore::MouseMovedEvent>(FSI_BIND_EVENT_FN(GuiInterface::OnMouseMoveEvent));
}

bool GuiInterface::OnMouseScrolled(fsicore::MouseScrolledEvent& e)
{
	Graphic3d_Vec2i pos;
	pos.SetValues(int(e.GetXOffset()), int(e.GetYOffset()));

	occtLayer->UpdateZoom(Aspect_ScrollDelta(pos, int(e.GetYOffset() * 8.0)));
	return false;
}

bool GuiInterface::OnMouseMoveEvent(fsicore::MouseMovedEvent& e)
{
	Graphic3d_Vec2i pos;
	pos.SetValues(int(e.GetX()), int(e.GetY()));

	occtLayer->UpdateMousePosition(pos, occtLayer->PressedMouseButtons(), occtLayer->LastMouseFlags(), false);

	return false;
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

void GuiInterface::AddShape(const TopoDS_Shape& shape)
{
	m_shapes.push_back(shape);
}

void GuiInterface::ProcessShape()
{
	/*
	if (occtLayer->GetContext().IsNull())
	{
		return;
	}
	
	occtLayer->GetView()->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_WIREFRAME);

	gp_Ax2 anAxis;
	anAxis.SetLocation(gp_Pnt(100.0, 100.0, 0.0));
	AddShape(BRepPrimAPI_MakeBox(50, 100, 20));
	AddShape(BRepPrimAPI_MakeCone(anAxis, 25, 0, 50));

	if (pScene)
	{
		m_objects = pScene->getObjects();

		for (auto& o : m_objects)
		{
			auto pGeomObj = std::dynamic_pointer_cast<fsi::GeometryObject>(o);
			if (pGeomObj)
			{
				if (pGeomObj->m_shape)
				{
					AddShape(*pGeomObj->m_shape);
				}
			}
		}
	}
	
	Standard_Real moveX = 0.0;
	anAxis.SetLocation(gp_Pnt(0.0, 0.0, 0.0));
	for (auto sh : m_shapes)
	{
		
		Handle(AIS_Shape) shape = new AIS_Shape(sh);
		occtLayer->GetContext()->Display(shape, AIS_Shaded, 0, false);
		anAxis.SetLocation(gp_Pnt(moveX + 50.0, 125.0, 0.0));

		// Adjust selection style.
		oc::AdjustSelectionStyle(occtLayer->GetContext());

		// Activate selection modes.
		occtLayer->GetContext()->Activate(4, true); // faces
		occtLayer->GetContext()->Activate(2, true); // edges
		moveX += 20.0;
	}
	*/
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