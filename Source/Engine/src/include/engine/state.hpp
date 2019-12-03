/*
 *	state.hpp
 *	State
 */

#ifndef __ENGINE_STATE_HPP
#define __ENGINE_STATE_HPP

#include <Engine/EntityManager.hpp>
#include <Engine/Event.hpp>
#include <Video/Color.hpp>
#include <Video/Sprite.hpp>
#include <Video/TileMap.hpp>
#include <Audio/Sound.hpp>
#include <Vm/VirtualMachine.hpp>

namespace Engine
{
	class State
	{
	private:
		//Vm::Variant*	clearColor;
		Video::Sprite*	background;
		Video::TileMap	tileMap;
		//Vm::Variant*	active;
		Audio::Sound*	bgMusic;

	public:
		Vm::Class		target;
		EntityManager	entityManager;

	public:
		State(const Vm::Class& targetClass);
		~State();

		bool LoadScene(Util::Stream* stream);

		void Enter();
		void Leave();
		void Update();
		void Draw();

		bool ExitRequested();

		void DispatchEvent(const Event& event);

		Video::Color GetClearColor();
		const Video::TileMap* GetTileMap() const;
	};
}

#endif
