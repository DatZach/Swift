/*
 *	sprite.hpp
 *	Swift Standard Library
 *	Sprite
 */

#ifndef __STDLIB_SPRITE_HPP
#define __STDLIB_SPRITE_HPP

#define SlGetSprite(x)		(x->objectValue ? (VmClassHandle(x)["handle"])->ObjectValue<Video::Sprite>() : nullptr)

#endif
