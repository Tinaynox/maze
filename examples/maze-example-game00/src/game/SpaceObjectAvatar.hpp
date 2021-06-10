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
#if (!defined(_SpaceObjectAvatar_hpp_))
#define _SpaceObjectAvatar_hpp_


//////////////////////////////////////////
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/utils/MazeComponentPool.hpp"
#include "game/SpaceObjectAvatar.hpp"
#include "game/DamageData.hpp"
#include "game/ProjectileAvatarType.hpp"
#include "maze-physics2d/ecs/components/MazeRigidbody2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SpaceObjectAvatar);
    MAZE_USING_SHARED_PTR(UnitPartRenderer);


    //////////////////////////////////////////
    // Class SpaceObjectAvatar
    //
    //////////////////////////////////////////
    class SpaceObjectAvatar
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SpaceObjectAvatar, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(SpaceObjectAvatar);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~SpaceObjectAvatar();

        //////////////////////////////////////////
        static SpaceObjectAvatarPtr Create();


        //////////////////////////////////////////
        void prepare();

        //////////////////////////////////////////
        void processDeath();

        //////////////////////////////////////////
        void processDamage();


        //////////////////////////////////////////
        inline Transform3DPtr const& getTransform() const { return m_transform; }


        //////////////////////////////////////////
        bool isDeathAnimationFinished();

    protected:

        //////////////////////////////////////////
        SpaceObjectAvatar();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processComponentAdded() MAZE_OVERRIDE;


        //////////////////////////////////////////
        void collectUnitPartRenderers();


    protected:
        Transform3DPtr m_transform;
        
        Vector<UnitPartRendererPtr> m_partRenderers;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _SpaceObjectAvatar_hpp_
//////////////////////////////////////////
