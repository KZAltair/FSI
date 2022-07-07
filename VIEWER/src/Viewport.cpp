#include "Viewport.h"
#include <AIS_Shape.hxx>
#include <BRepCheck_Analyzer.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <AIS_InteractiveContext.hxx>


#include "Core/Application.h"

namespace oc {
	//! Adjust the style of local selection.
	//! \param[in] context the AIS context.
	void AdjustSelectionStyle(const Handle(AIS_InteractiveContext)& context)
	{
		// Initialize style for sub-shape selection.
		Handle(Prs3d_Drawer) selDrawer = new Prs3d_Drawer;
		//
		selDrawer->SetLink(context->DefaultDrawer());
		selDrawer->SetFaceBoundaryDraw(true);
		selDrawer->SetDisplayMode(1); // Shaded
		selDrawer->SetTransparency(0.5f);
		selDrawer->SetZLayer(Graphic3d_ZLayerId_Topmost);
		selDrawer->SetColor(Quantity_NOC_GOLD);
		selDrawer->SetBasicFillAreaAspect(new Graphic3d_AspectFillArea3d());

		// Adjust fill area aspect.
		const Handle(Graphic3d_AspectFillArea3d)&
			fillArea = selDrawer->BasicFillAreaAspect();
		//
		fillArea->SetInteriorColor(Quantity_NOC_GOLD);
		fillArea->SetBackInteriorColor(Quantity_NOC_GOLD);
		//
		fillArea->ChangeFrontMaterial().SetMaterialName(Graphic3d_NOM_NEON_GNC);
		fillArea->ChangeFrontMaterial().SetTransparency(0.4f);
		fillArea->ChangeBackMaterial().SetMaterialName(Graphic3d_NOM_NEON_GNC);
		fillArea->ChangeBackMaterial().SetTransparency(0.4f);

		selDrawer->UnFreeBoundaryAspect()->SetWidth(1.0);

		// Update AIS context.
		context->SetHighlightStyle(Prs3d_TypeOfHighlight_LocalSelected, selDrawer);
	}
}

Occt::Occt()
{
	fsicore::Application* app = &fsicore::Application::Get();
	occtLayer = app->GetOcctLayer();
	p_ModelsContainer = app->GetSceneContainer();
	occtLayer->GetView()->MustBeResized();
	occtLayer->GetOcctWindow()->Map();
}
void Occt::OnUpdate()
{
	if (m_ViewportFocused && m_ViewportHovered)
	{
		if (fsicore::Input::IsMouseButtonPressed(fsicore::Mouse::ButtonLeft))
		{
			//TODO: fix rotation position
			Graphic3d_Vec2i pos = {};
			pos.SetValues(int(fsicore::Input::GetMouseX()), int(fsicore::Input::GetMouseY()));
			occtLayer->GetView()->StartRotation(pos.x(), pos.y());

		}

		if (fsicore::Input::IsKeyPressed(FSI_KEY_C))
		{
			if (testShapesLoading)
			{
				if (h_aisInteractor.IsNull())
				{
					if (!occtLayer->GetViewer().IsNull())
					{
						h_aisInteractor = new AIS_InteractiveContext(occtLayer->GetViewer());
						GenerateObjects(h_aisInteractor);
						testShapesLoading = false;
					}
				}
				else
				{
					GenerateObjects(h_aisInteractor);
					testShapesLoading = false;
				}
				
			}
		}
		if (fsicore::Input::IsKeyPressed(FSI_KEY_D))
		{
			if (!testShapesLoading)
			{
				if (!h_aisInteractor.IsNull())
				{
					/*
					for (auto& handle : shapesHandle)
					{
						h_aisInteractor->Remove(handle, false);
						//GetContext()->SelectionManager()->Remove(handle);
						handle.Nullify();
					}
					for (auto& sh : shapes)
					{
						sh.Nullify();
					}
					*/
					h_aisInteractor->RemoveAll(false);
					h_aisInteractor.Nullify();
				}
				testShapesLoading = true;
			}
		}
	}
	
	if (p_ModelsContainer->IsLoading())
	{
		if (h_aisInteractor.IsNull())
		{
			if (!occtLayer->GetViewer().IsNull())
			{
				h_aisInteractor = new AIS_InteractiveContext(occtLayer->GetViewer());
				ProcessShape(h_aisInteractor);
				occtLayer->GetView()->FitAll();
				p_ModelsContainer->SetLoadingStatus(false);
			}
		}
		else
		{
			ProcessShape(h_aisInteractor);
			occtLayer->GetView()->FitAll();
			p_ModelsContainer->SetLoadingStatus(false);
		}
		
	}
	if (p_ModelsContainer->IsRemoved())
	{
		if (!h_aisInteractor.IsNull())
		{
			/*
			for (auto& handle : shapesHandle)
			{
				h_aisInteractor->Remove(handle, false);
				h_aisInteractor->SelectionManager()->Remove(handle);
				handle.Nullify();
				p_ModelsContainer->SetRemovedStatus(false);
			}
			*/
			h_aisInteractor->RemoveAll(false);
			h_aisInteractor.Nullify();
		}
		
	}
	if (!h_aisInteractor.IsNull())
	{
		FlushViewEvents(h_aisInteractor, occtLayer->GetView(), true);
	}
	
}

