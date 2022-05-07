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
#if (!defined(_MazeGizmosSystem_hpp_))
#define _MazeGizmosSystem_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(GizmosSystem);
    MAZE_USING_SHARED_PTR(ECSWorld);
    MAZE_USING_SHARED_PTR(EntitiesSample);
    MAZE_USING_SHARED_PTR(ComponentGizmos);
    MAZE_USING_SHARED_PTR(GizmosDrawer);


    //////////////////////////////////////////
    // Class GizmosSystem
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API GizmosSystem
        : public ComponentSystem
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(GizmosSystem, ComponentSystem);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(GizmosSystem);

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
        virtual ~GizmosSystem();

        //////////////////////////////////////////
        static GizmosSystemPtr Create(RenderTarget* _renderTarget);

        //////////////////////////////////////////
        void drawGizmos(F32 _dt);


        //////////////////////////////////////////
        inline GizmosDrawerPtr const& getDrawer() const { return m_drawer; }


        //////////////////////////////////////////
        virtual S32 getOrder() const MAZE_OVERRIDE { return 40000; }

    public:

        //////////////////////////////////////////
        MultiDelegate<ComponentGizmosPtr const&, Entity*, ComponentPtr const&, GizmosDrawer*> eventDrawGizmosEvent;

    protected:

        //////////////////////////////////////////
        GizmosSystem();

        //////////////////////////////////////////
        bool init(RenderTarget* _renderTarget);

        //////////////////////////////////////////
        virtual void processSystemAdded() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processUpdate(F32 _dt) MAZE_OVERRIDE;

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
    };


} // namespace Maze
//////////////////////////////////////////
 

#endif // _MazeGizmosSystem_hpp_
//////////////////////////////////////////
