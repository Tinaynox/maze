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
#if (!defined(_MazeSimpleLineRenderer2D_hpp_))
#define _MazeSimpleLineRenderer2D_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeColorGradient.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/MazeColorF128.hpp"
#include "maze-core/ecs/events/MazeEcsCoreEvents.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(SimpleLineRenderer2D);
    MAZE_USING_SHARED_PTR(RenderMask);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_MANAGED_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(VertexArrayObject);


    //////////////////////////////////////////
    // Class SimpleLineRenderer2D
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API SimpleLineRenderer2D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SimpleLineRenderer2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(SimpleLineRenderer2D);

        //////////////////////////////////////////
        friend class Entity;
        friend void SimpleLineRenderer2DEntityRemoved(EntityRemovedEvent const&, Entity*, SimpleLineRenderer2D*);
        friend void SimpleLineRenderer2DOnEcsWorldWillBeDestroyed(EcsWorldWillBeDestroyedEvent const&, Entity*, SimpleLineRenderer2D*);

    public:

        //////////////////////////////////////////
        virtual ~SimpleLineRenderer2D();

        //////////////////////////////////////////
        static SimpleLineRenderer2DPtr Create(RenderSystem* _renderSystem = nullptr);


        //////////////////////////////////////////
        void processEntityRemoved();

        
        //////////////////////////////////////////
        RenderMeshPtr const& getRenderMesh() const { return m_renderMesh; }


        //////////////////////////////////////////
        inline void setColor(ColorF128 const& _color, bool _rebuildMesh) { m_color = _color; if (_rebuildMesh) { rebuildMesh(); } }

        //////////////////////////////////////////
        inline void setColor(ColorF128 const& _color) { setColor(_color, true); }

        //////////////////////////////////////////
        inline ColorF128 const& getColor() const { return m_color; }


        //////////////////////////////////////////
        inline void resizePositions(Size _size) { m_positions.resize(_size); }


        //////////////////////////////////////////
        inline void setPosition(Size _i, Vec2F const& _position) { m_positions[_i] = _position; }


        //////////////////////////////////////////
        inline void setPositions(Vector<Vec3F> const& _positions) { m_positions = _positions; rebuildMesh(); }

        //////////////////////////////////////////
        inline Vector<Vec3F> const& getPositions() const { return m_positions; }




        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        MeshRendererPtr const& getMeshRenderer() const { return m_meshRenderer; }

        //////////////////////////////////////////
        RenderMaskPtr const& getRenderMask() const { return m_renderMask; }

        //////////////////////////////////////////
        void clear();


        //////////////////////////////////////////
        void rebuildMesh();


    protected:

        //////////////////////////////////////////
        SimpleLineRenderer2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(RenderSystem* _renderSystem = nullptr);

        //////////////////////////////////////////
        virtual bool init(
            Component* _component,
            EntityCopyData _copyData) MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processSceneSet() MAZE_OVERRIDE;



    protected:
        RenderSystem* m_renderSystem;

        RenderMaskPtr m_renderMask;
        Transform2DPtr m_transform;
        MeshRendererPtr m_meshRenderer;

        ColorF128 m_color;

        VertexArrayObjectPtr m_vao;
        RenderMeshPtr m_renderMesh;

        Vector<Vec3F> m_positions;

        FastVector<Vec4F> m_colors;
        FastVector<U16> m_indices;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSimpleLineRenderer2D_hpp_
//////////////////////////////////////////
