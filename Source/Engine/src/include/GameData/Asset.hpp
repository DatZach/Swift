/*
 *	asset.hpp
 *	Game Data Asset
 */

#ifndef __GAME_DATA_ASSET_HPP
#define __GAME_DATA_ASSET_HPP

#include <Util/Stream.hpp>

#define GD_TAG_VALUE(a, b, c, d)	\
	(((unsigned int)(a)      ) |	\
	 ((unsigned int)(b) <<  8) |	\
	 ((unsigned int)(c) << 16) |	\
	 ((unsigned int)(d) << 24))

namespace GameData
{
	class IAsset
	{
	public:
		virtual ~IAsset()
		{
			
		}

		virtual void Read(Util::Stream* stream) = 0;
	};
}

#endif
