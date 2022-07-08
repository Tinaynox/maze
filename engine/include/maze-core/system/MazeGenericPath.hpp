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
#if (!defined(_MazeGenericPath_hpp_))
#define _MazeGenericPath_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    template <typename TPath, typename TString>
    class GenericPath
    {
    public:

        //////////////////////////////////////////
        using StringType = TString;
        using CharType = typename TString::value_type;

    public:

        //////////////////////////////////////////
        inline GenericPath() = default;

        //////////////////////////////////////////
        inline GenericPath(GenericPath const&) = default;

        //////////////////////////////////////////
        inline GenericPath(GenericPath&&) = default;

        //////////////////////////////////////////
        inline GenericPath(TString const& _path)
            : m_path(_path)
        {}

        //////////////////////////////////////////
        inline GenericPath(TString&& _path)
            : m_path(std::move(_path))
        {}

        //////////////////////////////////////////
        inline GenericPath(CharType const* _path)
        {
            if (_path)
                m_path = _path;
        }

        //////////////////////////////////////////
        inline GenericPath(CharType _char)
        {
            m_path.push_back(_char);
        }

        //////////////////////////////////////////
        virtual ~GenericPath() = default;

        //////////////////////////////////////////
        inline TPath& operator=(TPath const& _value)
        {
            setPath(_value.m_path);
            return *(TPath*)this;
        }

        //////////////////////////////////////////
        inline TPath& operator=(TPath&& _value)
        {
            setPath(std::move(_value.m_path));
            return *(TPath*)this;
        }


        //////////////////////////////////////////
        inline TPath operator+(TPath const& _value) const
        {
            return TPath(m_path + _value.m_path);
        }

        //////////////////////////////////////////
        inline bool operator==(TPath const& _value) const
        {
            return m_path == _value.m_path;
        }

        //////////////////////////////////////////
        inline bool operator!=(TPath const& _value) const
        {
            return !this->operator==(_value);
        }

        //////////////////////////////////////////
        inline TPath& operator+=(TPath const& _value)
        {
            setPath(m_path + _value.m_path);
            return *(TPath*)this;
        }

        //////////////////////////////////////////
        inline bool operator<(TPath const& _value) const
        {
            return getHash() < _value.getHash();
        }

        //////////////////////////////////////////
        inline CharType operator[](Size _index) const
        {
            return m_path[_index];
        }

        //////////////////////////////////////////
        inline TString const& getPath() const { return m_path; }

        //////////////////////////////////////////
        inline CharType const* c_str() const
        {
            return m_path.c_str();
        }

        //////////////////////////////////////////
        inline CharType front() const { return m_path.front(); }

        //////////////////////////////////////////
        inline CharType back() const { return m_path.back(); }

        //////////////////////////////////////////
        inline bool empty() const { return m_path.empty(); }

        //////////////////////////////////////////
        inline Size size() const { return m_path.size(); }

        //////////////////////////////////////////
        inline void clear() { return setPath(TString()); }

        //////////////////////////////////////////
        inline operator TString() const { return m_path; }

        //////////////////////////////////////////
        inline U32 getHash()
        {
            if (m_hashDirty)
            {
                m_hash = CalculateFNV1(m_path.c_str());
                m_hashDirty = false;
            }
            return m_hash;
        }

        //////////////////////////////////////////
        inline U32 getHash() const
        {
            return const_cast<TPath*>((TPath const*)this)->getHash();
        }

        //////////////////////////////////////////
        virtual String toUTF8() const MAZE_ABSTRACT;

    protected:

        //////////////////////////////////////////
        inline void setPath(TString const& _path)
        {
            m_path = _path;
            m_hashDirty = true;
        }

        //////////////////////////////////////////
        inline void setPath(TString&& _path)
        {
            m_path = std::move(_path);
            m_hashDirty = true;
        }

    protected:
        TString m_path;
        bool m_hashDirty = true;
        U32 m_hash = 0u;
    };

    
} // namespace Maze
//////////////////////////////////////////
    

#endif // _MazeGenericPath_hpp_
//////////////////////////////////////////
