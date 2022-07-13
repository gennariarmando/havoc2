newoption {
	trigger     = "with-glad",
	description = "Build glad library"
}

newoption {
	trigger     = "with-reactphysics3d",
	description = "Build reactphysics3d library"
}

newoption {
	trigger     = "with-pugixml",
	description = "Build pugixml library"
}

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
		
if(_OPTIONS["with-glad"]) then
project "vendor-glad"
	location "project_files/vendor/glad"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    targetname "glad"
	
	filter "platforms:x86"
		targetdir "vendor/glad/lib/x86"
		objdir ("vendor/glad/obj/x86")
	filter "platforms:x64"
		targetdir "vendor/glad/lib/x64"
		objdir ("vendor/glad/obj/x64")

    files {
		"vendor/glad/src/*.*",
    }

    includedirs {
        "vendor/glad/include"
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

		
if(_OPTIONS["with-reactphysics3d"]) then
project "vendor-reactphysics3d"
	location "project_files/vendor/reactphysics3d"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    targetname "reactphysics3d"
	
	filter "platforms:x86"
		targetdir "vendor/reactphysics3d/lib/x86"
		objdir ("vendor/reactphysics3d/obj/x86")
	filter "platforms:x64"
		targetdir "vendor/reactphysics3d/lib/x64"
		objdir ("vendor/reactphysics3d/obj/x64")

    files {
		"vendor/reactphysics3d/src/**.*",
    }

    includedirs {
        "vendor/reactphysics3d/include"
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
   
if(_OPTIONS["with-pugixml"]) then
project "vendor-pugixml"
	location "project_files/vendor/pugixml"
    kind "StaticLib"
    language "C"
    staticruntime "on"
	targetname "pugixml"

	filter "platforms:x86"
		targetdir "vendor/pugixml/lib/x86"
		objdir ("vendor/pugixml/obj/x86")
	filter "platforms:x64"
		targetdir "vendor/pugixml/lib/x64"
		objdir ("vendor/pugixml/obj/x64")

    files {
		"vendor/pugixml/src/**.*",
    }

    includedirs {
        "vendor/pugixml/include"
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
	}
	
	includedirs { 
		"source/**",
		"vendor/**",
	}
	
	filter "platforms:x86"	
		libdirs { "vendor/*/lib/x86/" }
		links { "glfw3", "glad", "pugixml", "reactphysics3d", "binkw32" }

	filter "platforms:x64"	
		libdirs { "vendor/*/lib/x64/" }
		links { "glfw3", "glad", "pugixml", "reactphysics3d" }

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
		