#include <FSIClient.h>
class ExampleLayer : public fsicore::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		FSI_INFO("ExampleLayer::Update");
	}

	void OnEvent(fsicore::Event& event) override
	{
		FSI_TRACE("{0}", event);
	}

};

class Sandbox : public fsicore::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new fsicore::ImGuiLayer());
	}
	~Sandbox()
	{

	}

};

fsicore::Application* fsicore::CreateApplication()
{
	return new Sandbox();
}