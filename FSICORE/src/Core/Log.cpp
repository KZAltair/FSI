#include "fsipch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace fsicore
{
	std::shared_ptr<spdlog::logger> Log::sp_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::sp_ClientLogger;
	
	void Log::Init()
	{
		//Pattern timestamp / name of logger / actual message
		spdlog::set_pattern("%^[%T] %n: %v%$");

		sp_CoreLogger = spdlog::stdout_color_mt("FSI");
		sp_CoreLogger->set_level(spdlog::level::trace);
		sp_ClientLogger = spdlog::stdout_color_mt("APP");
		sp_ClientLogger->set_level(spdlog::level::trace);
	}
}
