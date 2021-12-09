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
        inline Vector<MaterialPtr> const& getMaterials() const { return m_materials; }

        //////////////////////////////////////////
        inline MaterialPtr const& getMaterial() const
        {
            static MaterialPtr nullPointer;

            if (!m_materials.empty())
                return m_materials.front();
            
            return nullPointer;
        }

        //////////////////////////////////////////
        inline void setMaterials(Vector<MaterialPtr> const& _materials) { m_materials = _materials; }

        //////////////////////////////////////////
        void setMaterial(MaterialPtr const& _material);

        //////////////////////////////////////////
        void setMaterial(String const& _materialName);


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
            ECSWorld* _world,
            EntityCopyData _copyData = EntityCopyData()) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntityRemoved() MAZE_OVERRIDE;

    protected:
        RenderSystem* m_renderSystem;

        RenderMeshPtr m_renderMesh;
        Vector<MaterialPtr> m_materials;
        
        RenderMaskPtr m_renderMask;

        bool m_enabled;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMeshRenderer_hpp_
//////////////////////////////////////////
