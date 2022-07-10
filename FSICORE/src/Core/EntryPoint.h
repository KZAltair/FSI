#pragma once
#include "Core/Log.h"
#include "Core/Application.h"

#ifdef FSI_PLATFORM_WINDOWS

//#define __CRTDBG_MAP_ALLOC
//#include <crtdbg.h>


extern fsicore::Application* fsicore::CreateApplication();

int main(int argc, char** argv)
{
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	//_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
	//_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	//_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
	//_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	//_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);

	fsicore::Log::Init();
	FSI_CORE_WARN("FSI Core Log initialized.");
	int a = 5;
	FSI_INFO("Hello! Var={0}", a);
	auto app = fsicore::CreateApplication();
	app->Run();
	delete app;

	//_CrtDumpMemoryLeaks();
	return 0;
}
#else
#error FSI Viewer only supports Windows for now!
#endif // DEBUG

