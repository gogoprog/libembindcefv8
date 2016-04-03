solution "libembindcefv8"
    configurations { "DebugCef", "ReleaseCef", "DebugEmscripten", "ReleaseEmscripten" }

    if os.is("windows") then
        platforms { "x32" }
    else
        platforms { "x64" }
    end

    project "tests"
        kind "ConsoleApp"
        language "C++"

        files {
            "../src/**.h",
            "../src/**.cpp",
            "../tests/*.cpp"
            }

        includedirs {
            "../src/"
            }

        flags {
            "ExtraWarnings",
            "FloatFast",
            "NoExceptions",
            "NoFramePointer",
            "NoNativeWChar"
            }

        targetdir "../tests/"

        if not os.is("windows") then
            buildoptions { "-std=c++11 -Wno-error=unused-variable -Wno-error=unused-parameter" }
        end

        if os.is("linux") then
            defines { "_LINUX" }
            linkoptions { "-Wl,-rpath=../deps/lib/"}
        elseif os.is("windows") then
            defines { "_WINDOWS", "NOMINMAX" }
            flags { "StaticRuntime" }
        end

        configuration "Debug*"
            defines { "DEBUG" }
            flags { "Symbols" }
            targetname "testsd"

        configuration "Release*"
            defines { "NDEBUG" }
            flags { "Optimize" }
            targetname "tests"

        configuration "DebugEmscripten"
            postbuildcommands { "emcc --bind ../tests/testsd.bc -o ../tests/testsd.js" }

        configuration "ReleaseEmscripten"
            postbuildcommands { "emcc --bind ../tests/tests.bc -o ../tests/tests.js" }

        configuration "*Emscripten"
            defines { "EMSCRIPTEN" }
            targetsuffix ".bc"
            if not os.is("windows") then
                linkoptions { "-Wno-warn-absolute-paths" }
            end


        configuration "*Cef"
            defines { "CEF" }
            includedirs { "../deps/include/cef/" }
            links {
                "cef",
                "cef_dll_wrapper",
                "pthread"
                }
            libdirs { "../deps/lib" }
