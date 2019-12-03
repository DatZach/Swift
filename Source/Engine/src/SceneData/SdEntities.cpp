/*
 *	sdentities.cpp
 *	Scene Data Entities
 */

#include <SceneData/Entities.hpp>

namespace SceneData
{
	Entity::Entity()
		: ISection(),
		  tag(""),
		  name(""),
		  x(0),
		  y(0),
		  scaleX(0.0f),
		  scaleY(0.0f),
		  color(Video::Color::White),
		  rotation(0)
	{

	}

	void Entity::Read(Util::Stream* stream)
	{
		tag			= stream->ReadString();
		name		= stream->ReadString();
		x			= stream->ReadDword();
		y			= stream->ReadDword();
		scaleX		= stream->ReadSingle();
		scaleY		= stream->ReadSingle();
		color		= stream->ReadColor();
		rotation	= stream->ReadWord();
	}

	Entities::Entities()
		: ISection(),
		  entities()
	{

	}

	void Entities::Read(Util::Stream* stream)
	{
		unsigned int count = stream->ReadDword();
		while(count--)
		{
			Entity entity;
			entity.Read(stream);

			entities.push_back(entity);
		}
	}

	size_t Entities::size() const
	{
		return entities.size();
	}

	const Entity& Entities::operator[](size_t i) const
	{
		return entities[i];
	}
}
