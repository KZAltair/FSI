#include "Core/EntryPoint.h"
#include "Viewport.h"
#include "GuiInterface.h"

class Sandbox : public fsicore::Application
{
public:
	Sandbox()
	{
		PushLayer(new Occt());
		PushLayer(new GuiInterface());
	}
	~Sandbox()
	{

	}

};

fsicore::Application* fsicore::CreateApplication()
{
	return new Sandbox();
}