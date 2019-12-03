/*
 *	slgame.cpp
 *	Standard Library Game
 */

#include <Engine/Game.hpp>
#include <StdLib/SlApi.hpp>

namespace StdLib
{
	namespace SlGame
	{
		void Close(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			Game.Exit(true);
		}

		/*!
			class Game

			methods
			Close			0 static
		 */
		void Install()
		{
			// Game class
			Vm::Class gameClass;

			// Methods
			gameClass.AddMethod("Close", Close, 0);

			VirtualMachine.AddClass("Game", gameClass);
		}
	}
}
