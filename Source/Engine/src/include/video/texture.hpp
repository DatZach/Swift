/*
 *	texture.hpp
 *	Texture
 */

#ifndef __VIDEO_TEXTURE_HPP
#define __VIDEO_TEXTURE_HPP

#include <string>
#include <Video/Video.hpp>
#include <Util/Vector2.hpp>
#include <Util/Stream.hpp>

namespace Video
{
	class Texture
	{
	private:
		GLuint textureIndex;
		Vector2i size;
		bool linearFiltering;

		int GetMaximumSize() const;
		int ClampToValidSize(int value);

	public:
		Texture();
		~Texture();

		bool LoadFilename(const std::string& filename);
		bool Texture::LoadStream(Util::Stream* stream);
		bool LoadMemory(const unsigned char* buffer, const Vector2i& size);

		void SetLinear(bool value);

		void Bind();

		const Vector2i& GetSize() const;
	};
}

#endif
