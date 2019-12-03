/*
 *	background.hpp
 *	Scene Data Background
 */

#ifndef __SCENE_DATA_BACKGROUND_HPP
#define __SCENE_DATA_BACKGROUND_HPP

#include <string>
#include <Video/Color.hpp>
#include <SceneData/Section.hpp>

#define SD_TAG_BACKGROUND			SD_TAG_VALUE('B', 'K', 'G', 'D')

namespace SceneData
{
	class Background : public ISection
	{
	public:
		Video::Color		color;
		std::string			textureName;

	public:
		Background();

		virtual void Read(Util::Stream* stream);
	};
}

#endif
