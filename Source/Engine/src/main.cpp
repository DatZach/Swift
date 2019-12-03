/*
 *	main.cpp
 *	Swift Engine
 *
 *	Note that the version will only update in Release mode
 *
 */

#include <string>
#include <Audio/Audio.hpp>
#include <Engine/Game.hpp>
#include <Util/CommandLine.hpp>
#include <Util/StopWatch.hpp>
#include <Util/Logger.hpp>
#include <Version.hpp>

#ifdef DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

class MemoryLeakChecker
{
public:
	~MemoryLeakChecker()
	{
		 VerboseTrace << (_CrtDumpMemoryLeaks() == 0 ? "No leaks!" : "Leaks detected!")
			          << lendl;
	}
};

/*
 *	Note that this should work fine on Windows when compiled with MSVC.
 *	This takes advantage of a side effect of the CRT's implementation.
 *	"These terminators MUST be executed in reverse order (LIFO)!"
 *		crt0dat.c(540)
 */
MemoryLeakChecker checker;
#endif

int main(int argc, char* argv[])
{
	// Parse command line
	Util::CommandLine commandLine(argc, argv);

	// Set debug flag
	if (commandLine.IsFlagSet("debug"))
	{
		VerboseTrace.verboseTrace = true;
#ifdef DEBUG
		Util::CommandLine::RedirectIOToConsole();
#endif
	}
	else
		VerboseTrace.verboseTrace = false;

	// Set mute flag
	if (commandLine.IsFlagSet("mute"))
		Audio::System::SetMuteAll(true);

	// Display engine version
	VerboseTrace << "Swift Engine v"
				 << ENGINE_VERSION_MAJOR << "."
				 << ENGINE_VERSION_MINOR << "."
				 << ENGINE_VERSION_BUILD << "-"
				 << ENGINE_VERSION_REV_TAG
				 << lendl;

	Util::Stopwatch loadTime;
	loadTime.Start();
	
	// Load game data
	try
	{
		std::string gameDataPath = commandLine.GetFlagArgument("game");

		if (!Game.LoadGameData(gameDataPath))
		{
			Error << "Cannot load game data!" << lendl;
			return 1;
		}
	}
	catch(Util::StreamException& e)
	{
		Error << "Stream Exception (Loading)" << lendl;
		Error << ltab << e.what() << lendl;

		return 1;
	}
	
	// Initialize engine
	try
	{
		Game.Initialize();
	}
	catch(Video::VideoException& e)
	{
		Error << "Video Exception (Initialization)" << lendl;
		Error << ltab << e.what() << lendl;

		return 2;
	}
	catch(Audio::AudioException& e)
	{
		Error << "Audio Exception (Initialization)" << lendl;
		Error << ltab << e.what() << lendl;

		return 3;
	}

	loadTime.Stop();

	VerboseTrace << "Initialized engine in "
				 << loadTime.GetElapsedMilliseconds()
				 << "ms."
				 << lendl;

	// Run game
	Game.Run();

	return 0;
}

#if defined WINDOWS && !defined DEBUG
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	return main(__argc, __argv);
}
#endif
