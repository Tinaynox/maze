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
#if (!defined(_MazeRenderController_hpp_))
#define _MazeRenderController_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/utils/MazeDelegate.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderSystem);
    MAZE_USING_SHARED_PTR(EntitiesSample);
    MAZE_USING_SHARED_PTR(RenderController);
    MAZE_USING_SHARED_PTR(Camera3D);
    MAZE_USING_SHARED_PTR(MeshRenderer);
    MAZE_USING_SHARED_PTR(Transform3D);
    MAZE_USING_SHARED_PTR(RenderControllerModule3D);
    MAZE_USING_SHARED_PTR(RenderControllerModule2D);


    //////////////////////////////////////////
    // Class RenderController
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API RenderController
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(RenderController, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(RenderController);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~RenderController();

        //////////////////////////////////////////
        static RenderControllerPtr Create(RenderSystemPtr const& _renderSystem);


        //////////////////////////////////////////
        RenderControllerModule3DPtr const& getModule3D() const { return m_module3D; }

        //////////////////////////////////////////
        RenderControllerModule2DPtr const getModule2D() const { return m_module2D; }


        //////////////////////////////////////////
        void updateRenderTargets();

        //////////////////////////////////////////
        void render();

        //////////////////////////////////////////
        void update(F32 _dt);


        //////////////////////////////////////////
        inline SharedPtr<GenericInclusiveEntitiesSample<Canvas>> const& getCanvasesSample() const { return m_canvasesSample; }

        //////////////////////////////////////////
        inline SharedPtr<GenericInclusiveEntitiesSample<Camera3D>> const& getCameras3DSample() const { return m_cameras3DSample; }

    protected:

        //////////////////////////////////////////
        RenderController();

        //////////////////////////////////////////
        bool init(RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void processCanvasEntityAdded(Entity* _entity, Canvas* _canvas);

        //////////////////////////////////////////
        void processCanvasEntityRemoved(Entity* _entity, Canvas* _canvas);

        //////////////////////////////////////////
        void processCanvasRenderTargetChanged(Canvas* _canvas, RenderTargetPtr const& _renderTarget);

        //////////////////////////////////////////
        void processCameraEntityAdded(Entity* _entity, Camera3D* _camera);

        //////////////////////////////////////////
        void processCameraEntityRemoved(Entity* _entity, Camera3D* _camera);

        //////////////////////////////////////////
        void processCameraRenderTargetChanged(Camera3D* _camera3D, RenderTargetPtr const& _renderTarget);


        //////////////////////////////////////////
        void addRenderTarget(RenderTarget* _renderTarget);

        //////////////////////////////////////////
        void removeRenderTarget(RenderTarget* _renderTarget);

        //////////////////////////////////////////
        void clearRenderTargets();

        //////////////////////////////////////////
        void sortRenderTargets();


        //////////////////////////////////////////
        void notifyRenderTargetOrderChanged(RenderTarget* _renderTarget, S32 _order);

        //////////////////////////////////////////
        void notifyRenderTargetDestroyed(RenderTarget* _renderTarget);

    protected:
        RenderSystemPtr m_renderSystem;

        // #TODO: Refactor as separate systems
        RenderControllerModule3DPtr m_module3D;
        RenderControllerModule2DPtr m_module2D;

        SharedPtr<GenericInclusiveEntitiesSample<Canvas>> m_canvasesSample;
        Set<CanvasPtr> m_canvases;
        SharedPtr<GenericInclusiveEntitiesSample<Camera3D>> m_cameras3DSample;
        Set<Camera3DPtr> m_cameras3D;

        bool m_renderTargetsDirty;
        Vector<RenderTarget*> m_renderTargets;

        Delegate<void, RenderTarget*, S32> m_delegateRenderTargetOrderChanged;
        Delegate<void, RenderTarget*> m_delegateRenderTargetDestroyed;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderController_hpp_
//////////////////////////////////////////
