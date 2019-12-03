/*
 *	scenedata.hpp
 *	Scene Data Deserializer
 */

#ifndef __SCENE_DATA_HPP
#define __SCENE_DATA_HPP

#include <string>
#include <SceneData/Configuration.hpp>
#include <SceneData/Background.hpp>
#include <SceneData/Tiles.hpp>
#include <SceneData/Entities.hpp>

namespace SceneData
{
	class SceneData
	{
	public:
		Configuration		configuration;
		Background			background;
		Tiles				tiles;
		Entities			entities;

	public:
		SceneData();

		bool Load(const std::string& filename);
		bool Load(Util::Stream* stream);
	};
}

#endif
