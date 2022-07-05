#include "Viewport.h"
#include <BRepCheck_Analyzer.hxx>
#include <AIS_Shape.hxx>
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
	ProcessShape();
	occtLayer->GetView()->MustBeResized();
	occtLayer->GetOcctWindow()->Map();
}
void Occt::OnUpdate()
{
	FlushViewEvents(occtLayer->GetContext(), occtLayer->GetView(), true);
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

void Occt::AddShape(const TopoDS_Shape& shape)
{
	m_shapes.push_back(shape);
}

void Occt::ProcessShape()
{
	if (occtLayer->GetContext().IsNull())
	{
		return;
	}

	occtLayer->GetView()->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_WIREFRAME);

	gp_Ax2 anAxis;
	anAxis.SetLocation(gp_Pnt(100.0, 100.0, 0.0));
	AddShape(BRepPrimAPI_MakeBox(50, 100, 20));
	AddShape(BRepPrimAPI_MakeCone(anAxis, 25, 0, 50));

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
}
