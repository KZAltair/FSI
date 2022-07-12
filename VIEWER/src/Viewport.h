#pragma once
#include "PrimaryIncludes.h"
#include <AIS_Shape.hxx>
#include <AIS_InteractiveContext.hxx>
#include <Image_PixMap.hxx>
#include <OpenGl_Texture.hxx>

class Occt : virtual public fsicore::OcctRenderLayer
{
public:
	Occt();
	void OnUpdate() override;
	virtual void OnDetach() override;
	virtual void OnImGuiRender() override;
	virtual void OnImGuiDrawWidget() override;
	void ProcessShape(Handle(AIS_InteractiveContext) h_aisInteractor);
	void GenerateObjects(Handle(AIS_InteractiveContext) h_ais);
	virtual void OnEvent(fsicore::Event& e) override;
	bool OnMouseScrolled(fsicore::MouseScrolledEvent& e);
	bool OnMouseMoveEvent(fsicore::MouseMovedEvent& e);
	bool OnMousePressedEvent(fsicore::MouseButtonPressedEvent& e);
	bool OnMouseReleasedEvent(fsicore::MouseButtonReleasedEvent& e);
	bool OnObjectShowHide(fsicore::OcctShowHideEvent& e);
	bool OnShowHideAllObjects(fsicore::OcctShowHideSignleObjectEvent& e);
	bool OnEmptyScene(fsicore::OcctEmptySceneEvent& e);
	bool OnLoadScene(fsicore::OcctLoadSceneEvent& e);
	void set_mesh_load_callback(const std::function<void(const std::string&)>& callback)
	{
		fsiLoadedMesh = callback;
	}

private:
	fsicore::Application* app;
	Graphic3d_Vec2i pos;
	Handle(AIS_InteractiveContext) h_aisInteractor;
	std::vector<Handle(AIS_Shape)> shHandle;
	fsicore::OcctRenderLayer* occtLayer;
	bool m_ViewportFocused = false, m_ViewportHovered = false;
	fsicore::SceneContainer* p_ModelsContainer;
	std::vector<fsi::ObjectPtr> m_objects;
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	glm::vec2 m_ViewportBounds[2];

	std::function<void(const std::string&)> fsiLoadedMesh;

	bool testShapesLoading = true;

	Handle(OpenGl_Texture) t;
	Image_PixMap anImage;
};