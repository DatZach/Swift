/*
 *	entity.hpp
 *	Entity
 */

#ifndef __ENGINE_ENTITY_HPP
#define __ENGINE_ENTITY_HPP

#include <Video/Sprite.hpp>
#include <GameData/BytecodeObjects.hpp>
#include <Engine/Event.hpp>

namespace Engine
{
	class State;

	class Entity
	{
	private:
		State*				parent;

	public:
		Vm::Class			target;
		std::string			tag;

	public:
		Entity(const Vm::Class& targetClass);

		void DispatchEvent(const Event& event);

		void Create();
		void Destroy();
		void Update();
		void Draw();

		State* GetParent() const;
		FloatRect GetBoundingBox();
	};
}

#endif
