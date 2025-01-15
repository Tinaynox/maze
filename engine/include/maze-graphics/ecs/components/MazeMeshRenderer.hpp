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
#if (!defined(_MazeMeshRenderer_hpp_))
#define _MazeMeshRenderer_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(MeshRenderer);
    MAZE_USING_SHARED_PTR(RenderMask);


    //////////////////////////////////////////
    // Class MeshRenderer
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API MeshRenderer
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(MeshRenderer, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(MeshRenderer);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~MeshRenderer();

        //////////////////////////////////////////
        static MeshRendererPtr Create(RenderSystem* _renderSystem = nullptr);


        //////////////////////////////////////////
        void processEntityRemoved();


        //////////////////////////////////////////
        inline RenderSystem* getRenderSystem() const { return m_renderSystem; }

        
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
        inline Vector<MaterialAssetRef> const& getMaterialRefs() const { return m_materialRefs; }

        //////////////////////////////////////////
        inline MaterialPtr const& getMaterial() const
        {
            static MaterialPtr const nullPointer;

            if (!m_materialRefs.empty())
                return m_materialRefs.front().getMaterial();
            
            return nullPointer;
        }

        //////////////////////////////////////////
        inline MaterialPtr const& getMaterial(Size _i) const { return m_materialRefs[_i].getMaterial(); }

        //////////////////////////////////////////
        inline void setMaterialRefs(Vector<MaterialAssetRef> const& _materialRefs) { m_materialRefs = _materialRefs; }

        //////////////////////////////////////////
        void setMaterial(MaterialPtr const& _material);

        //////////////////////////////////////////
        void setMaterial(String const& _materialName);

        //////////////////////////////////////////
        void setMaterials(Vector<String> const& _materialNames);

        //////////////////////////////////////////
        void addMaterial(MaterialPtr const& _material);

        //////////////////////////////////////////
        void addMaterial(String const& _materialName);


        //////////////////////////////////////////
        inline RenderMaskPtr const& getRenderMask() const { return m_renderMask; }


        //////////////////////////////////////////
        inline void setEnabled(bool _enabled) { m_enabled = _enabled; }

        //////////////////////////////////////////
        inline bool getEnabled() const { return m_enabled; }

    protected:

        //////////////////////////////////////////
        MeshRenderer();

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


    protected:
        RenderSystem* m_renderSystem;

        RenderMeshAssetRef m_renderMeshRef;
        Vector<MaterialAssetRef> m_materialRefs;
        
        RenderMaskPtr m_renderMask;

        bool m_enabled;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMeshRenderer_hpp_
//////////////////////////////////////////
