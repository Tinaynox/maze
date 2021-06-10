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
#if (!defined(_MazeSingleton_hpp_))
#define _MazeSingleton_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/hash/MazeHashCRC.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/system/MazeMutex.hpp"
#include <cstring>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    #pragma warning(push)
    #pragma warning(disable : 4146)


    //////////////////////////////////////////
    // Class Singleton
    //
    //////////////////////////////////////////
    template <typename T>
    class MAZE_CORE_API Singleton
    {
    public:

        //////////////////////////////////////////
        virtual ~Singleton()
        {
        }

        //////////////////////////////////////////
        Singleton(Singleton const&) = delete;

        //////////////////////////////////////////
        Singleton(Singleton&&) = delete;

        //////////////////////////////////////////
        Singleton& operator=(Singleton const&) = delete;

        //////////////////////////////////////////
        Singleton& operator=(Singleton&&) = delete;


        //////////////////////////////////////////
        static inline T& GetInstance()
        {
            return (*GetInstancePtr().get());
        }

        //////////////////////////////////////////
        static inline SharedPtr<T> const& GetInstancePtr()
        {
            if (!s_instance)
            {
                MAZE_MUTEX_SCOPED_LOCK(s_mutex);
                if (!s_instance)
                {
                    s_instance = std::make_shared<T>();
                }
            }

            return s_instance;
        }

        //////////////////////////////////////////
        static inline bool IsInstanceValid()
        {
            return (s_instance != nullptr);
        }

        //////////////////////////////////////////
        static void DestroyInstance()
        {
            s_instance.reset();
        }

    protected:

        ////////////////////////////////////
        Singleton()
        {

        }


    protected:

        static SharedPtr<T> s_instance;
        static Mutex s_mutex;
    };


    //////////////////////////////////////////
    #pragma warning(pop)


} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#endif // _MazeSingleton_hpp_
