#include <FSIClient.h>

#include "imgui/imgui.h"

class ExampleLayer : public fsicore::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		if (fsicore::Input::IsKeyPressed(FSI_KEY_TAB))
			FSI_TRACE("Tab key is pressed (poll)!");
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(fsicore::Event& event) override
	{
		if (event.GetEventType() == fsicore::EventType::KeyPressed)
		{
			fsicore::KeyPressedEvent& e = (fsicore::KeyPressedEvent&)event;
			if (e.GetKeyCode() == FSI_KEY_TAB)
				FSI_TRACE("Tab key is pressed (event)!");
			FSI_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

};

class Sandbox : public fsicore::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{

	}

};

fsicore::Application* fsicore::CreateApplication()
{
	return new Sandbox();
}