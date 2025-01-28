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
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/events/MazeEcsGraphicsEvents.hpp"


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
        , public IRenderUnitDrawer
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
        void processEntityRemoved();

        
        //////////////////////////////////////////
        inline RenderMeshAssetRef const& getRenderMeshRef() const { return m_renderMeshRef; }

        //////////////////////////////////////////
        inline void setRenderMeshRef(RenderMeshAssetRef const& _renderMesh) { m_renderMeshRef.setRenderMesh(_renderMesh.getRenderMesh()); }

        //////////////////////////////////////////
        inline RenderMeshPtr const& getRenderMesh() const { return m_renderMeshRef.getRenderMesh(); }

        //////////////////////////////////////////
        inline void setRenderMesh(RenderMeshPtr const& _renderMesh) { m_renderMeshRef.setRenderMesh(_renderMesh); }

        //////////////////////////////////////////
        void setRenderMesh(String const& _renderMeshName);

        //////////////////////////////////////////
        void setMesh(MeshPtr const& _mesh);

        //////////////////////////////////////////
        void clearMesh();


        //////////////////////////////////////////
        inline void setMaterialRef(MaterialAssetRef const& _material) { m_materialRef.setMaterial(_material.getMaterial()); }

        //////////////////////////////////////////
        inline MaterialAssetRef const& getMaterialRef() const { return m_materialRef; }

        //////////////////////////////////////////
        inline void setMaterial(MaterialPtr const& _material) { m_materialRef.setMaterial(_material); }

        //////////////////////////////////////////
        inline MaterialPtr const& getMaterial() const { return m_materialRef.getMaterial(); }

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
        inline TMat const* getModelMatricesData() const { return &m_modelMatricies[0]; }

        //////////////////////////////////////////
        inline Vector<TMat> const& getModelMatrices() const { return m_modelMatricies; }

        //////////////////////////////////////////
        inline void setModelMatrices(Vector<TMat> const& _value) { m_modelMatricies = _value; }

        //////////////////////////////////////////
        inline void setModelMatrix(Size _index, TMat const& _value) { m_modelMatricies[_index] = _value; }

        //////////////////////////////////////////
        inline Vec4F const* getColorsData() const { return &m_colors[0]; }

        //////////////////////////////////////////
        inline Vector<Vec4F> const& getColors() const { return m_colors; }

        //////////////////////////////////////////
        inline void setColors(Vector<Vec4F> const& _value) { m_colors = _value; }

        //////////////////////////////////////////
        inline void setColor(Size _index, Vec4F const& _value) { m_colors[_index] = _value; }


        //////////////////////////////////////////
        inline Vec4F const* getUV0Data() const { return &m_uv0[0]; }

        //////////////////////////////////////////
        inline Vector<Vec4F> const& getUV0() const { return m_uv0; }

        //////////////////////////////////////////
        inline void setUV0(Vector<Vec4F> const& _value) { m_uv0 = _value; }

        //////////////////////////////////////////
        inline void setUV0(Size _index, Vec4F const& _value) { m_uv0[_index] = _value; }


        //////////////////////////////////////////
        inline Vec4F const* getUV1Data() const { return &m_uv1[0]; }

        //////////////////////////////////////////
        inline Vector<Vec4F> const& getUV1() const { return m_uv1; }

        //////////////////////////////////////////
        inline void setUV1(Vector<Vec4F> const& _value) { m_uv1 = _value; }

        //////////////////////////////////////////
        inline void setUV1(Size _index, Vec4F const& _value) { m_uv1[_index] = _value; }

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
            EntityCopyData _copyData = EntityCopyData()) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void drawDefaultPass(
            RenderQueuePtr const& _renderQueue,
            DefaultPassParams const& _params,
            RenderUnit const& _renderUnit) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void drawShadowPass(
            RenderQueuePtr const& _renderQueue,
            ShadowPassParams const& _params,
            RenderUnit const& _renderUnit) MAZE_OVERRIDE;


    protected:
        RenderSystem* m_renderSystem = nullptr;

        RenderMeshAssetRef m_renderMeshRef;
        MaterialAssetRef m_materialRef;
        
        RenderMaskPtr m_renderMask;

        Vector<TMat> m_modelMatricies;
        Vector<Vec4F> m_colors;
        Vector<Vec4F> m_uv0;
        Vector<Vec4F> m_uv1;

        bool m_enabled = true;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMeshRendererInstanced_hpp_
//////////////////////////////////////////
