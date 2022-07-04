#pragma once
#include "Core/Log.h"
#include "Core/Application.h"

#ifdef FSI_PLATFORM_WINDOWS

extern fsicore::Application* fsicore::CreateApplication();

int main(int argc, char** argv)
{
	fsicore::Log::Init();
	FSI_CORE_WARN("FSI Core Log initialized.");
	int a = 5;
	FSI_INFO("Hello! Var={0}", a);
	auto app = fsicore::CreateApplication();
	app->Run();
	delete app;
	return 0;
}
#else
#error FSI Viewer only supports Windows for now!
#endif // DEBUG

