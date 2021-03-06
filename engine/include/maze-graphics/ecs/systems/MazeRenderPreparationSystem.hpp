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
#if (!defined(_MazeRenderPreparationSystem_hpp_))
#define _MazeRenderPreparationSystem_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderSystem);
    MAZE_USING_SHARED_PTR(EntitiesSample);
    MAZE_USING_SHARED_PTR(RenderPreparationSystem);
    MAZE_USING_SHARED_PTR(Camera3D);
    MAZE_USING_SHARED_PTR(MeshRenderer);
    MAZE_USING_SHARED_PTR(Transform3D);
    MAZE_USING_SHARED_PTR(RenderPreparationSystemModule3D);
    MAZE_USING_SHARED_PTR(RenderPreparationSystemModule2D);


    //////////////////////////////////////////
    // Class RenderPreparationSystem
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API RenderPreparationSystem
        : public ComponentSystem
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(RenderPreparationSystem, ComponentSystem);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(RenderPreparationSystem);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~RenderPreparationSystem();

        //////////////////////////////////////////
        static RenderPreparationSystemPtr Create(RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        virtual S32 getOrder() const MAZE_OVERRIDE { return -50; }
        

    protected:

        //////////////////////////////////////////
        RenderPreparationSystem();

        //////////////////////////////////////////
        bool init(RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        virtual void processSystemAdded() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processUpdate(F32 _dt) MAZE_OVERRIDE;


    protected:
        RenderSystemPtr m_renderSystem;

        SharedPtr<GenericInclusiveEntitiesSample<Canvas>> m_canvasesSample;
        SharedPtr<GenericInclusiveEntitiesSample<Camera3D>> m_cameras3DSample;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderPreparationSystem_hpp_
//////////////////////////////////////////
