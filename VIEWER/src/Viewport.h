#pragma once
#include "PrimaryIncludes.h"

class Occt : public fsicore::OcctRenderLayer
{
public:
	Occt();
	void OnUpdate() override;

	virtual void OnImGuiRender() override;
	virtual void OnImGuiDrawWidget() override;
	void OnEvent(fsicore::Event& event) override;
};