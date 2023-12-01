//////////////////////////////////////////
//
// Maze Engine
// Copyright (C) 2021 Dmitriy "Tinaynox" Nosov (tinaynox@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeCoreHeader.hpp"
#include "maze-core/helpers/MazeDataBlockHelper.hpp"
#include "maze-core/data/MazeDataBlockShared.hpp"
#include "maze-core/data/MazeByteBufferWriteStream.hpp"
#include "maze-core/data/MazeByteBufferReadStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace DataBlockHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API bool SaveToByteBuffer(DataBlock const& _dataBlock, ByteBuffer& _buffer)
        {
            _buffer.clear();

            ByteBufferWriteStream writeStream(_buffer);

            StringKeyMap<DataBlock::SharedStringId> const& stringsMap = _dataBlock.getShared()->getStrings();
            writeStream << (U32)stringsMap.size() << _dataBlock.getShared()->getStringsIndexCounter();
            
            for (StringKeyMap<DataBlock::SharedStringId>::const_iterator it = stringsMap.begin(),
                                                                         end = stringsMap.end();
                                                                         it != end;
                                                                         ++it)
            {
                U32 stringSize = (U32)it->first.size();
                writeStream << stringSize;
                writeStream.write((U8 const*)it->first.c_str(), stringSize);
                writeStream << it->second;
            }

            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool LoadFromByteBuffer(DataBlock& _dataBlock, ByteBuffer const& _buffer)
        {
            _dataBlock.clear();

            ByteBufferReadStream readStream(_buffer);
            U32 stringsCount = 0u;
            U32 stringsIndexCounter = 0u;
            String string;

            readStream >> stringsCount >> stringsIndexCounter;
            _dataBlock.getShared()->setStringsIndexCounter(stringsIndexCounter);
            
            for (U32 i = 0; i < stringsCount; ++i)
            {
                U32 stringSize = 0u;
                readStream >> stringSize;

                string.resize(stringSize);
                readStream.read((U8*)&string[0], stringSize);

                DataBlock::SharedStringId stringId;
                readStream >> stringId;

                _dataBlock.getShared()->setString(HashedString((CString)&string[0], (Size)stringSize), stringId);
            }

            return false;
        }


    } // namespace DataBlockHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
