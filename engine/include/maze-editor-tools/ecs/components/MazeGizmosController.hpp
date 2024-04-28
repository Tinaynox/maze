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
#if (!defined(_MazeGizmosController_hpp_))
#define _MazeGizmosController_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(GizmosController);
    MAZE_USING_SHARED_PTR(ECSWorld);
    MAZE_USING_SHARED_PTR(EntitiesSample);
    MAZE_USING_SHARED_PTR(ComponentGizmos);
    MAZE_USING_SHARED_PTR(GizmosDrawer);


    //////////////////////////////////////////
    // Class GizmosController
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API GizmosController
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(GizmosController, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(GizmosController);

    public:

        //////////////////////////////////////////
        struct GizmosSample
        {
            ClassUID componentClassUID = 0;
            EntitiesSamplePtr sample;
            ComponentGizmosPtr gizmos;
        };

    public:

        //////////////////////////////////////////
        virtual ~GizmosController();

        //////////////////////////////////////////
        static GizmosControllerPtr Create(RenderTarget* _renderTarget);

        //////////////////////////////////////////
        void drawGizmos(F32 _dt);


        //////////////////////////////////////////
        inline GizmosDrawerPtr const& getDrawer() const { return m_drawer; }


        //////////////////////////////////////////
        inline SharedPtr<GenericInclusiveEntitiesSample<Camera3D>> const& getCameras3DSample() const { return m_cameras3DSample; }

    public:

        //////////////////////////////////////////
        MultiDelegate<ComponentGizmosPtr const&, Entity*, ComponentPtr const&, GizmosDrawer*> eventDrawGizmosEvent;

    protected:

        //////////////////////////////////////////
        GizmosController();

        //////////////////////////////////////////
        bool init(RenderTarget* _renderTarget);

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        // virtual void processUpdate(UpdateEvent const& _event) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyGizmosPerComponentClass();

        //////////////////////////////////////////
        void updateGizmosSamples();

        //////////////////////////////////////////
        void notifyRenderTargetDestroyed(RenderTarget* _renderTarget);

    protected:
        RenderTarget* m_renderTarget;

        Vector<GizmosSample> m_samples;

        GizmosDrawerPtr m_drawer;

        SharedPtr<GenericInclusiveEntitiesSample<Canvas>> m_canvasesSample;
        SharedPtr<GenericInclusiveEntitiesSample<Camera3D>> m_cameras3DSample;
    };


} // namespace Maze
//////////////////////////////////////////
 

#endif // _MazeGizmosController_hpp_
//////////////////////////////////////////
