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
#if (!defined(_MazeTransformUtilsSystem_hpp_))
#define _MazeTransformUtilsSystem_hpp_


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
    MAZE_USING_SHARED_PTR(TransformUtilsSystem);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Transform3D);
    MAZE_USING_SHARED_PTR(Rotor3D);
    MAZE_USING_SHARED_PTR(LinearMovement3D);
    MAZE_USING_SHARED_PTR(SinMovement3D);


    //////////////////////////////////////////
    // Class TransformUtilsSystem
    //
    //////////////////////////////////////////
    class MAZE_CORE_API TransformUtilsSystem
        : public ComponentSystem
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(TransformUtilsSystem, ComponentSystem);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(TransformUtilsSystem);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~TransformUtilsSystem();

        //////////////////////////////////////////
        static TransformUtilsSystemPtr Create();


        //////////////////////////////////////////
        virtual S32 getOrder() const MAZE_OVERRIDE { return -200; }

    protected:

        //////////////////////////////////////////
        TransformUtilsSystem();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processSystemAdded() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processUpdate(F32 _dt) MAZE_OVERRIDE;

    protected:
        SharedPtr<GenericInclusiveEntitiesSample<Rotor3D>> m_rotors3D;
        SharedPtr<GenericInclusiveEntitiesSample<LinearMovement3D>> m_linearMovements3D;
        SharedPtr<GenericInclusiveEntitiesSample<SinMovement3D>> m_sinMovements3D;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTransformUtilsSystem_hpp_
//////////////////////////////////////////
