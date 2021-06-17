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
#if (!defined(_MazeRenderWaterSystem_hpp_))
#define _MazeRenderWaterSystem_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeWaterRenderer3D.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystemModule3D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderSystem);
    MAZE_USING_SHARED_PTR(EntitiesSample);
    MAZE_USING_SHARED_PTR(RenderWaterSystem);
    MAZE_USING_SHARED_PTR(Camera3D);
    MAZE_USING_SHARED_PTR(MeshRenderer);
    MAZE_USING_SHARED_PTR(Transform3D);
    

    //////////////////////////////////////////
    // Class RenderWaterSystem
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API RenderWaterSystem
        : public ComponentSystem
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(RenderWaterSystem, ComponentSystem);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(RenderWaterSystem);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~RenderWaterSystem();

        //////////////////////////////////////////
        static RenderWaterSystemPtr Create();

        //////////////////////////////////////////
        virtual S32 getOrder() const MAZE_OVERRIDE { return 46000; }


        //////////////////////////////////////////
        inline RenderBufferPtr const& getReflectionBuffer() const { return m_reflectionBuffer; }

        //////////////////////////////////////////
        inline RenderBufferPtr const& getRefractionBuffer() const { return m_refractionBuffer; }

    protected:

        //////////////////////////////////////////
        RenderWaterSystem();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processSystemAdded() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processUpdate(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        void notifyRenderPrePass(
            RenderTarget* _renderTarget,
            DefaultPassParams const& _params);

    protected:
        SharedPtr<GenericInclusiveEntitiesSample<WaterRenderer3D>> m_waterRenderersSample;

        RenderControlSystemPtr m_renderControlSystem;

        RenderBufferPtr m_reflectionBuffer;
        RenderBufferPtr m_refractionBuffer;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderWaterSystem_hpp_
//////////////////////////////////////////
