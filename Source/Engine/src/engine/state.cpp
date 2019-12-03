/*
 *	state.cpp
 *	State
 */

#include <Engine/State.hpp>
#include <Stdlib/SlApi.hpp>
#include <Engine/Game.hpp>
#include <Video/SpriteBatch.hpp>
#include <SceneData/SceneData.hpp>
#include <Util/Logger.hpp>
#include <StdLib/Video/Sprite.hpp>

namespace Engine
{
	namespace HookedMethods
	{
		void StateLoadScene(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			State* state = parent->GetCo<State>();

			ExpectType(0, Vm::Variant::Type::String);
			std::string sceneName = stack.Pop()->String();

			GameData::Scene* scene = Assets.scenes[sceneName];
			if (scene == nullptr)
			{
				Error << "Scene \"" << sceneName << "\" does not exist." << lendl;
				StackPush(false);
				return;
			}

			StackPush(state->LoadScene(scene->stream));
		}

		// TODO Abstract behind a TileMap link eventually
		void TilePlaceFree(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			State* state = parent->GetCo<State>();

			ExpectType(0, Vm::Variant::Type::Integer);
			ExpectType(1, Vm::Variant::Type::Integer);

			unsigned int tX = *stack.Pop();
			unsigned int tY = *stack.Pop();
			
			StackPush(state->GetTileMap()->PlaceFree(Vector2u(tX, tY)));
		}

		void Remove(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			State* state = parent->GetCo<State>();

			ExpectType(0, Vm::Variant::Type::Object);
			Vm::Class* entity = stack.Pop()->Object();

			if (entity == nullptr)
			{
				Warn << "State.Remove(): A null object was passed." << lendl;
				return;
			}
			
			state->entityManager.Remove(entity->GetCo<Entity>());
		}

		void RemoveByTag(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			State* state = parent->GetCo<State>();

			ExpectType(0, Vm::Variant::Type::String);
			std::string tag = stack.Pop()->String();
			
			state->entityManager.Remove(state->entityManager.GetFromTag(tag));
		}

		void Add(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			State* state = parent->GetCo<State>();

			ExpectType(0, Vm::Variant::Type::Object);
			Vm::Class* baseClass = stack.Pop()->Object();

			if (baseClass == nullptr)
			{
				Warn << "State.Add(): A null object was passed." << lendl;
				return;
			}

			state->entityManager.Add(new Entity(*baseClass));
		}
	}

	State::State(const Vm::Class& targetClass)
		: background(nullptr),
		  tileMap(),
		  bgMusic(nullptr),
		  target(targetClass),
		  entityManager()
	{
		// Initialize object class
		target.co = this;

		// Private hooked methods
		target.AddMethod("LoadScene", HookedMethods::StateLoadScene, 1);
		target.AddMethod("TilePlaceFree", HookedMethods::TilePlaceFree, 2);
		target.AddMethod("Remove", HookedMethods::Remove, 1);
		target.AddMethod("RemoveByTag", HookedMethods::RemoveByTag, 1);
		target.AddMethod("Add", HookedMethods::Add, 1);

		// Call default constructor if there is one
		target.GetMethod(VM_CLASS_CTOR_NAME).Execute();
	}

	State::~State()
	{
		delete bgMusic;
		delete background;
	}

