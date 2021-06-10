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
#if (!defined(_MazeThreadLocalPointer_hpp_))
#define _MazeThreadLocalPointer_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/system/MazeThreadLocalVariable.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ThreadLocalPointer
    //
    //////////////////////////////////////////
    template <typename T>
    class ThreadLocalPointer
    {
    public:
        
        //////////////////////////////////////////
        ThreadLocalPointer(T* _value = nullptr);

        //////////////////////////////////////////
        ~ThreadLocalPointer();

        //////////////////////////////////////////
        T& operator*() const;

        //////////////////////////////////////////
        T* operator->() const;

        //////////////////////////////////////////
        operator T*() const;

        //////////////////////////////////////////
        ThreadLocalPointer<T>& operator=(T* _value);

        //////////////////////////////////////////
        ThreadLocalPointer<T>& operator=(T const* _value);

        //////////////////////////////////////////
        ThreadLocalPointer<T>& operator=(ThreadLocalPointer<T> const& _right);

        //////////////////////////////////////////
        void setValue(T* _value);

        //////////////////////////////////////////
        void setValue(T const* _value);

        //////////////////////////////////////////
        T* getValue() const;

    protected:
        ThreadLocalVariable m_localVariable;
    };


    //////////////////////////////////////////
    template <typename T>
    ThreadLocalPointer<T>::ThreadLocalPointer(T* _value) 
    {
        setValue(_value);
    }

    //////////////////////////////////////////
    template <typename T>
    ThreadLocalPointer<T>::~ThreadLocalPointer() 
    {
    }

    //////////////////////////////////////////
    template <typename T>
    T& ThreadLocalPointer<T>::operator*() const
    {
        return *static_cast<T*>(getValue());
    }

    //////////////////////////////////////////
    template <typename T>
    T* ThreadLocalPointer<T>::operator->() const
    {
        return static_cast<T*>(getValue());
    }

    //////////////////////////////////////////
    template <typename T>
    ThreadLocalPointer<T>::operator T*() const
    {
        return static_cast<T*>(getValue());
    }

    //////////////////////////////////////////
    template <typename T>
    ThreadLocalPointer<T>& ThreadLocalPointer<T>::operator=(T* _value)
    {
        setValue(_value);
        return *this;
    }

    //////////////////////////////////////////
    template <typename T>
    ThreadLocalPointer<T>& ThreadLocalPointer<T>::operator=(T const* _value)
    {
        setValue(_value);
        return *this;
    }

    //////////////////////////////////////////
    template <typename T>
    ThreadLocalPointer<T>& ThreadLocalPointer<T>::operator=(ThreadLocalPointer<T> const& _right)
    {
        setValue(_right.getValue());
        return *this;
    }

    //////////////////////////////////////////
    template <typename T>
    void ThreadLocalPointer<T>::setValue(T* _value)
    {
        m_localVariable.setValue(reinterpret_cast<void*>(_value));
    }

    //////////////////////////////////////////
    template <typename T>
    void ThreadLocalPointer<T>::setValue(T const* _value)
    {
        m_localVariable.setValue((void*)(_value));
    }


    //////////////////////////////////////////
    template <typename T>
    T* ThreadLocalPointer<T>::getValue() const
    {
        return reinterpret_cast<T*>(m_localVariable.getValue());
    }

    
} // namespace Maze
//////////////////////////////////////////
    

#endif // _MazeThreadLocalPointer_hpp_
//////////////////////////////////////////
