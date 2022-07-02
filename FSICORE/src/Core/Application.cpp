#include "fsipch.h"
#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "Log.h"

namespace fsicore
{
	Application::Application()
	{
	}
	Application::~Application()
	{
	}
	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		FSI_TRACE(e);
		while (true);
	}
}