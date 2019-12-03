/* 
 *	game.cpp
 *	Game Runner
 */

#include <thread>
#include <Engine/Game.hpp>
#include <Video/Video.hpp>
#include <Audio/Audio.hpp>
#include <Stdlib/StdLib.hpp>
#include <Video/SheetCache.hpp>
#include <Video/SpriteBatch.hpp>
#include <Util/Logger.hpp>
#include <Util/FileSystem.hpp>

// TODO Remove
#include <Video/Shape.hpp>

namespace Engine
{
	GameSingleton::GameSingleton()
		: window(nullptr),
		  stateStack(),
		  eventQueue(),
		  keyState(GLFW_KEY_LAST + 1, false),
		  previousKeyState(keyState),
		  mouseState(GLFW_MOUSE_BUTTON_LAST + 1, false),
		  previousMouseState(mouseState),
		  gameData(),
		  frameRate(0.0),
		  frameTime(0.0),
		  lowestFrameRate(std::numeric_limits<double>::max()),
		  highestFrameRate(0.0),
		  averageFrameRate(0.0),
		  running(true)
	{
		
	}

	GameSingleton::~GameSingleton()
	{
		delete window;

		Audio::System::Shutdown();
		Video::System::Shutdown();
		StdLib::Uninstall();
	}

	void GameSingleton::GameUpdate()
	{
		// Attach OGL context to this thread
		window->AttachContext();

		// Update when dirty
		window->UpdateView();

		frameTime = glfwGetTime();

		while(running && !stateStack.Empty())
		{
			double delta = glfwGetTime() - frameTime;
			
			if (delta > 1.0 / gameData.config.TargetFramerate)
			{
				// Dispatch queued events
				while(!eventQueue.empty())
				{
					for(int i = stateStack.Count() - 1; i >= 0; --i)
						stateStack[i]->DispatchEvent(eventQueue.front());

					eventQueue.pop();
				}

				// Dispatch persistent events
				for(size_t i = 0; i < keyState.size(); ++i)
				{
					if (!keyState[i])
						continue;

					Event event;
					event.type			= Event::Key;
					event.key.keyCode	= i;
					event.key.action	= GLFW_REPEAT + 1;
					event.key.mods		= 0;

					for(int i = stateStack.Count() - 1; i >= 0; --i)
						stateStack[i]->DispatchEvent(event);
				}

				for(size_t i = 0; i < mouseState.size(); ++i)
				{
					if (!mouseState[i])
						continue;

					double x, y;
					window->GetCursorPos(&x, &y);

					Event event;
					event.type					= Event::MouseButton;
					event.mouseButton.button	= i;
					event.mouseButton.action	= GLFW_REPEAT + 1;
					event.mouseButton.x			= static_cast<int>(x);
					event.mouseButton.y			= static_cast<int>(y);

					for(int i = stateStack.Count() - 1; i >= 0; --i)
						stateStack[i]->DispatchEvent(event);
				}

				// Update states (top down)
				for(int i = stateStack.Count() - 1; i >= 0; --i)
					stateStack[i]->Update();
			
				frameTime = glfwGetTime();

				previousKeyState = keyState;
				previousMouseState = mouseState;
			}

			// Clear background with bottom most state's color
			window->Clear(stateStack[0]->GetClearColor());

			// Draw states (bottom up)
			SpriteBatch.Begin();
			for(size_t i = 0; i < stateStack.Count(); ++i)
				stateStack[i]->Draw();

			SpriteBatch.End();
			SpriteBatch.Render();



			// TODO Remove me
			Video::ShapeRectangle rect;
			rect.SetPosition(Vector2f(16.0f, 16.0f));
			rect.SetSize(Vector2f(320.0f, 240.0f));
			rect.SetColor(Video::Color(255, 0, 0));
			rect.Draw();

			Video::ShapeCircle circle;
			circle.SetPosition(Vector2f(400.0f, 300.0f));
			circle.SetRadius(300.0f);
			circle.SetPointCount(32);
			circle.SetColor(Video::Color(0, 255, 0, 128));
			circle.Draw();

			Video::ShapeConvex triangle;
			triangle.SetPosition(Vector2f(450.0f, 200.0f));
			triangle.SetPoint(0, Vector2f(0.0f, 0.0f));
			triangle.SetPoint(1, Vector2f(256.0f, 256.0f));
			triangle.SetPoint(2, Vector2f(0.0f, 256.0f));
			triangle.SetColor(Video::Color(255, 0, 255));
			triangle.Draw();


			/*
			 *	Multipurpose hack
			 *	This prevents us from deleting VM objects that are in use this current frame and
			 *	improves performance by only doing cleaning once a frame instead of multiple times
			 *	a frame.
			 */

			VirtualMachine.CollectGarbage(Util::Stack<Vm::Variant*>());

			// Swap buffers and calculate frameRate
			window->SwapBuffers();
			CalculateFramerate();

			/*
			 *	TODO
			 *	Need to prevent 100% CPU and GPU usage with this.
			 *	Should dynamically let up on time depending on how quickly we're finishing frames.
			 *	Should sleep for the remaining slack time between frames at the target frameRate.
			 */
			//Sleep((int)(1000.0 / 60.0) - 1);
		}

		// Clean up states before we exit
		while(!stateStack.Empty())
			PopState();
	}

