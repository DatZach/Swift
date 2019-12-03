/*
 *	drm.hpp
 *	Digital Rights Management
 */

#ifndef __GAME_DATA_DIGITAL_RIGHTS_MANAGEMENT_HPP
#define __GAME_DATA_DIGITAL_RIGHTS_MANAGEMENT_HPP

#include <string>
#include <GameData/Asset.hpp>
#include <GameData/Configuration.hpp>

#define GD_TAG_DRM				GD_TAG_VALUE('S', 'D', 'R', 'M')

namespace GameData
{
	class Drm : public IAsset
	{
	public:
		enum LicenseType
		{
			None,
			Alpha,
			Free,
			Registered
		};

	public:
		LicenseType						type;
		Configuration::PlatformFlags	platforms;
		std::string						keyHash;

	public:
		Drm();

		virtual void Read(Util::Stream* stream);
	};
}

#endif
