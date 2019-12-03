/*
 *	gdtextures.cpp
 *	Textures
 */

#include <GameData/Textures.hpp>

namespace GameData
{
	Frame::Frame()
		: IAsset(),
		  sheetIndex(-1),
		  positionX(0.0f),
		  positionY(0.0f)
	{

	}

	void Frame::Read(Util::Stream* stream)
	{
		sheetIndex	= stream->ReadWord();
		positionX	= stream->ReadWord();
		positionY	= stream->ReadWord();
	}

	Texture::Texture()
		: IAsset(),
		  name(""),
		  width(0.0f),
		  height(0.0f),
		  frames(),
		  originX(0.0f),
		  originY(0.0f),
		  smooth(false),
		  bboxLeft(0.0f),
		  bboxTop(0.0f),
		  bboxRight(1.0f),
		  bboxBottom(1.0f),
		  collisionMode(0)
	{

	}

	void Texture::Read(Util::Stream* stream)
	{
		name	= stream->ReadString();
		width	= stream->ReadWord();
		height	= stream->ReadWord();
		
		unsigned int count = stream->ReadDword();
		while(count--)
		{
			Frame frame;
			frame.Read(stream);
			
			frames.push_back(frame);
		}

		originX			= stream->ReadSingle();
		originY			= stream->ReadSingle();
		smooth			= stream->ReadBoolean();
		bboxLeft		= stream->ReadSingle();
		bboxTop			= stream->ReadSingle();
		bboxRight		= stream->ReadSingle();
		bboxBottom		= stream->ReadSingle();
		collisionMode	= stream->ReadByte();
	}

	Textures::Textures()
		: IAsset(),
		  textures()
	{

	}

	Textures::~Textures()
	{
		for(Texture* texture : textures)
			delete texture;
	}

	void Textures::Read(Util::Stream* stream)
	{
		unsigned int count = stream->ReadDword();
		while(count--)
		{
			Texture* texture = new Texture();
			texture->Read(stream);

			textures.push_back(texture);
		}
	}

	size_t Textures::size() const
	{
		return textures.size();
	}

	Texture* Textures::operator[](const std::string& value) const
	{
		for(Texture* texture : textures)
		{
			if (texture->name == value)
				return texture;
		}

		return nullptr;
	}

	Texture* Textures::operator[](size_t i) const
	{
		return textures[i];
	}
}
