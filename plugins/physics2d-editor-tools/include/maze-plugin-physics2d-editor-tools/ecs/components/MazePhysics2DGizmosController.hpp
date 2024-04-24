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
#if (!defined(_MazePhysics2DGizmosController_hpp_))
#define _MazePhysics2DGizmosController_hpp_


//////////////////////////////////////////
#include "maze-plugin-physics2d-editor-tools/MazePhysics2DEditorToolsHeader.hpp"
#include "maze-physics2d/physics/MazePhysicsWorld2D.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-editor-tools/ecs/systems/MazeGizmosDrawer.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Physics2DGizmosController);
    MAZE_USING_SHARED_PTR(ECSWorld);
    MAZE_USING_SHARED_PTR(EntitiesSample);
    MAZE_USING_SHARED_PTR(GizmosDrawer);


    //////////////////////////////////////////
    // Class Physics2DGizmosController
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_PHYSICS2D_EDITOR_TOOLS_API Physics2DGizmosController
        : public Component
        , public MultiDelegateCallbackReceiver
        , public b2Draw
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Physics2DGizmosController, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Physics2DGizmosController);

    public:

        //////////////////////////////////////////
        virtual ~Physics2DGizmosController();

        //////////////////////////////////////////
        static Physics2DGizmosControllerPtr Create(PhysicsWorld2D* _physicsWorld, RenderTarget* _renderTarget);

        //////////////////////////////////////////
        void drawGizmos();


    protected:

        //////////////////////////////////////////
        Physics2DGizmosController();

        //////////////////////////////////////////
        bool init(PhysicsWorld2D* _physicsWorld, RenderTarget* _renderTarget);

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyRenderTargetDestroyed(RenderTarget* _renderTarget);

    protected:

        //////////////////////////////////////////
        virtual void DrawPolygon(b2Vec2 const* _vertices, int32 _vertexCount, b2Color const& _color) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void DrawSolidPolygon(b2Vec2 const* _vertices, int32 _vertexCount, b2Color const& _color) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void DrawCircle(b2Vec2 const& _center, F32 _radius, b2Color const& _color) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void DrawSolidCircle(b2Vec2 const& _center, F32 _radius, b2Vec2 const& _axis, b2Color const& _color) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void DrawSegment(b2Vec2 const& _p1, b2Vec2 const& _p2, b2Color const& _color) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void DrawTransform(b2Transform const& _xf) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void DrawPoint(b2Vec2 const& _p, F32 _size, b2Color const& _color) MAZE_OVERRIDE;

    protected:
        PhysicsWorld2D* m_physicsWorld = nullptr;
        RenderTarget* m_renderTarget = nullptr;

        GizmosDrawerPtr m_drawer;
    };


} // namespace Maze
//////////////////////////////////////////
 

#endif // _MazePhysics2DGizmosController_hpp_
//////////////////////////////////////////
