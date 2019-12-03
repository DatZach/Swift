/*
 *	section.hpp
 *	Scene Data Section
 */

#ifndef __SCENE_DATA_SECTION_HPP
#define __SCENE_DATA_SECTION_HPP

#include <Util/Stream.hpp>

#define SD_TAG_VALUE(a, b, c, d)	\
	(((unsigned int)(a)      ) |	\
	 ((unsigned int)(b) <<  8) |	\
	 ((unsigned int)(c) << 16) |	\
	 ((unsigned int)(d) << 24))

namespace SceneData
{
	class ISection
	{
	public:
		virtual ~ISection()
		{
			
		}

		virtual void Read(Util::Stream* stream) = 0;
	};
}

#endif
