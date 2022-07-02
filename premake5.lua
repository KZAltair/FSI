workspace "FSI"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"

project "FSICORE"
	location "FSICORE"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}") 
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "fsipch.h"
	pchsource "FSICORE/src/fsipch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/3rdparty/spdlog/include"
	}

	filter "system:Windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"FSI_PLATFORM_WINDOWS",
			"FSI_BUILD_DLL",
			"_WINDLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/VIEWER")
		}

	filter "configurations:Debug"
		defines "FSI_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "FSI_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "FSI_DIST"
		optimize "On"

project "VIEWER"
	location "VIEWER"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}") 
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"FSICORE/3rdparty/spdlog/include",
		"FSICORE/src"
	}

	links
	{
		"FSICORE"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "10.0.19041.0"

		defines
		{
			"FSI_PLATFORM_WINDOWS"
		}


	filter "configurations:Debug"
		defines "FSI_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "FSI_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "FSI_DIST"
		optimize "On"