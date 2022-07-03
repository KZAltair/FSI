workspace "FSI"
	architecture "x64"

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
		cppdialect "C++17"
		staticruntime "off"
		systemversion "latest"

		defines
		{
			"FSI_PLATFORM_WINDOWS",
			"FSI_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/VIEWER")
		}

	filter "configurations:Debug"
		defines "FSI_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "FSI_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "FSI_DIST"
		buildoptions "/MD"
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
		systemversion "latest"

		defines
		{
			"FSI_PLATFORM_WINDOWS"
		}


	filter "configurations:Debug"
		defines "FSI_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "FSI_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "FSI_DIST"
		buildoptions "/MD"
		optimize "On"