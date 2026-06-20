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
#if (!defined(_MazeOutlineRenderer_hpp_))
#define _MazeOutlineRenderer_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeColorF128.hpp"
#include "maze-graphics/ecs/events/MazeEcsGraphicsEvents.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(OutlineRenderer);


    //////////////////////////////////////////
    // Class OutlineRenderer
    //
    // Draws a selection/highlight outline around the mesh of a sibling MeshRenderer
    // using the classic inverted-hull stencil technique: the mesh's own silhouette is
    // stamped into the stencil buffer, then a normal-extruded shell of the mesh is drawn
    // with a flat color everywhere the stencil mask doesn't already cover.
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API OutlineRenderer
        : public Component
        , public IRenderUnitDrawer
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(OutlineRenderer, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(OutlineRenderer);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~OutlineRenderer();

        //////////////////////////////////////////
        static OutlineRendererPtr Create(RenderSystem* _renderSystem = nullptr);


        //////////////////////////////////////////
        inline RenderSystem* getRenderSystem() const { return m_renderSystem; }


        //////////////////////////////////////////
        inline void setOutlineMaterialRef(MaterialAssetRef const& _material) { m_outlineMaterialRef.setMaterial(_material.getMaterial()); }

        //////////////////////////////////////////
        inline MaterialAssetRef const& getOutlineMaterialRef() const { return m_outlineMaterialRef; }

        //////////////////////////////////////////
        inline void setOutlineMaterial(MaterialPtr const& _material) { m_outlineMaterialRef.setMaterial(_material); }

        //////////////////////////////////////////
        inline MaterialPtr const& getOutlineMaterial() const { return m_outlineMaterialRef.getMaterial(); }

        //////////////////////////////////////////
        void setOutlineMaterial(String const& _materialName);


        //////////////////////////////////////////
        inline void setEnabled(bool _enabled) { m_enabled = _enabled; }

        //////////////////////////////////////////
        inline bool getEnabled() const { return m_enabled; }


    protected:

        //////////////////////////////////////////
        OutlineRenderer();

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

        MeshRenderer* m_meshRenderer = nullptr;

        MaterialAssetRef m_outlineMaterialRef;

        bool m_enabled = true;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeOutlineRenderer_hpp_
//////////////////////////////////////////
