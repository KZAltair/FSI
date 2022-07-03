#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

namespace fsicore
{
	class FSI_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	//To be defined at client side
	Application* CreateApplication();
}

