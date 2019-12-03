/*
 *	bytecodeobjects.hpp
 *	Bytecode Objects
 */

#ifndef __GAME_DATA_BYTECODE_OBJECTS_HPP
#define __GAME_DATA_BYTECODE_OBJECTS_HPP

#include <string>
#include <vector>
#include <GameData/Asset.hpp>
#include <Vm/Class.hpp>

#define GD_TAG_CODE				GD_TAG_VALUE('C', 'O', 'D', 'E')

namespace GameData
{
	class BytecodeObjects : public IAsset
	{
	public:
		virtual void Read(Util::Stream* stream) final;
	};
}

#endif
