/*
 *	textures.hpp
 *	Textures
 */

#ifndef __GAME_DATA_TEXTURES_HPP
#define __GAME_DATA_TEXTURES_HPP

#include <vector>
#include <GameData/Asset.hpp>

#define GD_TAG_TEXTURES			GD_TAG_VALUE('T', 'X', 'T', 'R')

namespace GameData
{
	class Frame : public IAsset
	{
	public:
		short sheetIndex;
		float positionX;
		float positionY;

	public:
		Frame();

		virtual void Read(Util::Stream* stream);
	};

	class Texture : public IAsset
	{
	public:
		std::string			name;
		float				width;
		float				height;
		std::vector<Frame>	frames;
		float				originX;
		float				originY;
		bool				smooth;
		float				bboxLeft;
		float				bboxTop;
		float				bboxRight;
		float				bboxBottom;
		unsigned char		collisionMode;

	public:
		Texture();

		virtual void Read(Util::Stream* stream);
	};

	class Textures : public IAsset
	{
	private:
		std::vector<Texture*> textures;

	public:
		Textures();
		~Textures();

		virtual void Read(Util::Stream* stream);
		
		size_t size() const;
		Texture* operator[](const std::string& value) const;
		Texture* operator[](size_t i) const;
	};
}

#endif
