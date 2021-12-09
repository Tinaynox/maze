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
#if (!defined(_MazeRenderControlSystemModule2D_hpp_))
#define _MazeRenderControlSystemModule2D_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeCanvasRenderer.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeMeshRendererInstanced.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EntitiesSample);
    MAZE_USING_SHARED_PTR(RenderControlSystemModule2D);
    MAZE_USING_SHARED_PTR(Camera3D);
    MAZE_USING_SHARED_PTR(MeshRenderer);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(ScissorMask2D);
            

    //////////////////////////////////////////
    // Class RenderControlSystemModule2D
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API RenderControlSystemModule2D
        : public MultiDelegateCallbackReceiver
    {
    protected:

        //////////////////////////////////////////
        enum CanvasRenderCommandType
        {
            DrawMeshRenderer,
            DrawMeshRendererInstanced,
            PushScissorMask,
            PopScissorMask
        };

        //////////////////////////////////////////
        struct CanvasRenderCommand
        {
            //////////////////////////////////////////
            CanvasRenderCommand(Transform2D* _transform, MeshRenderer const* _meshRenderer)
                : type(CanvasRenderCommandType::DrawMeshRenderer)
                , transform(_transform)
                , meshRenderer(_meshRenderer)
            {}

            //////////////////////////////////////////
            CanvasRenderCommand(Transform2D* _transform, MeshRendererInstanced const* _meshRenderer)
                : type(CanvasRenderCommandType::DrawMeshRendererInstanced)
                , transform(_transform)
                , meshRendererInstanced(_meshRenderer)
            {}

            //////////////////////////////////////////
            CanvasRenderCommand(Transform2D* _transform, ScissorMask2D* _scissorMask, bool _push)
                : type(_push ? CanvasRenderCommandType::PushScissorMask : CanvasRenderCommandType::PopScissorMask)
                , transform(_transform)
                , scissorMask(_scissorMask)
            {}

            CanvasRenderCommandType type;

            Transform2D* transform;
            MeshRenderer const* meshRenderer;
            MeshRendererInstanced const* meshRendererInstanced;
            RenderMesh const* renderMesh;
            ScissorMask2D* scissorMask;
        };

        //////////////////////////////////////////
        struct CanvasRenderData
        {
            Canvas* canvas = nullptr;
            Canvas* rootCanvas = nullptr;
            Vector<CanvasRenderCommand> commands;
        };

    public:

        //////////////////////////////////////////
        virtual ~RenderControlSystemModule2D();

        //////////////////////////////////////////
        static RenderControlSystemModule2DPtr Create(
            ECSWorldPtr const& _world,
            RenderSystemPtr const& _renderSystem);


        //////////////////////////////////////////
        void draw(RenderTarget* _renderTarget);

        //////////////////////////////////////////
        void processUpdate(F32 _dt);

        //////////////////////////////////////////
        void processPostUpdate(F32 _dt);


    protected:

        //////////////////////////////////////////
        RenderControlSystemModule2D();

        //////////////////////////////////////////
        bool init(
            ECSWorldPtr const& _world,
            RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        void processCanvasEntityAdded(Entity* _entity, Canvas* _canvas);

        //////////////////////////////////////////
        void processCanvasEntityRemoved(Entity* _entity, Canvas* _canvas);

        //////////////////////////////////////////
        void processMeshRendererEntityAdded(Entity* _entity, MeshRenderer* _meshRenderer, Transform2D* _transform2D);

        //////////////////////////////////////////
        void processMeshRendererEntityRemoved(Entity* _entity, MeshRenderer* _meshRenderer, Transform2D* _transform2D);

        //////////////////////////////////////////
        void processMeshRendererEntityAdded(Entity* _entity, MeshRendererInstanced* _meshRenderer, Transform2D* _transform2D);

        //////////////////////////////////////////
        void processMeshRendererEntityRemoved(Entity* _entity, MeshRendererInstanced* _meshRenderer, Transform2D* _transform2D);

        //////////////////////////////////////////
        void updateSortedCanvasesList();

        //////////////////////////////////////////
        void updateSortedMeshRenderersList();

    protected:
        ECSWorld* m_world;
        RenderSystemPtr m_renderSystem;

        SharedPtr<GenericInclusiveEntitiesSample<Transform2D>> m_transform2Ds;
        SharedPtr<GenericInclusiveEntitiesSample<MeshRenderer, Transform2D>> m_meshRenderersSample;
        SharedPtr<GenericInclusiveEntitiesSample<MeshRendererInstanced, Transform2D>> m_meshRenderersInstancedSample;
        SharedPtr<GenericInclusiveEntitiesSample<Canvas>> m_canvasesSample;
        SharedPtr<GenericInclusiveEntitiesSample<CanvasScaler>> m_canvasScalersSample;
        SharedPtr<GenericInclusiveEntitiesSample<CanvasGroup>> m_canvasGroupsSample;
        SharedPtr<GenericInclusiveEntitiesSample<CanvasRenderer>> m_canvasRenderersSample;
        SharedPtr<GenericInclusiveEntitiesSample<SpriteRenderer2D>> m_spriteRenderer2DsSample;
        SharedPtr<GenericInclusiveEntitiesSample<SystemTextRenderer2D>> m_systemTextRenderer2DsSample;
        SharedPtr<GenericInclusiveEntitiesSample<SizePolicy2D>> m_sizePolicy2D;

        Vector<Canvas*> m_sortedCanvases;

        bool m_sortedMeshRenderersDirty;
        Vector<CanvasRenderData> m_sortedCanvasRenderData;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderControlSystemModule2D_hpp_
//////////////////////////////////////////