void Occt::OnImGuiRender()
{
	// Create the docking environment
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoBackground;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
	ImGui::PopStyleVar(3);

	ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");

	ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::End();

	ImGui::Begin("Perspective");
	auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto viewportOffset = ImGui::GetWindowPos();
	m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
	m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();
	fsicore::Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

	uint64_t textureID = occtLayer->GetTexID();
	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	ImGui::End();
}

void Occt::OnImGuiDrawWidget()
{

}

void Occt::ProcessShape(Handle(AIS_InteractiveContext) h_aisInteractor)
{
	std::vector<Handle(AIS_Shape)> shapesHandle;
	std::vector<TopoDS_Shape> shapes;
	if (h_aisInteractor.IsNull())
	{
		return;
	}

	occtLayer->GetView()->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_WIREFRAME);

	gp_Ax2 anAxis;
	anAxis.SetLocation(gp_Pnt(100.0, 100.0, 0.0));
	//AddShape(BRepPrimAPI_MakeBox(50, 100, 20));
	//AddShape(BRepPrimAPI_MakeCone(anAxis, 25, 0, 50));

	Standard_Real moveX = 0.0;
	anAxis.SetLocation(gp_Pnt(0.0, 0.0, 0.0));
	
	m_objects = p_ModelsContainer->GetScenePtr()->getObjects();

	for (size_t i = 0; i < m_objects.size(); ++i)
	{
		auto pGeomObj = std::dynamic_pointer_cast<fsi::GeometryObject>(m_objects.at(i));
		if (pGeomObj)
		{
			if (pGeomObj->m_shape)
			{

				shapesHandle.push_back(new AIS_Shape(*pGeomObj->m_shape));
				h_aisInteractor->Display(shapesHandle.back(), AIS_Shaded, 0, false);

				// Adjust selection style.
				oc::AdjustSelectionStyle(h_aisInteractor);

				// Activate selection modes.
				h_aisInteractor->Activate(4, true); // faces
				h_aisInteractor->Activate(2, true); // edges
			}
		}
	}
}

void Occt::GenerateObjects(Handle(AIS_InteractiveContext) h_aisInteractor)
{
	//Add shapes
	std::vector<Handle(AIS_Shape)> shapesHandle;
	std::vector<TopoDS_Shape> shapes;
	occtLayer->GetView()->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_WIREFRAME);
	for (size_t i = 0; i < 2; ++i)
	{
		gp_Pnt p;
		Standard_Real xCoord = (Standard_Real)(i * 150);
		p.SetX(xCoord);
		shapes.push_back(BRepPrimAPI_MakeBox(p, 50, 100, 20));
		
	}

	for (size_t i = 0; i < shapes.size(); ++i)
	{
		shapesHandle.push_back(new AIS_Shape(shapes.at(i)));
		h_aisInteractor->Display(shapesHandle.at(i), AIS_Shaded, 0, false);

		oc::AdjustSelectionStyle(h_aisInteractor);

		// Activate selection modes.
		h_aisInteractor->Activate(4, true); // faces
		h_aisInteractor->Activate(2, true); // edges
	}
	//testShapes.push_back(BRepPrimAPI_MakeBox(50, 100, 20));
	//gp_Ax2 anAxis;
	//anAxis.SetLocation(gp_Pnt(100.0, 100.0, 0.0));
	//testShapes.push_back(BRepPrimAPI_MakeCone(anAxis, 25, 0, 50));
	//Process shapes
	
	// Adjust selection style.
	oc::AdjustSelectionStyle(h_aisInteractor);

	// Activate selection modes.
	h_aisInteractor->Activate(4, true); // faces
	h_aisInteractor->Activate(2, true); // edges
	/*
	float moveX = 0.0f;
	for (size_t i = 0; i < testShapes.size(); ++i)
	{

		shapeHandles.push_back(new AIS_Shape(testShapes.at(i)));
		occtLayer->GetContext()->Display(shapeHandles.at(i), AIS_Shaded, 0, false);
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

void Occt::OnEvent(fsicore::Event& e)
{
	fsicore::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<fsicore::MouseScrolledEvent>(FSI_BIND_EVENT_FN(Occt::OnMouseScrolled));
	dispatcher.Dispatch<fsicore::MouseMovedEvent>(FSI_BIND_EVENT_FN(Occt::OnMouseMoveEvent));
}

bool Occt::OnMouseScrolled(fsicore::MouseScrolledEvent& e)
{

	UpdateZoom(Aspect_ScrollDelta(pos, int(e.GetYOffset() * 8.0)));
	return false;
}

bool Occt::OnMouseMoveEvent(fsicore::MouseMovedEvent& e)
{
	
	pos.SetValues(int(e.GetX()), int(e.GetY()));
	if (fsicore::Input::IsMouseButtonPressed(fsicore::Mouse::ButtonLeft))
	{
		occtLayer->GetView()->Rotation(pos.x(), pos.y());
	}

	UpdateMousePosition(pos, occtLayer->PressedMouseButtons(), occtLayer->LastMouseFlags(), false);
	return false;
}
