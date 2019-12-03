/*
 *	configuration.hpp
 *	Scene Data Configuration
 */

#ifndef __SCENE_DATA_CONFIGURATION_HPP
#define __SCENE_DATA_CONFIGURATION_HPP

#include <string>
#include <SceneData/Section.hpp>

#define SD_TAG_CONFIG			SD_TAG_VALUE('C', 'N', 'F', 'G')

namespace SceneData
{
	class Configuration : public ISection
	{
	public:
		unsigned int		width;
		unsigned int		height;
		std::string			defaultBgMusic;

	public:
		Configuration();

		virtual void Read(Util::Stream* stream);
	};
}

#endif
