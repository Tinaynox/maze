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
#include "maze-core/data/MazeHashedString.hpp"
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
        using MapType = UnorderedMap<HashedCString, Pair<String const, TValue>>;

        //////////////////////////////////////////
        class StringKeyMapIterator
        {
        public:

            //////////////////////////////////////////
            friend class StringKeyMap<TValue>;

            //////////////////////////////////////////
            friend class StringKeyMapConstIterator;

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
            inline Pair<String const, TValue>& operator*() { return m_iterator->second; }

            //////////////////////////////////////////
            inline Pair<String const, TValue> const& operator*() const { return m_iterator->second; }

            //////////////////////////////////////////
            inline Pair<String const, TValue>* operator->() { return &m_iterator->second; }

            //////////////////////////////////////////
            inline Pair<String const, TValue> const* operator->() const { return &m_iterator->second; }

        protected:
            typename MapType::iterator m_iterator;
        };

        //////////////////////////////////////////
        class StringKeyMapConstIterator
        {
        public:

            //////////////////////////////////////////
            friend class StringKeyMap<TValue>;

        public:

            //////////////////////////////////////////
            StringKeyMapConstIterator()
            {}

            //////////////////////////////////////////
            StringKeyMapConstIterator(typename MapType::const_iterator&& _iterator)
                : m_iterator(_iterator)
            {}

            //////////////////////////////////////////
            StringKeyMapConstIterator(typename MapType::iterator&& _iterator)
                : m_iterator(_iterator)
            {}

            //////////////////////////////////////////
            StringKeyMapConstIterator(StringKeyMapIterator const& _iterator)
                : m_iterator(_iterator.m_iterator)
            {}

            //////////////////////////////////////////
            inline StringKeyMapConstIterator& operator++() { ++m_iterator; return (*this); }

            //////////////////////////////////////////
            inline StringKeyMapConstIterator& operator--() { --m_iterator; return (*this); }

            //////////////////////////////////////////
            inline bool operator!=(StringKeyMapConstIterator const& _other) const { return m_iterator != _other.m_iterator; }

            //////////////////////////////////////////
            inline bool operator==(StringKeyMapConstIterator const& _other) const { return m_iterator == _other.m_iterator; }

            //////////////////////////////////////////
            inline HashedCString key() const { return m_iterator->first; }

            //////////////////////////////////////////
            inline TValue& value() const { return m_iterator->second.second; }

            //////////////////////////////////////////
            inline Pair<String const, TValue> const& operator*() const { return m_iterator->second; }

            //////////////////////////////////////////
            inline Pair<String const, TValue> const* operator->() const { return &m_iterator->second; }

        protected:
            typename MapType::const_iterator m_iterator;
        };


        //////////////////////////////////////////
        using iterator = StringKeyMapIterator;

        //////////////////////////////////////////
        using const_iterator = StringKeyMapConstIterator;

    public:


        //////////////////////////////////////////
        inline iterator begin() { return m_map.begin(); }

        //////////////////////////////////////////
        inline const_iterator begin() const { return m_map.begin(); }

        //////////////////////////////////////////
        inline iterator end() { return m_map.end(); }

        //////////////////////////////////////////
        inline const_iterator end() const { return m_map.end(); }


        //////////////////////////////////////////
        inline TValue* insert(String const& _key, TValue&& _value) { return insert(MAZE_HASHED_CSTRING(_key.c_str()), std::move(_value)); }

        //////////////////////////////////////////
        inline TValue* insert(String const& _key, TValue const& _value) { return insert(MAZE_HASHED_CSTRING(_key.c_str()), _value); }

        //////////////////////////////////////////
        inline TValue* insert(CString _key, TValue&& _value) { return insert(MAZE_HASHED_CSTRING(_key), std::move(_value)); }

        //////////////////////////////////////////
        inline TValue* insert(CString _key, TValue const& _value) { return insert(MAZE_HASHED_CSTRING(_key), _value); }

        //////////////////////////////////////////
        inline TValue* insert(HashedString const& _key, TValue&& _value) { return insert(_key.asHashedCString(), std::move(_value)); }

        //////////////////////////////////////////
        inline TValue* insert(HashedString const& _key, TValue const& _value) { return insert(_key.asHashedCString(), _value); }

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
                it->second.second = _value;
                return &it->second.second;
            }
        }


        //////////////////////////////////////////
        inline void erase(String const& _key) { erase(MAZE_HASHED_CSTRING(_key.c_str())); }

        //////////////////////////////////////////
        inline void erase(CString _key) { erase(MAZE_HASHED_CSTRING(_key)); }

        //////////////////////////////////////////
        inline void erase(HashedString const& _key) { erase(_key.asHashedCString()); }

        //////////////////////////////////////////
        inline void erase(HashedCString _key) { m_map.erase(_key); }

        //////////////////////////////////////////
        inline iterator erase(iterator const& _it) { return m_map.erase(_it.m_iterator); }


        //////////////////////////////////////////
        inline bool empty() { return m_map.empty(); }


        //////////////////////////////////////////
        inline TValue const* tryGet(String const& _key) const { return tryGet(MAZE_HASHED_CSTRING(_key.c_str())); }

        //////////////////////////////////////////
        inline TValue const* tryGet(CString _key) const { return tryGet(MAZE_HASHED_CSTRING(_key)); }

        //////////////////////////////////////////
        inline TValue const* tryGet(HashedString const& _key) const { return tryGet(_key.asHashedCString()); }

        //////////////////////////////////////////
        inline TValue const* tryGet(HashedCString _key) const
        {
            auto it = m_map.find(_key);
            if (it != m_map.end())
                return &it->second.second;

            return nullptr;
        }

        //////////////////////////////////////////
        inline TValue* tryGet(String const& _key) { return tryGet(MAZE_HASHED_CSTRING(_key.c_str())); }

        //////////////////////////////////////////
        inline TValue* tryGet(CString _key) { return tryGet(MAZE_HASHED_CSTRING(_key)); }

        //////////////////////////////////////////
        inline TValue* tryGet(HashedString const& _key) { return tryGet(_key.asHashedCString()); }

        //////////////////////////////////////////
        inline TValue* tryGet(HashedCString _key)
        {
            auto it = m_map.find(_key);
            if (it != m_map.end())
                return &it->second.second;

            return nullptr;
        }

        //////////////////////////////////////////
        inline iterator find(String const& _key) { return find(MAZE_HASHED_CSTRING(_key.c_str())); }

        //////////////////////////////////////////
        inline iterator find(CString _key) { return find(MAZE_HASHED_CSTRING(_key)); }

        //////////////////////////////////////////
        inline iterator find(HashedString const& _key) { return find(_key.asHashedCString()); }

        //////////////////////////////////////////
        inline iterator find(HashedCString _key) { return m_map.find(_key); }

        //////////////////////////////////////////
        inline const_iterator find(String const& _key) const { return find(MAZE_HASHED_CSTRING(_key.c_str())); }

        //////////////////////////////////////////
        inline const_iterator find(CString _key) const { return find(MAZE_HASHED_CSTRING(_key)); }

        //////////////////////////////////////////
        inline const_iterator find(HashedString const& _key) const { return find(_key.asHashedCString()); }

        //////////////////////////////////////////
        inline const_iterator find(HashedCString _key) const { return m_map.find(_key); }


        //////////////////////////////////////////
        inline TValue& ensure(String const& _key) { return ensure(MAZE_HASHED_CSTRING(_key.c_str())); }

        //////////////////////////////////////////
        inline TValue& ensure(CString _key) { return ensure(MAZE_HASHED_CSTRING(_key)); }

        //////////////////////////////////////////
        inline TValue& ensure(HashedString const& _key) { return ensure(_key.asHashedCString()); }

        //////////////////////////////////////////
        inline TValue& ensure(HashedCString _key)
        {
            TValue* value = tryGet(_key);
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
        inline TValue& operator[](HashedString _key) { return ensure(_key.asHashedCString()); }

        //////////////////////////////////////////
        inline TValue& operator[](HashedCString _key) { return ensure(_key); }

        //////////////////////////////////////////
        inline TValue const& operator[](String const& _key) const
        {
            TValue const* value = tryGet(_key);
            MAZE_ASSERT(value != nullptr);
            return *value;
        }

        //////////////////////////////////////////
        inline TValue const& operator[](CString _key) const
        {
            TValue const* value = tryGet(_key);
            MAZE_ASSERT(value != nullptr);
            return *value;
        }

        //////////////////////////////////////////
        inline TValue const& operator[](HashedString const& _key) const
        {
            return this->operator[](_key.asHashedCString());
        }

        //////////////////////////////////////////
        inline TValue const& operator[](HashedCString _key) const
        {
            TValue const* value = tryGet(_key);
            MAZE_ASSERT(value != nullptr);
            return *value;
        }


        //////////////////////////////////////////
        inline void clear() { m_map.clear(); }

        //////////////////////////////////////////
        inline void iterate(std::function<bool(HashedCString, TValue&)> _func)
        {
            for (typename MapType::iterator it = m_map.begin(),
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
            for (typename MapType::const_iterator it = m_map.begin(),
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


//////////////////////////////////////////
template <typename TValue>
inline bool operator!=(
    typename Maze::StringKeyMap<TValue>::StringKeyMapIterator const& _it0,
    typename Maze::StringKeyMap<TValue>::StringKeyMapConstIterator const& _it1)
{ return _it0.m_iterator != _it1.m_iterator; }

//////////////////////////////////////////
template <typename TValue>
inline bool operator==(
    typename Maze::StringKeyMap<TValue>::StringKeyMapIterator const& _it0,
    typename Maze::StringKeyMap<TValue>::StringKeyMapConstIterator const& _it1)
{ return _it0.m_iterator == _it1.m_iterator; }

//////////////////////////////////////////
template <typename TValue>
inline bool operator!=(
    typename Maze::StringKeyMap<TValue>::StringKeyMapConstIterator const& _it0,
    typename Maze::StringKeyMap<TValue>::StringKeyMapIterator const& _it1)
{
    return _it0.m_iterator != _it1.m_iterator;
}

//////////////////////////////////////////
template <typename TValue>
inline bool operator==(
    typename Maze::StringKeyMap<TValue>::StringKeyMapConstIterator const& _it0,
    typename Maze::StringKeyMap<TValue>::StringKeyMapIterator const& _it1)
{
    return _it0.m_iterator == _it1.m_iterator;
}


#endif // _MazeStringKeyMap_h_
//////////////////////////////////////////
