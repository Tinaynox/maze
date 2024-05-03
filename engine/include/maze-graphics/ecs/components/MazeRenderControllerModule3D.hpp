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
#if (!defined(_MazeRenderControllerModule3D_hpp_))
#define _MazeRenderControllerModule3D_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeVec4.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeMeshRendererInstanced.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer3D.hpp"
#include "maze-graphics/ecs/events/MazeECSGraphicsEvents.hpp"
#include "maze-graphics/config/MazeGraphicsConfig.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EntitiesSample);
    MAZE_USING_SHARED_PTR(RenderControllerModule3D);
    MAZE_USING_SHARED_PTR(Camera3D);
    MAZE_USING_SHARED_PTR(Light3D);
    MAZE_USING_SHARED_PTR(MeshRenderer);
    MAZE_USING_SHARED_PTR(Transform3D);
    MAZE_USING_SHARED_PTR(TrailRenderer3D);
    MAZE_USING_SHARED_PTR(TrailRenderer3DHider);
    MAZE_USING_SHARED_PTR(LineRenderer3D);
    MAZE_USING_SHARED_PTR(LineRenderer2D);
    MAZE_USING_SHARED_PTR(RenderPass);
    MAZE_USING_SHARED_PTR(VertexArrayObject);
    MAZE_USING_SHARED_PTR(LightingSettings);


    //////////////////////////////////////////
    // Class RenderControllerModule3D
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API RenderControllerModule3D
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~RenderControllerModule3D();

        //////////////////////////////////////////
        static RenderControllerModule3DPtr Create(
            ECSWorldPtr const& _world,
            RenderSystemPtr const& _renderSystem);


        //////////////////////////////////////////
        void drawDefaultPass(
            RenderTarget* _renderTarget,
            DefaultPassParams const& _params,
            std::function<void(RenderQueuePtr const&)> _beginRenderQueueCallback = nullptr,
            std::function<void(RenderQueuePtr const&)> _beginDrawCallback = nullptr,
            std::function<void(RenderQueuePtr const&)> _endDrawCallback = nullptr,
            std::function<void(RenderQueuePtr const&)> _endRenderQueueCallback = nullptr);

        //////////////////////////////////////////
        void draw(RenderTarget* _renderTarget);

        //////////////////////////////////////////
        void processUpdate(F32 _dt);

        //////////////////////////////////////////
        void processPostUpdate(F32 _dt);


    protected:

        //////////////////////////////////////////
        RenderControllerModule3D();

        //////////////////////////////////////////
        bool init(
            ECSWorldPtr const& _world,
            RenderSystemPtr const& _renderSystem);


    protected:
        ECSWorld* m_world = nullptr;
        RenderSystemPtr m_renderSystem;

        SharedPtr<GenericInclusiveEntitiesSample<Camera3D>> m_cameras3DSample;
        SharedPtr<GenericInclusiveEntitiesSample<Light3D>> m_lights3DSample;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderControllerModule3D_hpp_
//////////////////////////////////////////
