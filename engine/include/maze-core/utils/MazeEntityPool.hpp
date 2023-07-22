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
#if (!defined(_MazeEntityPool_hpp_))
#define _MazeEntityPool_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/helpers/MazeLogHelper.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/utils/MazeSharedObjectPool.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EntityPool);
    MAZE_USING_SHARED_PTR(EntityPoolObject);


    //////////////////////////////////////////
    // Class EntityPool
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EntityPool
        : public SharedObjectPool<Entity>
    {
    
    };


    //////////////////////////////////////////
    // Class EntityPoolObject
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EntityPoolObject
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EntityPoolObject, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(EntityPoolObject);

    public:

        //////////////////////////////////////////
        static EntityPoolObjectPtr Create(EntityPoolPtr const& _pool = nullptr);

        //////////////////////////////////////////
        virtual ~EntityPoolObject();

        //////////////////////////////////////////
        void release();

    protected:

        //////////////////////////////////////////
        EntityPoolObject();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(EntityPoolPtr const& _pool);

    private:

        EntityPoolWPtr m_pool;
    };

    
} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#endif // _MazeEntityPool_hpp_
