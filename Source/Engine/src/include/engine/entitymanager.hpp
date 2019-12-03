/*
 *	entitymanager.hpp
 *	Entity Manager
 */

#ifndef __ENGINE_ENTITY_MANAGER_HPP
#define __ENGINE_ENTITY_MANAGER_HPP

#include <map>
#include <list>
#include <Engine/Entity.hpp>
#include <Engine/Event.hpp>
#include <Util/Vector2.hpp>

namespace Engine
{
	class EntityManager
	{
	private:
		struct Node
		{
			Entity*		value;
			Node*		next;
		};

	private:
		Node* entities;
		std::map<Vector2i, std::list<Entity*> > entityGrid;

		void GridAdd(Entity* entity);
		void GridRemove(Entity* entity);

	public:
		EntityManager();
		~EntityManager();

		void DispatchEvent(const Event& event);

		void Update();
		void Draw();

		void Add(Entity* entity);
		void Remove(Entity* entity);
		void Clear();

		std::vector<Entity*> GetEntities() const;

		Entity* GetFromTag(const std::string& tag) const;
	};
}

#endif
