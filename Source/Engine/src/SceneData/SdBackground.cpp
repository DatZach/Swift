/*
 *	sdbackground.cpp
 *	Scene Data Background
 */

#include <SceneData/Background.hpp>

namespace SceneData
{
	Background::Background()
		: ISection(),
		  color(Video::Color::CornflowerBlue),
		  textureName("")
	{

	}

	void Background::Read(Util::Stream* stream)
	{
		color = stream->ReadColor();
		textureName = stream->ReadString();
	}
}
