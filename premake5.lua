workspace "FSI"
	architecture "x64"
	startproject "VIEWER"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "FSICORE/3rdparty/GLFW/include"
IncludeDir["Glad"] = "FSICORE/3rdparty/Glad/include"
IncludeDir["ImGui"] = "FSICORE/3rdparty/imgui"

-- This is for including another premake for GLFW lua script
include "FSICORE/3rdparty/GLFW"
include "FSICORE/3rdparty/Glad"
include "FSICORE/3rdparty/imgui"

project "FSICORE"
	location "FSICORE"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}") 
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "fsipch.h"
	pchsource "FSICORE/src/fsipch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/3rdparty/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"opengl32.lib",
		"IMGUI"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"FSI_PLATFORM_WINDOWS",
			"FSI_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			--("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/VIEWER")
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/VIEWER/\"")
		}

	filter "configurations:Debug"
		defines "FSI_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "FSI_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "FSI_DIST"
		runtime "Release"
		optimize "on"

project "VIEWER"
	location "VIEWER"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"FSICORE/src",
		"FSICORE/3rdparty",
	}

	links
	{
		"FSICORE"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"FSI_PLATFORM_WINDOWS"
		}


	filter "configurations:Debug"
		defines "FSI_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "FSI_RELEASE"
		runtime "Release"
		optimize "on"
		symbols "off"

	filter "configurations:Dist"
		defines "FSI_DIST"
		runtime "Release"
		optimize "on"
		symbols "off"