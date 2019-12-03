/*
 *	texture.cpp
 *	Texture
 */

#include <Video/Texture.hpp>
#include <Util/Logger.hpp>
#include <SOIL.h>

namespace Video
{
	Texture::Texture()
		: textureIndex(0),
		  size(),
		  linearFiltering(false)
	{
		glGenTextures(1, &textureIndex);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &textureIndex);
	}

	bool Texture::LoadFilename(const std::string& filename)
	{
		unsigned char* imageData = SOIL_load_image(filename.c_str(), &size.x, &size.y, NULL, SOIL_LOAD_RGBA);
		if (imageData == nullptr)
			return false;

		bool result = LoadMemory(imageData, size);

		SOIL_free_image_data(imageData);

		return result;
	}

	bool Texture::LoadStream(Util::Stream* stream)
	{
		const unsigned char* buffer = stream->GetRawMemoryBuffer();
		int length = stream->GetLength();

		unsigned char* imageData = SOIL_load_image_from_memory(buffer, length, &size.x, &size.y, NULL, SOIL_LOAD_RGBA);
		if (imageData == nullptr)
			return false;

		bool result = LoadMemory(imageData, size);

		SOIL_free_image_data(imageData);

		return result;
	}
	
	bool Texture::LoadMemory(const unsigned char* buffer, const Vector2i& imageSize)
	{
		if (buffer == nullptr)
			return false;

		if (size.x > GetMaximumSize() || size.y > GetMaximumSize())
		{
			Error << "Texture::LoadMemory: Texture exceeds maximum size of "
				  << GetMaximumSize()
				  << " supported by this GPU."
				  << lendl;

			return false;
		}

		// Set defaults
		size = imageSize;

		// Bind the texture
		Bind();

		// Generate texture image and set texture parameters
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// TODO Mipmaps

		GlCheckError();

		return true;
	}

	void Texture::SetLinear(bool value)
	{
		if (linearFiltering == value)
			return;

		linearFiltering = value;

		// Preserve texture binding
		GLint bindingIndex;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &bindingIndex);

		// Bind this texture and update linear filtering
		Bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, linearFiltering ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, linearFiltering ? GL_LINEAR : GL_NEAREST);
	
		// Restore preserved binding
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, bindingIndex);
	}

	void Texture::Bind()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureIndex);
	}

	const Vector2i& Texture::GetSize() const
	{
		return size;
	}

	int Texture::GetMaximumSize() const
	{
		GLint size;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);

		return size;
	}

	int Texture::ClampToValidSize(int value)
	{
		if (GLEW_ARB_texture_non_power_of_two)
			return value;

		int valuePower = 1;
		while(valuePower < value)
			valuePower <<= 1;

		return valuePower;
	}
}
