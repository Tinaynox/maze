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
#if (!defined(_MazeBounds2DSystem_hpp_))
#define _MazeBounds2DSystem_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EntitiesSample);
    MAZE_USING_SHARED_PTR(Bounds2DSystem);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Transform3D);
    MAZE_USING_SHARED_PTR(Bounds2D);


    //////////////////////////////////////////
    // Class Bounds2DSystem
    //
    //////////////////////////////////////////
    class MAZE_CORE_API Bounds2DSystem
        : public ComponentSystem
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Bounds2DSystem, ComponentSystem);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Bounds2DSystem);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~Bounds2DSystem();

        //////////////////////////////////////////
        static Bounds2DSystemPtr Create();


        //////////////////////////////////////////
        virtual S32 getOrder() const MAZE_OVERRIDE { return 30000; }

    protected:

        //////////////////////////////////////////
        Bounds2DSystem();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processSystemAdded() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processUpdate(F32 _dt) MAZE_OVERRIDE;

    protected:
        SharedPtr<GenericInclusiveEntitiesSample<Bounds2D>> m_bounds2D;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeBounds2DSystem_hpp_
//////////////////////////////////////////
