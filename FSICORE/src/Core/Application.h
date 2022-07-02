#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace fsicore
{
	class FSI_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	//To be defined at client side
	Application* CreateApplication();
}

