#pragma once

#ifdef FSI_PLATFORM_WINDOWS
	#ifdef FSI_BUILD_DLL
		#define FSI_API __declspec(dllexport)
	#else
		#define FSI_API __declspec(dllimport)
	#endif
#else
	#error FSI Viewer only supports Windows for now!
#endif

#define BIT(x) (1 << x)
