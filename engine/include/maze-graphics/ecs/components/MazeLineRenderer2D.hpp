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
#if (!defined(_MazeLineRenderer2D_hpp_))
#define _MazeLineRenderer2D_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeColorGradient.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/MazeColorF128.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(LineRenderer2D);
    MAZE_USING_SHARED_PTR(RenderMask);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(VertexArrayObject);


    //////////////////////////////////////////
    // Class LineRenderer2D
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API LineRenderer2D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(LineRenderer2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(LineRenderer2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~LineRenderer2D();

        //////////////////////////////////////////
        static LineRenderer2DPtr Create(RenderSystem* _renderSystem = nullptr);

        
        //////////////////////////////////////////
        RenderMeshPtr const& getRenderMesh() const { return m_renderMesh; }

        //////////////////////////////////////////
        inline void setWidth(F32 _width) { m_width = _width; rebuildMesh(); }

        //////////////////////////////////////////
        inline F32 getWidth() const { return m_width; }


        //////////////////////////////////////////
        inline void setColor(ColorF128 const& _color, bool _rebuildMesh = true)
        {
            setColor(ColorGradient(0.0f, _color.toVec3F32()), _rebuildMesh);
        }

        //////////////////////////////////////////
        inline void setColor(ColorGradient const& _color, bool _rebuildMesh) { m_color = _color; if (_rebuildMesh) { rebuildMesh(); } }

        //////////////////////////////////////////
        inline void setColor(ColorGradient const& _color) { setColor(_color, true); }

        //////////////////////////////////////////
        inline ColorGradient const& getColor() const { return m_color; }


        //////////////////////////////////////////
        inline void resizePositions(Size _size) { m_positions.resize(_size); }


        //////////////////////////////////////////
        inline void setPosition(Size _i, Vec2F const& _position) { m_positions[_i] = _position; }


        //////////////////////////////////////////
        inline void setPositions(Vector<Vec2F> const& _positions) { m_positions = _positions; rebuildMesh(); }

        //////////////////////////////////////////
        inline Vector<Vec2F> const& getPositions() const { return m_positions; }




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
        LineRenderer2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(RenderSystem* _renderSystem = nullptr);

        //////////////////////////////////////////
        virtual bool init(
            Component* _component,
            ECSWorld* _world,
            EntityCopyData _copyData) MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processSceneSet() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntityRemoved() MAZE_OVERRIDE;

        //////////////////////////////////////////
        F32 getTrailWidth(F32 _progress);


    protected:
        RenderSystem* m_renderSystem;

        RenderMaskPtr m_renderMask;
        Transform2DPtr m_transform;
        MeshRendererPtr m_meshRenderer;

        F32 m_width;
        ColorGradient m_color;

        VertexArrayObjectPtr m_vao;
        RenderMeshPtr m_renderMesh;

        Vector<Vec2F> m_positions;

        FastVector<Vec3F> m_vertices;
        FastVector<Vec2F> m_uvs;
        FastVector<Vec4F> m_colors;
        FastVector<U16> m_indices;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeLineRenderer2D_hpp_
//////////////////////////////////////////
