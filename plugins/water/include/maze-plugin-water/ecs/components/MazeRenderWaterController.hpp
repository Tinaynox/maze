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
#if (!defined(_MazeRenderWaterController_hpp_))
#define _MazeRenderWaterController_hpp_


//////////////////////////////////////////
#include "maze-plugin-water/MazeWaterHeader.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeWaterRenderer3D.hpp"
#include "maze-graphics/ecs/events/MazeECSGraphicsEvents.hpp"
#include "maze-graphics/ecs/components/MazeRenderController.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderSystem);
    MAZE_USING_SHARED_PTR(EntitiesSample);
    MAZE_USING_SHARED_PTR(RenderWaterController);
    MAZE_USING_SHARED_PTR(Camera3D);
    MAZE_USING_SHARED_PTR(MeshRenderer);
    MAZE_USING_SHARED_PTR(Transform3D);
    

    //////////////////////////////////////////
    // Class RenderWaterController
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_WATER_API RenderWaterController
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(RenderWaterController, ComponentSystem);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(RenderWaterController);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~RenderWaterController();

        //////////////////////////////////////////
        static RenderWaterControllerPtr Create();


        //////////////////////////////////////////
        inline void setEnabled(bool _value) { m_enabled = _value; }

        //////////////////////////////////////////
        inline bool getEnabled() const { return m_enabled; }


        //////////////////////////////////////////
        inline RenderBufferPtr const& getReflectionBuffer() const { return m_reflectionBuffer; }

        //////////////////////////////////////////
        inline RenderBufferPtr const& getRefractionBuffer() const { return m_refractionBuffer; }

        
        //////////////////////////////////////////
        void processPrePass(
            RenderTarget* _renderTarget,
            DefaultPassParams const& _params);

        //////////////////////////////////////////
        void resizeBuffers(Vec2U32 const& _size);

    protected:

        //////////////////////////////////////////
        RenderWaterController();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;



        //////////////////////////////////////////
        void createBuffers(Vec2U32 const& _size);


        //////////////////////////////////////////
        void notifyComponentSystemAdded(ComponentSystemPtr const& _system);

    protected:
        SharedPtr<GenericInclusiveEntitiesSample<WaterRenderer3D>> m_waterRenderersSample;
        SharedPtr<GenericInclusiveEntitiesSample<RenderController>> m_renderControllerSample;

        bool m_enabled = true;
        RenderBufferPtr m_reflectionBuffer;
        RenderBufferPtr m_refractionBuffer;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderWaterController_hpp_
//////////////////////////////////////////