	void GameSingleton::CalculateFramerate()
	{
		static double lastFrameTime = 0.0;
		static unsigned int frameCount = 0;

		if (frameTime - lastFrameTime >= 0.5)
		{
			double delta = frameTime - lastFrameTime;
			frameRate = static_cast<double>(frameCount) / delta;
			frameRate = floor(frameRate + 0.5);
			lastFrameTime = frameTime;
			frameCount = 1;

			if (frameRate != 0.0)
			{
				if (frameRate > highestFrameRate)
					highestFrameRate = frameRate;
				
				if (frameRate < lowestFrameRate && frameRate)
					lowestFrameRate = frameRate;

				averageFrameRate = averageFrameRate == 0.0 ? frameRate : (averageFrameRate + frameRate) / 2.0;
			}
		}
		else
			++frameCount;
	}

	bool GameSingleton::LoadGameData(const std::string& filename)
	{
		// Load game data
		if (!filename.empty())
			return gameData.Load(filename);

		// No file was explicitly passed, try find game data in the working directory
		std::string path = Util::Filesystem::FindFile(GD_FILE_MASK);
		if (path.empty())
		{
			Error << "Cannot find suitable game data in the current working directory." << lendl;
			return false;
		}

		return gameData.Load(path);
	}

	void GameSingleton::Initialize()
	{
		// Initialize all subsystems
		Video::System::Initialize();
		Audio::System::Initialize();
		StdLib::Install();

		// Create the window so we have a context to do everything else in, it's hidden initially
		window = new Video::Window(gameData.config.GameTitle, gameData.config.Size);

		// Load texture sheets after we create the context
		SheetCache.LoadSheets(gameData.textureSheets);

		// Now that all initialization is done, unhide window
		window->Show();
	}

	void GameSingleton::Run()
	{
		// Set initial state
		const Vm::Class& stateClass = VirtualMachine[gameData.config.InitialState];
		if (stateClass.Name().empty())
		{
			if (gameData.config.InitialState == "")
				Error << "No initial state set." << lendl;
			else
				Error << "Initial state \""
					  << gameData.config.InitialState
					  << "\" doesn't exist."
					  << lendl;

			return;
		}

		SetState(new State(stateClass));

		// Detach context so the draw thread can obtain it
		window->DetachContext();

		// Create update/draw thread
		// NOTE This will LEAK MEMORY (44 bytes), this is a bug in the C++11 CRT for VS2012
		//		it has been fixed in VS2013, whenever you get around to updating.
		std::thread updateDrawThread(&GameSingleton::GameUpdate, this);

		// Handle window events
		while(running && !stateStack.Empty())
		{
			window->WaitEvents();

			if (window->ShouldClose())
				Exit();
		}

		// Join threads before termination
		updateDrawThread.join();

		if (averageFrameRate != 0)
		{
			VerboseTrace << "Framerate Statistics" << lendl;
			VerboseTrace << "High: "
						 << highestFrameRate
						 << " | Low: "
						 << lowestFrameRate
						 << " | Avg: "
						 << averageFrameRate
						 << lendl;
		}
	}

	void GameSingleton::Exit(bool userTriggered)
	{
		VerboseTrace << "Engine exiting..." << lendl;

		if (userTriggered)
		{
			for(size_t i = 0; i < stateStack.Count(); ++i)
			{
				if (!stateStack[i]->ExitRequested())
				{
					VerboseTrace << "Engine shutdown aborted via state request." << lendl;
					return;
				}
			}
		}

		running = false;
	}

	void GameSingleton::DispatchEvent(const Event& event)
	{
		switch(event.type)
		{
			case Event::Key:
				// Discard these events, they shouldn't be dispatched in the first place...
				if (event.key.keyCode == GLFW_KEY_UNKNOWN || event.key.keyCode >= keyState.size())
					return;

				// Update key state
				keyState[event.key.keyCode] = event.key.action == GLFW_PRESS || event.key.action == GLFW_REPEAT;
				break;

			case Event::MouseButton:
				// Discard these events, they shouldn't be dispatched in the first place...
				if (event.mouseButton.button >= mouseState.size())
					return;

				mouseState[event.mouseButton.button] = event.mouseButton.action == GLFW_PRESS || event.mouseButton.action == GLFW_REPEAT;
				break;
		}

		// NOTE New event system queues events for dispatching in update/draw thread
		eventQueue.push(event);
	}

	State* GameSingleton::GetTopState()
	{
		return stateStack.Peek();
	}

	void GameSingleton::SetState(State* state)
	{
		size_t count = stateStack.Count();
		while(count--)
			PopState();

		PushState(state);
	}

	void GameSingleton::PushState(State* state)
	{
		stateStack.Push(state);
		state->Enter();
	}

	void GameSingleton::PopState()
	{
		stateStack.Peek()->Leave();
		delete stateStack.Pop();
	}

	bool GameSingleton::GetKeyState(unsigned int keyCode) const
	{
		if (keyCode >= keyState.size())
			return false;

		return keyState[keyCode];
	}

	bool GameSingleton::GetPreviousKeyState(unsigned int keyCode) const
	{
		if (keyCode >= previousKeyState.size())
			return false;

		return previousKeyState[keyCode];
	}
	
	bool GameSingleton::GetMouseButtonState(unsigned int button) const
	{
		if (button >= mouseState.size())
			return false;

		return mouseState[button];
	}

	bool GameSingleton::GetPreviousMouseButtonState(unsigned int button) const
	{
		if (button >= previousMouseState.size())
			return false;

		return previousMouseState[button];
	}

	Video::Window* GameSingleton::GetWindow() const
	{
		return window;
	}

	const GameData::GameData& GameSingleton::GetGameData() const
	{
		return gameData;
	}

	GameSingleton& GameSingleton::GetInstance()
	{
		static GameSingleton instance;
		return instance;
	}
}
