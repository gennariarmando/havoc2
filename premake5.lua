newoption {
	trigger     = "with-glad",
	description = "Build glad library"
}

newoption {
	trigger     = "with-box2d",
	description = "Build box2d library"
}

workspace "havoc2"
	configurations { "Release", "Debug" }
	location "project_files"
   	startproject "havoc2"
    platforms { "Win64" }
	architecture "x64"
   			
if(_OPTIONS["with-box2d"]) then
project "box2d"
	location "project_files/box2d"
    kind "StaticLib"
    language "C++"
    staticruntime "on"
    
	targetdir "vendor/box2d/lib/"
	objdir ("vendor/box2d/obj")

    files {
		"vendor/box2d/**.*",
    }

    includedirs {
        "vendor/box2d/*"
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
        "vendor/glad/*"
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
	
	libdirs { 
		"vendor/*/lib",
	}
	
	links {
		"glfw3",
		"glad",
        "box2d",
	}
	
	kind "WindowedApp"
	language "C++"
	targetdir "output/exe/"
	objdir ("output/obj")
	targetextension ".exe"
	characterset ("MBCS")
	linkoptions "/SAFESEH:NO"
	buildoptions { "-std:c++latest", "/Zc:threadSafeInit-", "/Zc:strictStrings" }
	symbols "on"

	filter "configurations:Debug"		
		targetname "havoc2"
		defines { "DEBUG" }
		staticruntime "on"
		debugdir "output/exe/"
		debugcommand "output/exe/havoc2.exe"

	filter "configurations:Release"
		targetname "havoc2"
		defines { "NDEBUG" }
		optimize "On"
		staticruntime "on"
		debugdir "output/exe/"
		debugcommand "output/exe/havoc2.exe"