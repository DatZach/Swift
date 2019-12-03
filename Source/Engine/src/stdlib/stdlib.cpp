/*
 *	stdlib.cpp
 *	Swift Standard Library
 */

namespace StdLib
{
	// Audio
	namespace Sound { void Install(); }

	// DataStructures
	namespace Grid { void Install(); }

	// IO
	namespace BinaryFile { void Install(); }
	namespace FileSystem { void Install(); }
	namespace Keyboard { void Install(); }
	namespace Mouse { void Install(); }
	namespace TextFile { void Install(); }

	// Math
	namespace Math { void Install(); }
	namespace Random { void Install(); }
	
	// RunTime
	namespace List { void Install(); }
	namespace String { void Install(); }

	// System
	namespace Debug { void Install(); }
	namespace Time { void Install(); }
	namespace SlGame { void Install(); }

	// Video
	namespace Sprite { void Install(); }
	namespace Color { void Install(); }

	void Install()
	{
		// Audio
		Sound::Install();
		
		// DataStructures
		Grid::Install();

		// IO
		BinaryFile::Install();
		FileSystem::Install();
		Keyboard::Install();
		Mouse::Install();
		TextFile::Install();

		// Math
		Math::Install();
		Random::Install();
		
		// RunTime
		List::Install();
		String::Install();

		// System
		Debug::Install();
		Time::Install();
		SlGame::Install();
		
		// Video
		Sprite::Install();
		Color::Install();
	}

	void Uninstall()
	{
		
	}
}
