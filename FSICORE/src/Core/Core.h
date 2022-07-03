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

#ifdef FSI_ENABLE_ASSERTS
#define HZ_ASSERT(x, ...) { if(!(x)) { FSI_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define HZ_CORE_ASSERT(x, ...) { if(!(x)) { FSI_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define FSI_ASSERT(x, ...)
#define FSI_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define FSI_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
