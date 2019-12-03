/*
 *	sdconfiguration.cpp
 *	Scene Data Configuration
 */

#include <SceneData/Configuration.hpp>

namespace SceneData
{
	Configuration::Configuration()
		: ISection(),
		  width(0),
		  height(0),
		  defaultBgMusic("")
	{

	}

	void Configuration::Read(Util::Stream* stream)
	{
		width			= stream->ReadDword();
		height			= stream->ReadDword();
		defaultBgMusic	= stream->ReadString();
	}
}
