/*
 *	game.hpp
 *	Game Runner
 */

#ifndef __ENGINE_GAME_HPP
#define __ENGINE_GAME_HPP

#include <string>
#include <vector>
#include <queue>
#include <Video/Window.hpp>
#include <GameData/GameData.hpp>
#include <Engine/Event.hpp>
#include <Engine/State.hpp>
#include <Util/Stack.hpp>

#if defined(DEBUG) && 0
#include <vld.h>
#endif

#define Game			Engine::GameSingleton::GetInstance()
#define Assets			Engine::GameSingleton::GetInstance().GetGameData()

namespace Engine
{
	class GameSingleton
	{
	private:
		Video::Window* window;
		Util::Stack<State*> stateStack;
		std::queue<Event> eventQueue;
		std::vector<bool> keyState, previousKeyState;
		std::vector<bool> mouseState, previousMouseState;
		GameData::GameData gameData;
		double frameRate, frameTime;
		double lowestFrameRate, highestFrameRate, averageFrameRate;
		volatile bool running;

		void GameUpdate();
		void CalculateFramerate();

	public:
		GameSingleton();
		~GameSingleton();

		bool LoadGameData(const std::string& filename);
		void Initialize();
		void Run();
		void Exit(bool userTriggered = false);

		void DispatchEvent(const Event& event);

		State* GetTopState();
		void SetState(State* state);
		void PushState(State* state);
		void PopState();

		bool GetKeyState(unsigned int keyCode) const;
		bool GetPreviousKeyState(unsigned int keyCode) const;
		bool GetMouseButtonState(unsigned int button) const;
		bool GetPreviousMouseButtonState(unsigned int button) const;

		Video::Window* GetWindow() const;
		const GameData::GameData& GetGameData() const;

		static GameSingleton& GetInstance();
	};
}

#endif
