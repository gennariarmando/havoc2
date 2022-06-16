newoption {
	trigger     = "with-glad",
	description = "Build glad library"
}

workspace "havoc2"
	configurations { "Release", "Debug" }
	location "project_files"
   	startproject "havoc2"
    platforms { "Win64" }
	architecture "x64"

if(_OPTIONS["with-glad"]) then
project "glad"
	location "project_files/glad"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
	targetdir "vendor/glad/lib/"
	objdir ("vendor/glad/obj")

    files {
		"vendor/glad/**.*",
    }

    includedirs {
        "vendor/glad"
    }
    
    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
end
   
project "havoc2"
	location "project_files/havoc2"

	files {
		"source/**.*",
		"vendor/pugixml/src/*.*"
	}
	
	includedirs { 
		"source/**",
		"vendor/*",
	}
	
	libdirs { 
		"vendor/*/lib",
	}
	
	links {
		"glfw3",
		"glad",
	}
	
	kind "WindowedApp"
	language "C++"
	targetdir "output/exe/GTA2/"
	objdir ("output/obj")
	targetextension ".exe"
	characterset ("MBCS")
	linkoptions  { "/SAFESEH:NO", "/ENTRY:mainCRTStartup", "/NODEFAULTLIB:LIBCMT.lib" }
	buildoptions { "-std:c++latest", "/Zc:threadSafeInit-", "/Zc:strictStrings" }
	symbols "on"

	filter "configurations:Debug"		
		targetname "havoc2"
		defines { "DEBUG" }
		staticruntime "on"
		debugdir "output/exe/GTA2/"
		debugcommand "output/exe/GTA2/havoc2.exe"

	filter "configurations:Release"
		targetname "havoc2"
		defines { "NDEBUG" }
		optimize "On"
		staticruntime "on"
		debugdir "output/exe/GTA2/"
		debugcommand "output/exe/GTA2/havoc2.exe"