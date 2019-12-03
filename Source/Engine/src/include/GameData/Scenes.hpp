/*
 *	scenes.hpp
 *	Game Data Scenes
 */

#ifndef __GAME_DATA_SCENES_HPP
#define __GAME_DATA_SCENES_HPP

#include <vector>
#include <string>
#include <GameData/Asset.hpp>

#define GD_TAG_SCENES				GD_TAG_VALUE('S', 'C', 'N', 'E')

namespace GameData
{
	class Scene : public IAsset
	{
	public:
		std::string		name;
		Util::Stream*	stream;

	public:
		Scene();
		~Scene();

		virtual void Read(Util::Stream* stream);
	};

	class Scenes : public IAsset
	{
	private:
		std::vector<Scene*> scenes;

	public:
		Scenes();
		~Scenes();

		virtual void Read(Util::Stream* stream);

		size_t size() const;
		Scene* operator[](const std::string& value) const;
		Scene* operator[](size_t i) const;
	};
}

#endif
