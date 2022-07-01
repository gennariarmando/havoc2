workspace "havoc2"
	configurations { "Release", "Debug" }
	location "project_files"
   	startproject "havoc2"
    platforms { "x86", --[["x64"--]] }
	
	filter "platforms:x86"
		architecture "x86"
		system "windows"
	
	filter "platforms:x64"
		architecture "x64"
		system "windows"
   
project "havoc2"
	location "project_files/havoc2"

	files {
		"source/**.*",
		"vendor/pugixml/src/*.*",
		"vendor/glad/src/*.*",
		"vendor/glad/include/**.*",
	}
	
	includedirs { 
		"source/**",
		"vendor/**",
	}
	
	filter "platforms:x86"	
		libdirs { "vendor/*/lib/x86/" }
		links { "glfw3", "binkw32" }

	filter "platforms:x64"	
		libdirs { "vendor/*/lib/x64/" }
		links { "glfw3" }

	filter {}
	
	kind "WindowedApp"
	language "C++"
	targetdir "output/exe/GTA2/"
	objdir ("output/obj")
	targetextension ".exe"
	characterset ("MBCS")
	linkoptions  { "/SAFESEH:NO", "/ENTRY:mainCRTStartup" }
	buildoptions { "-std:c++latest", "/Zc:threadSafeInit-", "/Zc:strictStrings" }
	defines { "_CRT_SECURE_NO_WARNINGS", "_CRT_NONSTDC_NO_DEPRECATE", "_WIN32" }
	disablewarnings { "4244", "4800", "4305", "4073", "4838", "4996", "4221", "4430", "26812", "26495", "6031" }
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
		