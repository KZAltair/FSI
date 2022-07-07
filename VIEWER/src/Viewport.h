#pragma once
#include "PrimaryIncludes.h"
#include <AIS_Shape.hxx>
#include <AIS_InteractiveContext.hxx>

class Occt : virtual public fsicore::OcctRenderLayer
{
public:
	Occt();
	void OnUpdate() override;

	virtual void OnImGuiRender() override;
	virtual void OnImGuiDrawWidget() override;
	void ProcessShape(Handle(AIS_InteractiveContext) h_aisInteractor);
	void GenerateObjects(Handle(AIS_InteractiveContext) h_aisInteractor);
	virtual void OnEvent(fsicore::Event& e) override;
	bool OnMouseScrolled(fsicore::MouseScrolledEvent& e);
	bool OnMouseMoveEvent(fsicore::MouseMovedEvent& e);
	void set_mesh_load_callback(const std::function<void(const std::string&)>& callback)
	{
		fsiLoadedMesh = callback;
	}

private:
	Graphic3d_Vec2i pos;
	Handle(AIS_InteractiveContext) h_aisInteractor;
	
	fsicore::OcctRenderLayer* occtLayer;
	bool m_ViewportFocused = false, m_ViewportHovered = false;
	fsicore::SceneContainer* p_ModelsContainer;
	std::vector<fsi::ObjectPtr> m_objects;
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	glm::vec2 m_ViewportBounds[2];

	std::function<void(const std::string&)> fsiLoadedMesh;

	bool testShapesLoading = true;
};