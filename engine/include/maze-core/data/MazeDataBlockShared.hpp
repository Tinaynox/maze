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
#pragma once
#if (!defined(_MazeDataBlockShared_hpp_))
#define _MazeDataBlockShared_hpp_s


//////////////////////////////////////////
// Include
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"
#include "maze-core/helpers/MazeLogHelper.hpp"
#include "maze-core/serialization/MazeBinarySerializable.hpp"
#include "maze-core/serialization/MazeStringSerializable.hpp"
#include "maze-core/utils/MazeSharedCopyable.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"
#include "maze-core/data/MazeDataBlock.hpp"
#include "maze-core/memory/MazeBlockMemoryAllocator.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"
#include "maze-core/data/MazeHashedCString.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Aliases
    MAZE_USING_SHARED_PTR(DataBlockShared);


    //////////////////////////////////////////
    // Class DataBlockShared
    //
    //////////////////////////////////////////
    class MAZE_CORE_API DataBlockShared MAZE_FINAL
    {
    public:

        //////////////////////////////////////////
        DataBlockShared() = default;


        //////////////////////////////////////////
        void* allocateDataBuffer() { return m_dataBufferAllocator.allocBlock(); }

        //////////////////////////////////////////
        void freeDataBuffer(void* _ptr) { return m_dataBufferAllocator.freeBlock(_ptr); }


        //////////////////////////////////////////
        void* allocateDataBlock() { return m_dataBlockAllocator.allocBlock(); }

        //////////////////////////////////////////
        void freeDataBlock(void* _ptr) { return m_dataBlockAllocator.freeBlock(_ptr); }

        //////////////////////////////////////////
        bool setString(HashedString _name, DataBlock::SharedStringId _id);

        //////////////////////////////////////////
        DataBlock::SharedStringId addString(HashedCString _name);

        //////////////////////////////////////////
        DataBlock::SharedStringId getStringId(HashedCString _name);

        //////////////////////////////////////////
        inline DataBlock::SharedStringId addString(CString _str, Size _length)
        {
            return addString(HashedCString(_str, Hash::CalculateFNV1(_str, _length)));
        }

        //////////////////////////////////////////
        inline DataBlock::SharedStringId getStringId(CString _str, Size _length)
        {
            return getStringId(HashedCString(_str, Hash::CalculateFNV1(_str, _length)));
        }

        //////////////////////////////////////////
        CString getCString(DataBlock::SharedStringId _index) const;

        //////////////////////////////////////////
        String const& getString(DataBlock::SharedStringId _index) const;

        //////////////////////////////////////////
        HashedCString getNameHashedCString(DataBlock::SharedStringId _index) const;


        //////////////////////////////////////////
        inline StringKeyMap<DataBlock::SharedStringId> const& getStrings() const { return m_strings; }

        //////////////////////////////////////////
        inline DataBlock::SharedStringId getStringsIndexCounter() const { return m_stringsIndexCounter; }

        //////////////////////////////////////////
        inline void setStringsIndexCounter(DataBlock::SharedStringId _counter) { m_stringsIndexCounter = _counter; }

        //////////////////////////////////////////
        void clear();

    private:
        BlockMemoryAllocator<sizeof(DataBlockDataBuffer), sizeof(DataBlockDataBuffer) * 128> m_dataBufferAllocator;
        BlockMemoryAllocator<sizeof(DataBlock), sizeof(DataBlock) * 32> m_dataBlockAllocator;

        StringKeyMap<DataBlock::SharedStringId> m_strings;
        UnorderedMap<DataBlock::SharedStringId, Pair<U32, String const*>> m_stringsById;

        DataBlock::SharedStringId m_stringsIndexCounter = 0;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeDataBlockShared_h_
//////////////////////////////////////////
