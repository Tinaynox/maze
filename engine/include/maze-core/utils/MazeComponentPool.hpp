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
#if (!defined(_MazeComponentPool_hpp_))
#define _MazeComponentPool_hpp_


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
    // Class ComponentPoolObject
    //
    //////////////////////////////////////////
    template <typename TComponent>
    class MAZE_CORE_API ComponentPoolObject
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ComponentPoolObject<TComponent>, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_AND_IMPLEMENT_MEMORY_ALLOCATION_DEFAULT(ComponentPoolObject<TComponent>);

    public:

        //////////////////////////////////////////
        static SharedPtr<ComponentPoolObject> Create(SharedPtr<SharedObjectPool<TComponent>> const& _pool = nullptr);

        //////////////////////////////////////////
        virtual ~ComponentPoolObject();

        //////////////////////////////////////////
        void release(SharedPtr<TComponent> const& _component);

    protected:

        //////////////////////////////////////////
        ComponentPoolObject();

        //////////////////////////////////////////
        bool init(SharedPtr<SharedObjectPool<TComponent>> const& _pool);

    private:

        SharedPtr<SharedObjectPool<TComponent>> m_pool;
    };


    //////////////////////////////////////////
    template <typename TComponent>
    ComponentPoolObject<TComponent>::ComponentPoolObject()
    {
    }

    //////////////////////////////////////////
    template <typename TComponent>
    ComponentPoolObject<TComponent>::~ComponentPoolObject()
    {
    }

    //////////////////////////////////////////
    template <typename TComponent>
    SharedPtr<ComponentPoolObject<TComponent>> ComponentPoolObject<TComponent>::Create(SharedPtr<SharedObjectPool<TComponent>> const& _pool)
    {
        SharedPtr<ComponentPoolObject<TComponent>> object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ComponentPoolObject<TComponent>, object, init(_pool));
        return object;
    }

    //////////////////////////////////////////
    template <typename TComponent>
    bool ComponentPoolObject<TComponent>::init(SharedPtr<SharedObjectPool<TComponent>> const& _pool)
    {
        m_pool = _pool;
        return true;
    }

    //////////////////////////////////////////
    template <typename TComponent>
    void ComponentPoolObject<TComponent>::release(SharedPtr<TComponent> const& _component)
    {
        m_pool->release(_component);
    }

    
} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#endif // _MazeComponentPool_hpp_
