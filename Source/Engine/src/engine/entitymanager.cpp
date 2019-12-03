/*
 *	entitymanager.cpp
 *	Entity Manager
 */

#include <Engine/EntityManager.hpp>
#include <Util/Logger.hpp>

namespace Engine
{
	EntityManager::EntityManager()
		: entities(nullptr),
		  entityGrid()
	{

	}

	EntityManager::~EntityManager()
	{
		Clear();
	}

	void EntityManager::DispatchEvent(const Event& event)
	{
		Node* node = entities;
		while(node)
		{
			node->value->DispatchEvent(event);
			node = node->next;
		}
	}

	void EntityManager::Update()
	{
		Node* node = entities;
		while(node)
		{
			node->value->Update();
			node = node->next;
		}
	}

	void EntityManager::Draw()
	{
		Node* node = entities;
		while(node)
		{
			node->value->Draw();
			node = node->next;
		}
	}

	void EntityManager::Add(Entity* entity)
	{
		Node* node = new Node();
		node->value = entity;

		if (entities == nullptr || entities->value->target.Field("z").Integer() <= entity->target.Field("z").Integer())
		{
			node->next = entities;
			entities = node;
		}
		else
		{
			Node* cur = entities;
			while(cur->next && cur->next->value->target.Field("z").Integer() > entity->target.Field("z").Integer())
				cur = cur->next;

			node->next = cur->next;
			cur->next = node;
		}
		
		entity->Create();
	}

	void EntityManager::Remove(Entity* entity)
	{
		Node* cur, *prev;
		cur = entities;

		if (entity == nullptr)
		{
			Warn << "Cannot remove non-existant entity!" << lendl;
			return;
		}

		entity->Destroy();

		while(cur)
		{
			if (cur->value == entity)
			{
				if (cur == entities)
					entities = cur->next;
				else
					prev->next = cur->next;

				delete cur->value;
				delete cur;
				return;
			}
			
			prev = cur;
			cur = cur->next;
		}
	}

	void EntityManager::Clear()
	{
		Node* node = entities;
		while(node)
		{
			Node* nextNode = node->next;
			Remove(node->value);
			node = nextNode;
		}
	}

	std::vector<Entity*> EntityManager::GetEntities() const
	{
		std::vector<Entity*> value;
		Node* node = entities;
		while(node)
		{
			value.push_back(node->value);
			node = node->next;
		}

		return value;
	}

	Entity* EntityManager::GetFromTag(const std::string& tag) const
	{
		Node* node = entities;
		while(node)
		{
			if (node->value->tag == tag)
				return node->value;

			node = node->next;
		}

		return nullptr;
	}

	void EntityManager::GridAdd(Entity* entity)
	{
		// TODO Implement
		// TODO Collisions
	}

	void EntityManager::GridRemove(Entity* entity)
	{

	}
}
