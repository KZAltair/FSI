#include <FSIClient.h>

class Sandbox : public fsicore::Application
{
public:
	Sandbox()
	{

	}
	~Sandbox()
	{

	}

};

fsicore::Application* fsicore::CreateApplication()
{
	return new Sandbox();
}