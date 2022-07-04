#pragma once
#include "PrimaryIncludes.h"

class GuiInterface : public fsicore::Layer
{
public:
	GuiInterface();
	void OnUpdate() override;
	virtual void OnImGuiRender() override;
	virtual void OnImGuiDrawWidget() override;
	void OnEvent(fsicore::Event& event) override;
};