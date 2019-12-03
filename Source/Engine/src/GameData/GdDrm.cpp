/*
 *	gddrm.cpp
 *	Digital Rights Management
 */

#include <GameData/Drm.hpp>

namespace GameData
{
	Drm::Drm()
		: type(None),
		  platforms(Configuration::PfNone),
		  keyHash("")
	{

	}

	void Drm::Read(Util::Stream* stream)
	{
		type = static_cast<LicenseType>(stream->ReadByte());

		switch(type)
		{
			case Alpha:
				keyHash = stream->ReadString();
				break;

			case Registered:
				platforms = static_cast<Configuration::PlatformFlags>(stream->ReadByte());
				keyHash = stream->ReadString();
				break;

			default:
				break;
		}
	}
}
