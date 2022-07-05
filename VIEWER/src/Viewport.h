#pragma once
#include "PrimaryIncludes.h"
#include <TopoDS_Shape.hxx>


class Occt : virtual public fsicore::OcctRenderLayer
{
public:
	Occt();
	void OnUpdate() override;

	virtual void OnImGuiRender() override;
	virtual void OnImGuiDrawWidget() override;
	void AddShape(const TopoDS_Shape& shape);
	void ProcessShape();
	//void OnEvent(fsicore::Event& event) override;

private:
	fsicore::OcctRenderLayer* occtLayer;
	bool m_ViewportFocused = false, m_ViewportHovered = false;
	std::vector<TopoDS_Shape> m_shapes; //!< Shapes to visualize.
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	glm::vec2 m_ViewportBounds[2];
};