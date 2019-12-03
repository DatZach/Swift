/*
 *	entities.hpp
 *	Scene Data Entities
 */

#ifndef __SCENE_DATA_ENTITIES_HPP
#define __SCENE_DATA_ENTITIES_HPP

#include <vector>
#include <string>
#include <Video/Color.hpp>
#include <SceneData/Section.hpp>

#define SD_TAG_ENTITIES				SD_TAG_VALUE('E', 'N', 'T', 'S')

namespace SceneData
{
	class Entity : public ISection
	{
	public:
		std::string			tag;
		std::string			name;
		unsigned int		x;
		unsigned int		y;
		float				scaleX;
		float				scaleY;
		Video::Color		color;
		unsigned short		rotation;

	public:
		Entity();

		virtual void Read(Util::Stream* stream);
	};

	class Entities : public ISection
	{
	public:
		std::vector<Entity> entities;
		
	public:
		Entities();

		virtual void Read(Util::Stream* stream);

		size_t size() const;
		const Entity& operator[](size_t i) const;
	};
}

#endif
