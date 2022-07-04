#include "Viewport.h"

Occt::Occt()
{

}
void Occt::OnUpdate()
{

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

	float mSize[2] = { 0,0 };

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	mSize[0] = viewportPanelSize.x;
	mSize[1] = viewportPanelSize.y;

	// add rendered texture to ImGUI scene window
	uint64_t textureID = GetTexID();
	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ mSize[0], mSize[1] }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	ImGui::End();
}

void Occt::OnImGuiDrawWidget()
{

}

void Occt::OnEvent(fsicore::Event& event)
{
}