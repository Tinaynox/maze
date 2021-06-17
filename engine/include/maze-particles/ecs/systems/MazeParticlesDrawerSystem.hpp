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
#if (!defined(_MazeParticlesDrawerSystem_hpp_))
#define _MazeParticlesDrawerSystem_hpp_


//////////////////////////////////////////
#include "maze-particles/MazeParticlesHeader.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystemModule3D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderSystem);
    MAZE_USING_SHARED_PTR(ParticlesDrawerSystem);
    MAZE_USING_SHARED_PTR(RenderControlSystem);
    MAZE_USING_SHARED_PTR(ParticleSystem2D);
    MAZE_USING_SHARED_PTR(ParticleSystem3D);
    

    //////////////////////////////////////////
    // Class ParticlesDrawerSystem
    //
    //////////////////////////////////////////
    class MAZE_PARTICLES_API ParticlesDrawerSystem
        : public ComponentSystem
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ParticlesDrawerSystem, ComponentSystem);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ParticlesDrawerSystem);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~ParticlesDrawerSystem();

        //////////////////////////////////////////
        static ParticlesDrawerSystemPtr Create(RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        virtual S32 getOrder() const MAZE_OVERRIDE { return 45000; }

        
    protected:

        //////////////////////////////////////////
        ParticlesDrawerSystem();

        //////////////////////////////////////////
        bool init(RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        virtual void processSystemAdded() MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void processUpdate(F32 _dt) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyGatherRenderUnits(
            RenderTarget* _renderTarget,
            DefaultPassParams const& _params,
            Vector<RenderUnit>& _renderData);

    protected:
        RenderSystemPtr m_renderSystem;

        RenderControlSystemPtr m_renderControlSystem;

        SharedPtr<GenericInclusiveEntitiesSample<ParticleSystem3D>> m_particleSystem3DSample;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeParticlesDrawerSystem_hpp_
//////////////////////////////////////////
