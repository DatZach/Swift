/*
 *	stdlibtexture.cpp
 *	Swift Standard Library
 *	Sprite
 */

#include <StdLib/SlApi.hpp>
#include <Video/Sprite.hpp>
#include <Video/SpriteBatch.hpp>
#include <Engine/Game.hpp>

namespace StdLib
{
	namespace Sprite
	{
		/*
		 *	class Sprite;
		 *
		 *	ctor(string assetName)
		 *	dtor()
		 *
		 *	x
		 *	y
		 *	Animated
		 *	FrameSpeed
		 *	Color
		 *	ScaleX
		 *	ScaleY
		 *	Rotation
		 *
		 *	Draw()
		 *	Draw(x, y)
		 */

		void ConstructorAsset(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);

			std::string assetName = *stack.Pop();
			GameData::Texture* asset = Assets.textures[assetName];
			if (asset == nullptr)
			{
				Error << "Texture asset \""
					  << assetName
					  << "\" doesn't exist."
					  << lendl;
				//stack.Push(new Vm::Variant(parent));
				return;
			}

			parent->Field("handle").SetEngineObject(new Video::Sprite(*asset));
			//stack.Push(new Vm::Variant(parent));
		}

		void Deconstructor(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			delete parent->Field("handle").EngineObject<Video::Sprite>();
		}

		void Draw(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			Video::Sprite* hSprite = parent->Field("handle").EngineObject<Video::Sprite>();
			if (hSprite == nullptr)
				return;

			float x = static_cast<float>(parent->Field("x").Number());
			float y = static_cast<float>(parent->Field("y").Number());
			double frameSpeed = parent->Field("FrameSpeed");
			float scaleX = static_cast<float>(parent->Field("ScaleX").Number());
			float scaleY = static_cast<float>(parent->Field("ScaleY").Number());
			float rotation = static_cast<float>(parent->Field("Rotation").Number());

			hSprite->position = Vector2f(x, y);
			hSprite->frameSpeed = frameSpeed;
			hSprite->scale = Vector2f(scaleX, scaleY);
			hSprite->rotation = rotation;

			SpriteBatch.Draw(hSprite);
		}

		void DrawXY(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			ExpectTypeNumber(1);

			Video::Sprite* hSprite = parent->Field("handle").EngineObject<Video::Sprite>();
			if (hSprite == nullptr)
				return;

			float x = static_cast<float>(stack.Pop()->Number());
			float y = static_cast<float>(stack.Pop()->Number());
			double frameSpeed = parent->Field("FrameSpeed").Number();
			float scaleX = static_cast<float>(parent->Field("ScaleX").Number());
			float scaleY = static_cast<float>(parent->Field("ScaleY").Number());
			float rotation = static_cast<float>(parent->Field("Rotation").Number());

			hSprite->position = Vector2f(x, y);
			hSprite->frameSpeed = frameSpeed;
			hSprite->scale = Vector2f(scaleX, scaleY);
			hSprite->rotation = rotation;

			SpriteBatch.Draw(hSprite);
		}

		/*!
			class Sprite

			fields
			x				double
			y				double
			Animated		int
			FrameSpeed		double
			Color			object:Color
			ScaleX			double
			ScaleY			double
			Rotation		double

			methods
			__Constructor	1
			Draw			0
			Draw			2
		 */
		void Install()
		{
			// Sprite class
			Vm::Class spriteClass;

			// Fields
			spriteClass["handle"] = nullptr;
			spriteClass["x"] = 0.0;
			spriteClass["y"] = 0.0;
			spriteClass["Animated"] = 1;
			spriteClass["FrameSpeed"] = 1.0;
			spriteClass["Color"] = nullptr;
			spriteClass["ScaleX"] = 1.0;
			spriteClass["ScaleY"] = 1.0;
			spriteClass["Rotation"] = 0.0;

			// Methods
			spriteClass.AddMethod(VM_CLASS_CTOR_NAME, ConstructorAsset, 1);
			spriteClass.AddMethod(VM_CLASS_DTOR_NAME, Deconstructor, 0);
			spriteClass.AddMethod("Draw", Draw, 0);
			spriteClass.AddMethod("Draw", DrawXY, 2);

			VirtualMachine.AddClass("Sprite", spriteClass);
		}
	}
}
