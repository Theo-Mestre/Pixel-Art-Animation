project (AppName)
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"

   files { "Source/**.h", "Source/**.cpp", "Source/**.inl"}

   defines { "SFML_STATIC", "NOMINMAX" }

   includedirs
   {
      "Source",
	  "../Core/Source",
	  "../SFMLUI/Source",
      "../Vendor/include"
   }

   libdirs
   {
	  "../Vendor/lib"
   }

   links
   {
	  "Core",
	  "SFMLUI",
      "flac",
      "freetype",
      "ogg",
      "openal32",
      "vorbis",
      "vorbisenc",
      "vorbisfile",
      "opengl32.lib", 
      "winmm.lib",
      "gdi32.lib"
   }


   targetdir ("../binaries/" .. OutputDir .. "/" .. AppName)
   objdir ("../binaries/intermediates/" .. OutputDir .. "/" .. AppName)
   debugdir "../"

   filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"
       links { "sfml-graphics-s-d", "sfml-window-s-d", "sfml-system-s-d", "sfml-audio-s-d" }

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"
       links { "sfml-graphics-s", "sfml-window-s", "sfml-system-s", "sfml-audio-s" }

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"
       links { "sfml-graphics-s", "sfml-window-s", "sfml-system-s", "sfml-audio-s" }