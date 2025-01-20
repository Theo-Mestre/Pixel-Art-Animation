-- premake5.lua
ProjectName = "Sandbox"
CoreName = "Core"
AppName = "App"

workspace (ProjectName)
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject (AppName)

   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "UI"
	include "SFMLUI/Build-SFMLUI.lua"
group "Core"
	include "Core/Build-Core.lua"
group ""
include (AppName .. "/Build-" .. AppName ..".lua")