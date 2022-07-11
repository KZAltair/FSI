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
	: Layer("Viewport")
{
	//Init
	h_aisInteractor.Nullify();

	fsicore::Application* app = &fsicore::Application::Get();
	occtLayer = app->GetOcctLayer();
	p_ModelsContainer = app->GetSceneContainer();
	h_aisInteractor = new AIS_InteractiveContext(occtLayer->GetViewer());

	occtLayer->GetView()->MustBeResized();
	occtLayer->GetOcctWindow()->Map();

	//occtLayer->GetView()->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_WIREFRAME);
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
					h_aisInteractor->Deactivate();
					h_aisInteractor->RemoveAll(true);
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
			h_aisInteractor->Deactivate();
			
			h_aisInteractor->RemoveAll(false);

			h_aisInteractor.Nullify();
		}
		
	}
	if (!h_aisInteractor.IsNull())
	{
		occtLayer->FlushViewEvents(h_aisInteractor, occtLayer->GetView(), true);
	}
	
}

void Occt::OnDetach()
{
	if (!t.IsNull())
	{
		t->Release(occtLayer->GetOcctGLContext().get());
	}
	if (h_aisInteractor.IsNull())
	{
		h_aisInteractor.Nullify();
	}
	for (auto& sh : shHandle)
	{
		if (!sh.IsNull())
		{
			sh.Nullify();
		}
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
	
	//p.SetValues(int(fsicore::Input::GetMousePosition().first - mPos.x), int(fsicore::Input::GetMousePosition().second - mPos.y));
	
	auto viewportOffset = ImGui::GetWindowPos();
	m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
	m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();
	fsicore::Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
	

	if (!t.IsNull())
	{
		t->Release(occtLayer->GetOcctGLContext().get());
	}
	t = new OpenGl_Texture();
	anImage.Clear();

	//Make this not magic numbers !!!!!
	if (!occtLayer->GetView()->ToPixMap(anImage, 1920, 1080))
	{
		FSI_ERROR("View dump failed");
	}
	
	t->Init(occtLayer->GetOcctGLContext(), anImage, Graphic3d_TypeOfTexture_2D, true);
	
	uint64_t textureID = t->TextureId();
	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	ImGui::End();
}

void Occt::OnImGuiDrawWidget()
{

}

void Occt::ProcessShape(Handle(AIS_InteractiveContext) h_aisModel)
{
	
	std::vector<TopoDS_Shape> shapes;
	
	
	m_objects = p_ModelsContainer->GetScenePtr()->getObjects();

	for (size_t i = 0; i < m_objects.size(); ++i)
	{
		auto pGeomObj = std::dynamic_pointer_cast<fsi::GeometryObject>(m_objects.at(i));
		if (pGeomObj)
		{
			if (pGeomObj->m_shape)
			{

				shHandle.push_back(new AIS_Shape(*pGeomObj->m_shape));
				h_aisModel->Display(shHandle.back(), AIS_Shaded, 0, false);

				// Adjust selection style.
				oc::AdjustSelectionStyle(h_aisModel);

				// Activate selection modes.
				h_aisModel->Activate(4, true); // faces
				h_aisModel->Activate(2, true); // edges
			}
		}
	}
}

void Occt::GenerateObjects(Handle(AIS_InteractiveContext) h_ais)
{
	//!!!!!Ciritial importance. Can't name handles with the same name. They should be unique
	Handle(AIS_Shape) shapeHandle;
	if (!shapeHandle.IsNull())
	{
		FSI_INFO("Handle was not released!");
		return;
	}
	std::vector<TopoDS_Shape> shapes;
	
	for (size_t i = 0; i < 2; ++i)
	{
		gp_Pnt p;
		Standard_Real xCoord = (Standard_Real)(i * 150);
		p.SetX(xCoord);
		shapes.push_back(BRepPrimAPI_MakeBox(p, 50, 100, 20));
		
	}

	for (size_t i = 0; i < shapes.size(); ++i)
	{
		shapeHandle = new AIS_Shape(shapes.at(i));
		h_ais->Display(shapeHandle, AIS_Shaded, 0, false);

		oc::AdjustSelectionStyle(h_ais);

		// Activate selection modes.
		h_ais->Activate(4, true); // faces
		h_ais->Activate(2, true); // edges
	}
	//testShapes.push_back(BRepPrimAPI_MakeBox(50, 100, 20));
	//gp_Ax2 anAxis;
	//anAxis.SetLocation(gp_Pnt(100.0, 100.0, 0.0));
	//testShapes.push_back(BRepPrimAPI_MakeCone(anAxis, 25, 0, 50));
	//Process shapes
	
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
	dispatcher.Dispatch<fsicore::MouseButtonPressedEvent>(FSI_BIND_EVENT_FN(Occt::OnMousePressedEvent));
	dispatcher.Dispatch<fsicore::MouseButtonReleasedEvent>(FSI_BIND_EVENT_FN(Occt::OnMouseReleasedEvent));
	dispatcher.Dispatch<fsicore::OcctShowHideEvent>(FSI_BIND_EVENT_FN(Occt::OnObjectShowHide));
	dispatcher.Dispatch<fsicore::OcctShowHideSignleObjectEvent>(FSI_BIND_EVENT_FN(Occt::OnShowHideAllObjects));
	
}

bool Occt::OnMouseScrolled(fsicore::MouseScrolledEvent& e)
{
	occtLayer->UpdateZoom(Aspect_ScrollDelta(pos, int(e.GetYOffset() * 8.0)));
	return false;
}

bool Occt::OnMouseMoveEvent(fsicore::MouseMovedEvent& e)
{
	pos.SetValues(int(e.GetX()), int(e.GetY()));
	occtLayer->UpdateMousePosition(pos, occtLayer->PressedMouseButtons(), occtLayer->LastMouseFlags(), false);
	return false;
}

bool Occt::OnMousePressedEvent(fsicore::MouseButtonPressedEvent& e)
{
	Aspect_VKeyMouse button = Aspect_VKeyMouse_NONE;
	switch (e.GetMouseButton())
	{
		case 0:   
		{
			button = Aspect_VKeyMouse_LeftButton;
		}break;
		case 1:  
		{	
			button = Aspect_VKeyMouse_RightButton;
		}break;
		case 2: 
		{
			button = Aspect_VKeyMouse_MiddleButton;
		}break;
		default: button = Aspect_VKeyMouse_NONE;
	}
	Aspect_VKeyFlags aFlags = Aspect_VKeyFlags_NONE;
	if ((e.GetMouseFalgs() & 0x0001) != 0)
	{
		aFlags |= Aspect_VKeyFlags_SHIFT;
	}
	if ((e.GetMouseFalgs() & 0x0002) != 0)
	{
		aFlags |= Aspect_VKeyFlags_CTRL;
	}
	if ((e.GetMouseFalgs() & 0x0004) != 0)
	{
		aFlags |= Aspect_VKeyFlags_ALT;
	}
	if ((e.GetMouseFalgs() & 0x0008) != 0)
	{
		aFlags |= Aspect_VKeyFlags_META;
	}
	occtLayer->PressMouseButton(pos, button, aFlags, false);
	return false;
}

bool Occt::OnMouseReleasedEvent(fsicore::MouseButtonReleasedEvent& e)
{
	Aspect_VKeyMouse button = Aspect_VKeyMouse_NONE;
	switch (e.GetMouseButton())
	{
	case 0:
	{
		button = Aspect_VKeyMouse_LeftButton;
	}break;
	case 1:
	{
		button = Aspect_VKeyMouse_RightButton;
	}break;
	case 2:
	{
		button = Aspect_VKeyMouse_MiddleButton;
	}break;
	default: button = Aspect_VKeyMouse_NONE;
	}
	Aspect_VKeyFlags aFlags = Aspect_VKeyFlags_NONE;
	if ((e.GetMouseFalgs() & 0x0001) != 0)
	{
		aFlags |= Aspect_VKeyFlags_SHIFT;
	}
	if ((e.GetMouseFalgs() & 0x0002) != 0)
	{
		aFlags |= Aspect_VKeyFlags_CTRL;
	}
	if ((e.GetMouseFalgs() & 0x0004) != 0)
	{
		aFlags |= Aspect_VKeyFlags_ALT;
	}
	if ((e.GetMouseFalgs() & 0x0008) != 0)
	{
		aFlags |= Aspect_VKeyFlags_META;
	}
	occtLayer->ReleaseMouseButton(pos, button, aFlags, false);
	return false;
}

bool Occt::OnObjectShowHide(fsicore::OcctShowHideEvent& e)
{
	FSI_INFO("Single object show/hide event in viewport called!");
	if (e.GetFlag())
	{
		h_aisInteractor->Erase(shHandle.at(p_ModelsContainer->GetAisShapeID()), false);
	}
	if (!e.GetFlag())
	{
		h_aisInteractor->Display(shHandle.at(p_ModelsContainer->GetAisShapeID()), AIS_Shaded, 0, false);
	}
	return false;
}

bool Occt::OnShowHideAllObjects(fsicore::OcctShowHideSignleObjectEvent& e)
{
	FSI_INFO("All objects hidden status in viewport called!");
	if (e.GetFlag())
	{
		for (int i = 0; i < (int)shHandle.size(); ++i)
		{
			h_aisInteractor->Erase(shHandle.at(i), false);
		}
	}
	if (!e.GetFlag())
	{
		for (int i = 0; i < (int)shHandle.size(); ++i)
		{
			h_aisInteractor->Display(shHandle.at(i), AIS_Shaded, 0, false);
		}
	}
	return false;
}
