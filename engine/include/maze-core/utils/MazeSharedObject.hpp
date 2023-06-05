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
#if (!defined(_MazeSharedObject_hpp_))
#define _MazeSharedObject_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SharedObject
    //
    //////////////////////////////////////////
    template <typename T>
    class SharedObject
        : public std::enable_shared_from_this<T>
    {
    public:
        
        //////////////////////////////////////////
        using TPtr = SharedPtr<T>;
        using TWPtr = WeakPtr<T>;
        
    public:
        
        //////////////////////////////////////////
        virtual ~SharedObject() = default;
        
        //////////////////////////////////////////
        SharedObject(SharedObject const&) = delete;
        
        //////////////////////////////////////////
        SharedObject(SharedObject&&) = delete;
        
        //////////////////////////////////////////
        SharedObject& operator=(SharedObject const&) = delete;
        
        //////////////////////////////////////////
        SharedObject& operator=(SharedObject&&) = delete;
        
        //////////////////////////////////////////
        template <class U>
        inline SharedPtr<U> cast()
        { 
            MAZE_DEBUG_BP_IF(!getSharedPtr());
            return std::static_pointer_cast<U>(getSharedPtr()); 
        }

        //////////////////////////////////////////
        template <class U>
        inline WeakPtr<U> castWeak()
        {
            return this->cast<U>();
        }
        
        //////////////////////////////////////////
        template <class U>
        inline const U* castRaw() const
        { 
            return static_cast<const U*>(this); 
        }
        
        //////////////////////////////////////////
        template <class U>
        inline U* castRaw()
        { 
            return static_cast<U*>(this); 
        }
        
        //////////////////////////////////////////
        inline TPtr getSharedPtr()
        {
            return this->shared_from_this();
        }
        
    protected:
        
        //////////////////////////////////////////
        SharedObject() = default;
    };

    
} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#endif // _MazeSharedObject_hpp_
