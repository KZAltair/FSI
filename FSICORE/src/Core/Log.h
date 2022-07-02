#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"

//Deal with format
#include "spdlog/fmt/ostr.h"

namespace fsicore
{
	class FSI_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return sp_CoreLogger;  }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return sp_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> sp_CoreLogger;
		static std::shared_ptr<spdlog::logger> sp_ClientLogger;
	};
}


//Core log helper macros
#define FSI_CORE_ERROR(...)		::fsicore::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FSI_CORE_WARN(...)		::fsicore::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FSI_CORE_INFO(...)		::fsicore::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FSI_CORE_TRACE(...)		::fsicore::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FSI_CORE_FATAL(...)		::fsicore::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client log helper macros
#define FSI_ERROR(...)			::fsicore::Log::GetClientLogger()->error(__VA_ARGS__)
#define FSI_WARN(...)			::fsicore::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FSI_INFO(...)			::fsicore::Log::GetClientLogger()->info(__VA_ARGS__)
#define FSI_TRACE(...)			::fsicore::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FSI_FATAL(...)			::fsicore::Log::GetClientLogger()->fatal(__VA_ARGS__)