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
#include "maze-core/data/MazeDataBlockShared.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/encoding/MazeEncodingBase64.hpp"


//////////////////////////////////////////
// Namespace
namespace Maze
{
    //////////////////////////////////////////
    // DataBlockShared
    //
    //////////////////////////////////////////
    bool DataBlockShared::setString(HashedString _name, DataBlock::SharedStringId _id)
    {
#if MAZE_DEBUG
        auto it = m_strings.find(_name);
        MAZE_ERROR_RETURN_VALUE_IF(
            it != m_strings.end(),
            false,
            "%s is already exists with id=%d (_id=%d)",
            _name.getString().c_str(), (S32)it->second, (S32)_id);
#endif

        m_strings.emplace(_name, _id);
        return true;
    }

    //////////////////////////////////////////
    DataBlock::SharedStringId DataBlockShared::addString(HashedCString _name)
    {
        auto it = m_strings.find(_name);
        if (it != m_strings.end())
            return it->second;

        DataBlock::SharedStringId id = DataBlock::SharedStringId(++m_stringsIndexCounter);
        m_strings.emplace(_name, id);
        return id;
    }

    //////////////////////////////////////////
    DataBlock::SharedStringId DataBlockShared::getStringId(HashedCString _name)
    {
        auto it = m_strings.find(_name);
        if (it != m_strings.end())
            return it->second;

        return 0u;
    }

    //////////////////////////////////////////
    CString DataBlockShared::getCString(DataBlock::SharedStringId _index) const
    {
        for (auto it = m_strings.begin(), end = m_strings.end(); it != end; ++it)
            if (it->second == _index)
                return it->first.c_str();

        return nullptr;
    }

    //////////////////////////////////////////
    String const& DataBlockShared::getString(DataBlock::SharedStringId _index) const
    {
        static String nullValue;

        for (auto it = m_strings.begin(), end = m_strings.end(); it != end; ++it)
            if (it->second == _index)
                return it->first;

        return nullValue;
    }

    //////////////////////////////////////////
    HashedCString DataBlockShared::getNameHashedCString(DataBlock::SharedStringId _index) const
    {
        for (auto it = m_strings.begin(), end = m_strings.end(); it != end; ++it)
            if (it->second == _index)
                return it.key();

        return HashedCString();
    }

    //////////////////////////////////////////
    void DataBlockShared::clear()
    {
        m_strings.clear();
        m_stringsIndexCounter = 0;
    }

} // namespace Maze
//////////////////////////////////////////
