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
#if (!defined(_MazeLineRenderer3D_hpp_))
#define _MazeLineRenderer3D_hpp_


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
    MAZE_USING_SHARED_PTR(LineRenderer3D);
    MAZE_USING_SHARED_PTR(RenderMask);
    MAZE_USING_SHARED_PTR(Transform3D);
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(VertexArrayObject);


    //////////////////////////////////////////
    // Class LineRenderer3D
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API LineRenderer3D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(LineRenderer3D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(LineRenderer3D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~LineRenderer3D();

        //////////////////////////////////////////
        static LineRenderer3DPtr Create(RenderSystem* _renderSystem = nullptr);

        
        //////////////////////////////////////////
        RenderMeshPtr const& getRenderMesh() const { return m_renderMesh; }


        //////////////////////////////////////////
        inline Vector<MaterialPtr> const& getMaterials() const { return m_materials; }

        //////////////////////////////////////////
        inline void setMaterials(Vector<MaterialPtr> const& _materials) { m_materials = _materials; }

        //////////////////////////////////////////
        void setMaterial(MaterialPtr const& _material);

        //////////////////////////////////////////
        void setMaterial(String const& _materialName);


        //////////////////////////////////////////
        inline void setWidth(F32 _width) { m_width = _width; rebuildMesh(); }

        //////////////////////////////////////////
        inline F32 getWidth() const { return m_width; }


        //////////////////////////////////////////
        inline void setColor(ColorF128 const& _color)
        {
            setColor(ColorGradient(0.0f, _color.toVec3DF()));
        }

        //////////////////////////////////////////
        inline void setColor(ColorGradient const& _color) { m_color = _color; rebuildMesh(); }

        //////////////////////////////////////////
        inline ColorGradient const& getColor() const { return m_color; }


        //////////////////////////////////////////
        inline void setPositions(Vector<Vec3DF> const& _positions) { m_positions = _positions; rebuildMesh(); }

        //////////////////////////////////////////
        inline Vector<Vec3DF> const& getPositions() const { return m_positions; }


        //////////////////////////////////////////
        Transform3DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        RenderMaskPtr const& getRenderMask() const { return m_renderMask; }

        //////////////////////////////////////////
        void clear();


    protected:

        //////////////////////////////////////////
        LineRenderer3D();

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
        void rebuildMesh();

        //////////////////////////////////////////
        F32 getTrailWidth(F32 _progress);

    protected:
        RenderSystem* m_renderSystem;

        RenderMaskPtr m_renderMask;
        Transform3DPtr m_transform;

        Vector<MaterialPtr> m_materials;

        F32 m_width;
        ColorGradient m_color;

        VertexArrayObjectPtr m_vao;
        RenderMeshPtr m_renderMesh;

        Vector<Vec3DF> m_positions;

        FastVector<Vec3DF> m_vertices;
        FastVector<Vec2DF> m_uvs;
        FastVector<Vec4DF> m_colors;
        FastVector<U32> m_indices;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeLineRenderer3D_hpp_
//////////////////////////////////////////
