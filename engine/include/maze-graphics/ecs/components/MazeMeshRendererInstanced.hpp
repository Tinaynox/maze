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
#if (!defined(_MazeMeshRendererInstanced_hpp_))
#define _MazeMeshRendererInstanced_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(MeshRendererInstanced);
    MAZE_USING_SHARED_PTR(RenderMask);


    //////////////////////////////////////////
    // Class MeshRendererInstanced
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API MeshRendererInstanced
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(MeshRendererInstanced, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(MeshRendererInstanced);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~MeshRendererInstanced();

        //////////////////////////////////////////
        static MeshRendererInstancedPtr Create(RenderSystem* _renderSystem = nullptr);

            
        //////////////////////////////////////////
        inline RenderMeshPtr const& getRenderMesh() const { return m_renderMesh; }

        //////////////////////////////////////////
        inline void setRenderMesh(RenderMeshPtr const& _renderMesh) { m_renderMesh = _renderMesh; }

        //////////////////////////////////////////
        void setRenderMesh(String const& _renderMeshName);

        //////////////////////////////////////////
        void setMesh(MeshPtr const& _mesh);

        //////////////////////////////////////////
        void clearMesh();


        //////////////////////////////////////////
        inline void setMaterial(MaterialPtr const& _material) { m_material = _material; }

        //////////////////////////////////////////
        inline MaterialPtr const& getMaterial() const { return m_material; }

        //////////////////////////////////////////
        void setMaterial(String const& _materialName);


        //////////////////////////////////////////
        inline RenderMaskPtr const& getRenderMask() const { return m_renderMask; }


        //////////////////////////////////////////
        inline void setEnabled(bool _enabled) { m_enabled = _enabled; }

        //////////////////////////////////////////
        inline bool getEnabled() const { return m_enabled; }


        //////////////////////////////////////////
        void resize(S32 _count);

        //////////////////////////////////////////
        inline Mat4DF const* getModelMatricesData() const { return &m_modelMatricies[0]; }

        //////////////////////////////////////////
        inline Vector<Mat4DF> const& getModelMatrices() const { return m_modelMatricies; }

        //////////////////////////////////////////
        inline void setModelMatrices(Vector<Mat4DF> const& _value) { m_modelMatricies = _value; }

        //////////////////////////////////////////
        inline void setModelMatrix(Size _index, Mat4DF const& _value) { m_modelMatricies[_index] = _value; }

        //////////////////////////////////////////
        inline Vec4DF const* getColorsData() const { return &m_colors[0]; }

        //////////////////////////////////////////
        inline Vector<Vec4DF> const& getColors() const { return m_colors; }

        //////////////////////////////////////////
        inline void setColors(Vector<Vec4DF> const& _value) { m_colors = _value; }

        //////////////////////////////////////////
        inline void setColor(Size _index, Vec4DF const& _value) { m_colors[_index] = _value; }


        //////////////////////////////////////////
        inline Vec4DF const* getUV0Data() const { return &m_uv0[0]; }

        //////////////////////////////////////////
        inline Vector<Vec4DF> const& getUV0() const { return m_uv0; }

        //////////////////////////////////////////
        inline void setUV0(Vector<Vec4DF> const& _value) { m_uv0 = _value; }

        //////////////////////////////////////////
        inline void setUV0(Size _index, Vec4DF const& _value) { m_uv0[_index] = _value; }


        //////////////////////////////////////////
        inline Vec4DF const* getUV1Data() const { return &m_uv1[0]; }

        //////////////////////////////////////////
        inline Vector<Vec4DF> const& getUV1() const { return m_uv1; }

        //////////////////////////////////////////
        inline void setUV1(Vector<Vec4DF> const& _value) { m_uv1 = _value; }

        //////////////////////////////////////////
        inline void setUV1(Size _index, Vec4DF const& _value) { m_uv1[_index] = _value; }

    protected:

        //////////////////////////////////////////
        MeshRendererInstanced();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(RenderSystem* _renderSystem = nullptr);


        //////////////////////////////////////////
        virtual bool init(
            Component* _component,
            ECSWorld* _world,
            EntityCopyData _copyData = EntityCopyData()) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntityRemoved() MAZE_OVERRIDE;

    protected:
        RenderSystem* m_renderSystem;

        RenderMeshPtr m_renderMesh;
        MaterialPtr m_material;
        
        RenderMaskPtr m_renderMask;

        Vector<Mat4DF> m_modelMatricies;
        Vector<Vec4DF> m_colors;
        Vector<Vec4DF> m_uv0;
        Vector<Vec4DF> m_uv1;

        bool m_enabled;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMeshRendererInstanced_hpp_
//////////////////////////////////////////
