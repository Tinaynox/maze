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
#if (!defined(_MazeStringKeyMap_hpp_))
#define _MazeStringKeyMap_hpp_


//////////////////////////////////////////
// Include
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"
#include "maze-core/data/MazeHashedCString.hpp"
#include <cstring>
#include <cassert>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class StringKeyMap
    //
    //////////////////////////////////////////
    template <typename TValue>
    class StringKeyMap
    {
    public:

        //////////////////////////////////////////
        using MapType = UnorderedMap<HashedCString, Pair<String, TValue>>;

        //////////////////////////////////////////
        class StringKeyMapIterator
        {
        public:

            //////////////////////////////////////////
            StringKeyMapIterator()
            {}

            //////////////////////////////////////////
            StringKeyMapIterator(typename MapType::iterator&& _iterator)
                : m_iterator(_iterator)
            {}

            //////////////////////////////////////////
            inline StringKeyMapIterator& operator++() { ++m_iterator; return (*this); }

            //////////////////////////////////////////
            inline StringKeyMapIterator& operator--() { --m_iterator; return (*this); }

            //////////////////////////////////////////
            inline bool operator!=(StringKeyMapIterator const& _other) const { return m_iterator != _other.m_iterator; }

            //////////////////////////////////////////
            inline bool operator==(StringKeyMapIterator const& _other) const { return m_iterator == _other.m_iterator; }

            //////////////////////////////////////////
            inline HashedCString key() const { return m_iterator->first; }

            //////////////////////////////////////////
            inline TValue& value() { return m_iterator->second.second; }

            //////////////////////////////////////////
            inline TValue& value() const { return m_iterator->second.second; }

            //////////////////////////////////////////
            TValue& operator*() { return value(); }

            //////////////////////////////////////////
            const TValue& operator*() const { return value(); }

            //////////////////////////////////////////
            TValue* operator->() { return &m_iterator->second.second; }

        protected:
            typename MapType::iterator m_iterator;
        };


        //////////////////////////////////////////
        using iterator = StringKeyMapIterator;

    public:


        //////////////////////////////////////////
        inline iterator begin()
        {
            return m_map.begin();
        }

        //////////////////////////////////////////
        inline iterator end()
        {
            return m_map.end();
        }


        //////////////////////////////////////////
        inline void insert(String const& _key, TValue&& _value) { insert(MAZE_HASHED_CSTRING(_key.c_str()), std::move(_value)); }

        //////////////////////////////////////////
        inline void insert(String const& _key, TValue const& _value) { insert(MAZE_HASHED_CSTRING(_key.c_str()), _value); }

        //////////////////////////////////////////
        inline TValue* insert(CString _key, TValue&& _value) { return insert(MAZE_HASHED_CSTRING(_key), std::move(_value)); }

        //////////////////////////////////////////
        inline TValue* insert(CString _key, TValue const& _value) { return insert(MAZE_HASHED_CSTRING(_key), _value); }

        //////////////////////////////////////////
        inline TValue* insert(HashedCString _key, TValue&& _value)
        {
            auto it = m_map.find(_key);
            if (it == m_map.end())
            {
                Pair<String, TValue> valueData = std::make_pair<String, TValue>(
                    _key.str,
                    std::move(_value));

                auto at = m_map.emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(_key),
                    std::forward_as_tuple(std::move(valueData)));

                if (at.second)
                {
                    HashedCString* newKey = const_cast<HashedCString*>(&at.first->first);
                    newKey->str = &at.first->second.first[0];

                    return &at.first->second.second;
                }

                return nullptr;
            }
            else
            {
                it->second.second = std::move(_value);
                return &it->second.second;
            }
        }

        //////////////////////////////////////////
        inline TValue* insert(HashedCString _key, TValue const& _value)
        {
            auto it = m_map.find(_key);
            if (it == m_map.end())
            {
                Pair<String, TValue> valueData = Pair<String, TValue>(
                    _key.str,
                    _value);

                auto at = m_map.emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(_key),
                    std::forward_as_tuple(std::move(valueData)));

                HashedCString* newKey = const_cast<HashedCString*>(&at.first->first);
                newKey->str = &at.first->second.first[0];
            }
            else
            {
                it->second.second = _value;
            }
        }


        //////////////////////////////////////////
        inline void erase(String const& _key) { erase(MAZE_HASHED_CSTRING(_key.c_str())); }

        //////////////////////////////////////////
        inline void erase(CString _key) { erase(MAZE_HASHED_CSTRING(_key)); }

        //////////////////////////////////////////
        inline void erase(HashedCString _key) { m_map.erase(_key); }


        //////////////////////////////////////////
        inline TValue const* find(String const& _key) const { return find(MAZE_HASHED_CSTRING(_key.c_str())); }

        //////////////////////////////////////////
        inline TValue const* find(CString _key) const { return find(MAZE_HASHED_CSTRING(_key)); }

        //////////////////////////////////////////
        inline TValue const* find(HashedCString _key) const
        {
            auto it = m_map.find(_key);
            if (it != m_map.end())
                return &it->second.second;

            return nullptr;
        }

        //////////////////////////////////////////
        inline TValue* find(String const& _key) { return find(MAZE_HASHED_CSTRING(_key.c_str())); }

        //////////////////////////////////////////
        inline TValue* find(CString _key) { return find(MAZE_HASHED_CSTRING(_key)); }

        //////////////////////////////////////////
        inline TValue* find(HashedCString _key)
        {
            auto it = m_map.find(_key);
            if (it != m_map.end())
                return &it->second.second;

            return nullptr;
        }


        //////////////////////////////////////////
        inline TValue& ensure(String const& _key) { return ensure(MAZE_HASHED_CSTRING(_key.c_str())); }

        //////////////////////////////////////////
        inline TValue& ensure(CString _key) { return ensure(MAZE_HASHED_CSTRING(_key)); }

        //////////////////////////////////////////
        inline TValue& ensure(HashedCString _key)
        {
            TValue* value = find(_key);
            if (value)
                return *value;
            value = insert(_key, TValue());
            return *value;
        }


        //////////////////////////////////////////
        inline TValue& operator[](String const& _key) { return ensure(_key); }

        //////////////////////////////////////////
        inline TValue& operator[](CString _key) { return ensure(_key); }

        //////////////////////////////////////////
        inline TValue& operator[](HashedCString _key) { return ensure(_key); }

        //////////////////////////////////////////
        inline TValue const& operator[](String const& _key) const
        {
            TValue const* value = find(_key);
            MAZE_ASSERT(value != nullptr);
            return *value;
        }

        //////////////////////////////////////////
        inline TValue const& operator[](CString _key) const
        {
            TValue const* value = find(_key);
            MAZE_ASSERT(value != nullptr);
            return *value;
        }

        //////////////////////////////////////////
        inline TValue const& operator[](HashedCString _key) const
        {
            TValue const* value = find(_key);
            MAZE_ASSERT(value != nullptr);
            return *value;
        }


        //////////////////////////////////////////
        inline void clear() { m_map.clear(); }

        //////////////////////////////////////////
        inline void iterate(std::function<bool(HashedCString, TValue&)> _func)
        {
            for (MapType::iterator it = m_map.begin(),
                                   end = m_map.end();
                                   it != end;
                                   ++it)
            {
                if (!_func(it->first, it->second.second))
                    break;
            }
        }

        //////////////////////////////////////////
        inline void iterate(std::function<bool(HashedCString, TValue const&)> _func) const
        {
            for (MapType::const_iterator it = m_map.begin(),
                                         end = m_map.end();
                                         it != end;
                                         ++it)
            {
                if (!_func(it->first, it->second.second))
                    break;
            }
        }

    private:
        MapType m_map;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeStringKeyMap_h_
//////////////////////////////////////////
