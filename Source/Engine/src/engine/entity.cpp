/*
 *	entity.cpp
 *	Entity
 */

#include <Engine/Game.hpp>
#include <Engine/Entity.hpp>
#include <Video/SpriteBatch.hpp>
#include <GameData/Textures.hpp>
#include <Util/Logger.hpp>
#include <StdLib/Video/Sprite.hpp>
#include <StdLib/SlApi.hpp>

namespace Engine
{
	namespace HookedMethods
	{
		void GetParentState(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			Entity* entity = parent->GetCo<Entity>();
			StackPush(Vm::Variant(&entity->GetParent()->target, false));
		}

		void GetFromTag(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			Entity* entity = parent->GetCo<Entity>();
			std::string tag = *stack.Pop();

			Entity* tagged = entity->GetParent()->entityManager.GetFromTag(tag);
			StackPush(Vm::Variant(tagged, nullptr));
		}

		void GetCollidingFirst(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			Entity* entity = parent->GetCo<Entity>();
			FloatRect boundingBox = entity->GetBoundingBox();
			if (!boundingBox.width || !boundingBox.height)
			{
				StackPush(nullptr);
				return;
			}

			for(Entity* ent : entity->GetParent()->entityManager.GetEntities())
			{
				if (ent == entity)
					continue;

				FloatRect entBBox = ent->GetBoundingBox();

				if (entBBox.Intersects(boundingBox))
				{
					StackPush(Vm::Variant(&ent->target, false));
					return;
				}
			}

			StackPush(nullptr);
		}
	}

	Entity::Entity(const Vm::Class& targetClass)
		: target(targetClass),
		  parent(Game.GetTopState()),
		  tag("")
	{
		target.co = this;

		// Private hooked methods
		target.AddMethod("__get_ParentState", HookedMethods::GetParentState, 0);
		target.AddMethod("GetFromTag", HookedMethods::GetFromTag, 1);
		target.AddMethod("GetCollidingFirst", HookedMethods::GetCollidingFirst, 0);
	}

	void Entity::DispatchEvent(const Event& event)
	{
		Util::Stack<Vm::Variant*> argStack;

		switch(event.type)
		{
			case Event::FocusChange:
			{
				Vm::Method method = target.GetMethod("FocusChanged");
				if (!method.Legal())
					break;

				argStack.Push(VirtualMachine.CreateVariant(event.focusChanged.focused));
				VirtualMachine.Execute(method, argStack);
				break;
			}

			case Event::Key:
			{
				switch(event.key.action)
				{
					case GLFW_PRESS:
					{
						Vm::Method method = target.GetMethod("KeyPressed");
						if (!method.Legal())
							break;

						argStack.Push(VirtualMachine.CreateVariant(event.key.keyCode));
						VirtualMachine.Execute(method, argStack);
						break;
					}

					case GLFW_RELEASE:
					{
						Vm::Method method = target.GetMethod("KeyReleased");
						if (!method.Legal())
							break;

						argStack.Push(VirtualMachine.CreateVariant(event.key.keyCode));
						VirtualMachine.Execute(method, argStack);
						break;
					}

					case (GLFW_REPEAT + 1):
					{
						Vm::Method method = target.GetMethod("KeyHeld");
						if (!method.Legal())
							break;

						argStack.Push(VirtualMachine.CreateVariant(event.key.keyCode));
						VirtualMachine.Execute(method, argStack);
						break;
					}
				}
				break;
			}
			
			case Event::MouseButton:
			{
				if (!GetBoundingBox().Contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))
					return;

				switch(event.mouseButton.action)
				{
					case GLFW_PRESS:
					{
						Vm::Method method = target.GetMethod("MouseButtonPressed");
						if (!method.Legal())
							break;

						argStack.Push(VirtualMachine.CreateVariant(event.mouseButton.button));
						VirtualMachine.Execute(method, argStack);
						break;
					}

					case GLFW_RELEASE:
					{
						Vm::Method method = target.GetMethod("MouseButtonReleased");
						if (!method.Legal())
							break;

						argStack.Push(VirtualMachine.CreateVariant(event.mouseButton.button));
						VirtualMachine.Execute(method, argStack);
						break;
					}

					case (GLFW_REPEAT + 1):
					{
						Vm::Method method = target.GetMethod("MouseButtonHeld");
						if (!method.Legal())
							break;

						argStack.Push(VirtualMachine.CreateVariant(event.mouseButton.button));
						VirtualMachine.Execute(method, argStack);
						break;
					}
				}
				break;
			}

			case Event::MousePosition:
			{
				Vm::Method method = target.GetMethod("MousePosition");
				if (!method.Legal())
					break;

				argStack.Push(VirtualMachine.CreateVariant(event.mousePosition.x));
				argStack.Push(VirtualMachine.CreateVariant(event.mousePosition.y));
				VirtualMachine.Execute(method, argStack);
				break;
			}

			case Event::MouseScroll:
			{
				const Vm::Method method = target.GetMethod("MouseScroll");
				if (!method.Legal())
					break;

				argStack.Push(VirtualMachine.CreateVariant(event.mouseScroll.y));
				VirtualMachine.Execute(method, argStack);
				break;
			}
		}
	}

	void Entity::Create()
	{
		target.GetMethod("Create").Execute();
	}

	void Entity::Destroy()
	{
		target.GetMethod("Destroy").Execute();
	}

	void Entity::Update()
	{
		target.GetMethod("Update").Execute();
	}

	void Entity::Draw()
	{
		Vm::Method method = target.GetMethod("Draw");
		if (method.Type() != Vm::Method::Type::Illegal)
			method.Execute();
		else
		{
			Vm::Class* sprite = target["Sprite"].Object();
			if (sprite == nullptr || sprite->Name().empty())
				return;

			Video::Sprite* hSprite = sprite->Field("handle").EngineObject<Video::Sprite>();
			if (hSprite == nullptr)
				return;

			hSprite->position = Vector2f(static_cast<float>(target["x"].Number()),
										 static_cast<float>(target["y"].Number()));
			hSprite->animated = sprite->Field("Animated").Boolean();
			hSprite->frameSpeed = sprite->Field("FrameSpeed").Number();
			// TODO Add other fields

			SpriteBatch.Draw(hSprite);
		}
	}

	State* Entity::GetParent() const
	{
		return parent;
	}

	FloatRect Entity::GetBoundingBox()
	{
		Vm::Class* sprite = target["Sprite"].Object();
		if (sprite == nullptr || sprite->Name().empty())
			return FloatRect();

		Video::Sprite* hSprite = sprite->Field("handle").EngineObject<Video::Sprite>();
		if (hSprite == nullptr)
			return FloatRect();

		FloatRect spriteBoundingBox = hSprite->GetBoundingBox();
		return FloatRect(static_cast<float>(target["x"].Number()) - hSprite->gdTexture.originX,
						 static_cast<float>(target["y"].Number()) - hSprite->gdTexture.originY,
						 spriteBoundingBox.width, spriteBoundingBox.height);
	}
}
