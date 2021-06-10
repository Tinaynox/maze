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
#if (!defined(_SpaceObjectAI_SinMovement_hpp_))
#define _SpaceObjectAI_SinMovement_hpp_


//////////////////////////////////////////
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/utils/MazeComponentPool.hpp"
#include "game/DamageData.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SpaceObject);
    MAZE_USING_SHARED_PTR(SpaceObjectAI_SinMovement);


    //////////////////////////////////////////
    // Class SpaceObjectAI_SinMovement
    //
    //////////////////////////////////////////
    class SpaceObjectAI_SinMovement
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SpaceObjectAI_SinMovement, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(SpaceObjectAI_SinMovement);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~SpaceObjectAI_SinMovement();

        //////////////////////////////////////////
        static SpaceObjectAI_SinMovementPtr Create();


        //////////////////////////////////////////
        void update(F32 _dt);

        //////////////////////////////////////////
        SpaceObjectPtr const& getSpaceObject() const { return m_spaceObject; }

    protected:

        //////////////////////////////////////////
        SpaceObjectAI_SinMovement();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processComponentAdded() MAZE_OVERRIDE;

    protected:
        SpaceObjectPtr m_spaceObject;

        F32 m_timer;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _SpaceObjectAI_SinMovement_hpp_
//////////////////////////////////////////