	bool State::LoadScene(Util::Stream* stream)
	{
		SceneData::SceneData sceneData;

		if (!sceneData.Load(stream))
		{
			Error << "State cannot load scene." << lendl;
			return false;
		}

		// Clear previous scene
		entityManager.Clear();
		tileMap.Clear();

		// Load configuration information
		if (sceneData.configuration.defaultBgMusic != "")
		{
			// TODO Support streaming
			GameData::Sound* sound = Assets.sounds[sceneData.configuration.defaultBgMusic];
			if (sound->mode == GameData::Sound::SmStream)
			{
				bgMusic = new Audio::Sound();
				if (bgMusic->Open(sound->filename))
					bgMusic->Play();
			}
			else
				Warn << "Music memory streaming not currently supported by engine, use 'stream' instead." << lendl;
		}

		// Load background information
		Vm::Class* colorHandle = target["ClearColor"].Object();
		if (colorHandle != nullptr)
		{
			colorHandle->Field("r") = static_cast<long long>(sceneData.background.color.r * 255.0f);
			colorHandle->Field("g") = static_cast<long long>(sceneData.background.color.g * 255.0f);
			colorHandle->Field("b") = static_cast<long long>(sceneData.background.color.b * 255.0f);
			colorHandle->Field("a") = static_cast<long long>(sceneData.background.color.a * 255.0f);
		}

		if (!sceneData.background.textureName.empty())
		{
			GameData::Texture* gdTexture = Assets.textures[sceneData.background.textureName];
			background = new Video::Sprite(*gdTexture);
		}

		// Load tile information
		// TODO Support multiple maps (layers)
		if (sceneData.tiles.size() > 0)
		{
			const SceneData::Tiles::TileMap& map = sceneData.tiles[0];
			tileMap.SetTexture(map.textureName);
			tileMap.SetTileSize(map.tileSize);
			tileMap.SetMapSize(Vector2u(sceneData.configuration.width, sceneData.configuration.height));
			
			size_t i = map.tiles.size();
			while(i--)
				tileMap.Add(map.tiles[i].position, map.tiles[i].tilePosition);
		}

		// Load entity information
		for(size_t i = 0; i < sceneData.entities.size(); ++i)
		{
			SceneData::Entity sceneEntity = sceneData.entities[i];
			Vm::Class& target = VirtualMachine[sceneEntity.name];
			if (target.Name().empty())
			{
				Warn << "Entity created in scene but not declared in code. Object name \"" << sceneEntity.name << "\"." << lendl;
				continue;
			}

			Entity* entity = new Entity(target);
			entity->target.Field("x") = static_cast<double>(sceneEntity.x);
			entity->target.Field("y") = static_cast<double>(sceneEntity.y);
			entity->target.Field("tag") = sceneEntity.tag;
			
			if (entity->target.Field("Sprite").Object() != nullptr)
			{
				Video::Sprite* spriteHandle = entity->target.Field("Sprite").Object()->Field("handle").EngineObject<Video::Sprite>();
				if (spriteHandle != nullptr)
				{
					spriteHandle->scale = Vector2f(sceneEntity.scaleX, sceneEntity.scaleY);
					spriteHandle->color = sceneEntity.color;
					spriteHandle->rotation = sceneEntity.rotation;
				}
			}

			entityManager.Add(entity);
		}

		Trace << "Loaded " << sceneData.entities.size() << " scene entities." << lendl;
		
		// Signal SceneLoaded event
		for(Entity* entity : entityManager.GetEntities())
			entity->target.GetMethod("SceneLoaded").Execute();

		return true;
	}

	void State::Enter()
	{
		target.GetMethod("Create").Execute();
	}

	void State::Leave()
	{
		target.GetMethod("Destroy").Execute();
	}

	void State::Update()
	{
		if (!target["Active"].Boolean())
			return;

		target.GetMethod("Update").Execute();
		entityManager.Update();
	}

	void State::Draw()
	{
		if (background != nullptr)
			SpriteBatch.Draw(background);

		tileMap.Draw();

		entityManager.Draw();

		target.GetMethod("Draw").Execute();
	}

	bool State::ExitRequested()
	{
		Util::Stack<Vm::Variant*> stack;
		if (!target.GetMethod("ExitRequested").Execute(stack))
			return true;

		return stack.Pop()->Boolean();
	}

	void State::DispatchEvent(const Event& event)
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
				Vm::Method method = target.GetMethod("MouseScroll");
				if (!method.Legal())
					break;

				argStack.Push(VirtualMachine.CreateVariant(event.mouseScroll.y));
				VirtualMachine.Execute(method, argStack);
				break;
			}
		}

		entityManager.DispatchEvent(event);
	}

	Video::Color State::GetClearColor()
	{
		Vm::Class* color = target["ClearColor"];
		if (color == nullptr)
			return Video::Color::CornflowerBlue;

		return Video::Color(color->Field("r"), color->Field("g"), color->Field("b"), color->Field("a"));
	}

	const Video::TileMap* State::GetTileMap() const
	{
		return &tileMap;
	}
}
