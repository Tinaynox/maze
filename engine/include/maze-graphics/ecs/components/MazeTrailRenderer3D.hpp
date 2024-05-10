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
#if (!defined(_MazeTrailRenderer3D_hpp_))
#define _MazeTrailRenderer3D_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/MazeColorF128.hpp"
#include "maze-graphics/MazeColorGradient.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(TrailRenderer3D);
    MAZE_USING_SHARED_PTR(RenderMask);
    MAZE_USING_SHARED_PTR(Transform3D);
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(VertexArrayObject);


    //////////////////////////////////////////
    // Class TrailRenderer3D
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API TrailRenderer3D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(TrailRenderer3D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(TrailRenderer3D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        struct TrailEdge
        {
            //////////////////////////////////////////
            TrailEdge(
                F32 _time,
                Vec3F const& _position)
                : time(_time)
                , position(_position)

            { }

            F32 time;
            Vec3F position;
            Vec3F direction = Vec3F::c_zero;
            F32 halfWidth = 0.0f;
            F32 distanceToNextEdge = 0.0f;
        };

    public:

        //////////////////////////////////////////
        virtual ~TrailRenderer3D();

        //////////////////////////////////////////
        static TrailRenderer3DPtr Create(RenderSystem* _renderSystem = nullptr);


        //////////////////////////////////////////
        inline RenderSystem* getRenderSystem() const { return m_renderSystem; }

        
        //////////////////////////////////////////
        RenderMeshPtr const& getRenderMesh() const { return m_renderMesh; }


        //////////////////////////////////////////
        inline Vector<MaterialAssetRef> const& getMaterialRefs() const { return m_materialRefs; }

        //////////////////////////////////////////
        inline void setMaterialRefs(Vector<MaterialAssetRef> const& _materials) { m_materialRefs = _materials; }

        //////////////////////////////////////////
        void setMaterial(MaterialPtr const& _material);

        //////////////////////////////////////////
        void setMaterial(String const& _materialName);


        //////////////////////////////////////////
        inline void setWidth(F32 _width) { m_width = _width; }

        //////////////////////////////////////////
        inline F32 getWidth() const { return m_width; }


        //////////////////////////////////////////
        inline void setColor(Vec4F const& _color) { setColor(ColorGradient(_color)); }

        //////////////////////////////////////////
        inline void setColor(ColorU32 const& _color) { setColor(ColorGradient(_color.toVec4F32())); }

        //////////////////////////////////////////
        inline void setColor(ColorF128 const& _color) { setColor(ColorGradient(_color.toVec4F32())); }

        //////////////////////////////////////////
        inline void setColor(ColorGradient const& _color) { m_color = _color; }

        //////////////////////////////////////////
        inline ColorGradient const& getColor() const { return m_color; }


        //////////////////////////////////////////
        inline void setMinVertexDistance(F32 _minVertexDistance)
        {
            m_minVertexDistance = _minVertexDistance;
            m_minVertexDistanceSqr = _minVertexDistance * _minVertexDistance;
        }

        //////////////////////////////////////////
        inline F32 getMinVertexDistance() const { return m_minVertexDistance; }


        //////////////////////////////////////////
        inline void setTime(F32 _time) { m_time = _time; }

        //////////////////////////////////////////
        inline F32 getTime() const { return m_time; }


        //////////////////////////////////////////
        inline F32 getTimer() const { return m_timer; }


        //////////////////////////////////////////
        Transform3DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        RenderMaskPtr const& getRenderMask() const { return m_renderMask; }

        //////////////////////////////////////////
        void update(F32 _dt);

        //////////////////////////////////////////
        void clear();


        //////////////////////////////////////////
        inline Vec3F const& getWorldPosition()
        {
            return m_edges.back().position;
        }

        //////////////////////////////////////////
        inline Size getEdgesCount() const { return m_edges.size(); }

    protected:

        //////////////////////////////////////////
        TrailRenderer3D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(RenderSystem* _renderSystem = nullptr);

        //////////////////////////////////////////
        virtual bool init(
            Component* _component,
            EcsWorld* _world,
            EntityCopyData _copyData) MAZE_OVERRIDE;


        //////////////////////////////////////////
        void addEdge(
            F32 _time,
            Vec3F const& _position);

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processSceneSet() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntityRemoved() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void rebuildMesh();

        //////////////////////////////////////////
        F32 getTrailWidth(F32 _progress);

    protected:
        RenderSystem* m_renderSystem;

        RenderMaskPtr m_renderMask;
        Transform3DPtr m_transform;

        Vector<MaterialAssetRef> m_materialRefs;

        F32 m_time;
        F32 m_minVertexDistance;
        F32 m_minVertexDistanceSqr;
        F32 m_width;
        ColorGradient m_color;

        VertexArrayObjectPtr m_vao;
        RenderMeshPtr m_renderMesh;

        F32 m_timer = 0.0f;

        Deque<TrailEdge> m_edges;
        FastVector<Vec3F> m_vertices;
        FastVector<Vec2F> m_uvs;
        FastVector<Vec4F> m_colors;
        FastVector<U16> m_indices;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTrailRenderer3D_hpp_
//////////////////////////////////////////
