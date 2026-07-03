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
#if (!defined(_MazeAbstractTextRenderer2D_hpp_))
#define _MazeAbstractTextRenderer2D_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeAlignment2D.hpp"
#include "maze-graphics/MazeSystemFont.hpp"
#include "maze-graphics/ecs/components/MazeAbstractTextRenderer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(Material);
    MAZE_USING_MANAGED_SHARED_PTR(Mesh);
    MAZE_USING_MANAGED_SHARED_PTR(SubMesh);
    MAZE_USING_SHARED_PTR(AbstractTextRenderer2D);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(CanvasRenderer);
    MAZE_USING_SHARED_PTR(MeshRendererInstanced);


    //////////////////////////////////////////
    // Class AbstractTextRenderer2D
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API AbstractTextRenderer2D
        : public AbstractTextRenderer
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AbstractTextRenderer2D, AbstractTextRenderer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(AbstractTextRenderer2D);

        //////////////////////////////////////////
        friend class Entity;

        //////////////////////////////////////////
        enum class Flags : U8
        {
            None = 0,

            MeshDataDirty = MAZE_BIT(0),        // Full text relayout (updateMeshDataNow)
            ColorDirty = MAZE_BIT(1),           // Push m_localColors * mesh color to the mesh renderer
            ModelMatricesDirty = MAZE_BIT(2),   // Push world-transformed m_localMatrices to the mesh renderer
            MaterialDirty = MAZE_BIT(3),        // Refetch material
            GlyphColorsDirty = MAZE_BIT(4),     // Rewrite m_localColors without relayout (renderers with per-glyph colors)
        };

    public:

        //////////////////////////////////////////
        virtual ~AbstractTextRenderer2D();


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        CanvasRendererPtr const& getCanvasRenderer() const { return m_canvasRenderer; }

        //////////////////////////////////////////
        MeshRendererInstancedPtr const& getMeshRenderer() const { return m_meshRenderer; }


        //////////////////////////////////////////
        inline Vec2F const& getBoundingSize() const { return m_boundingSize; }


        //////////////////////////////////////////
        inline U8 getFlags() const { return m_flags; }

        //////////////////////////////////////////
        inline void setFlags(U8 _flags) { m_flags = _flags; }

        //////////////////////////////////////////
        inline void setFlag(Flags _flag, bool _value)
        {
            if (_value)
                setFlags(m_flags | static_cast<U8>(_flag));
            else
                setFlags(m_flags & ~static_cast<U8>(_flag));
        }

        //////////////////////////////////////////
        inline bool getFlag(Flags _flag) const { return (m_flags & static_cast<U8>(_flag)) != 0; }

        //////////////////////////////////////////
        inline void enableFlag(Flags _flag) { setFlag(_flag, true); }

        //////////////////////////////////////////
        inline void disableFlag(Flags _flag) { setFlag(_flag, false); }


        //////////////////////////////////////////
        virtual bool getPixelPerfect() const { return m_pixelPerfect; }

        //////////////////////////////////////////
        virtual void setPixelPerfect(bool _value)
        {
            if (m_pixelPerfect == _value)
                return;

            m_pixelPerfect = _value;

            enableFlag(Flags::ModelMatricesDirty);
        }


        //////////////////////////////////////////
        // Processes all pending dirty flags. Called by the render system
        // once per frame; can be called manually to flush pending changes
        void prepareForRender();

        //////////////////////////////////////////
        // Immediate full rebuild (flushes all pending changes right away)
        void updateMeshData();


        //////////////////////////////////////////
        void updateMeshRendererColors();

        //////////////////////////////////////////
        void updateMeshRendererModelMatrices();

    protected:

        //////////////////////////////////////////
        AbstractTextRenderer2D();

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
        virtual void processTextDataChanged() MAZE_OVERRIDE
        {
            enableFlag(Flags::MeshDataDirty);
            enableFlag(Flags::MaterialDirty);
        }

        //////////////////////////////////////////
        virtual void processColorChanged() MAZE_OVERRIDE
        {
            enableFlag(Flags::ColorDirty);
        }


        //////////////////////////////////////////
        // Rebuilds m_localMatrices/m_localColors and the mesh from the current text data
        virtual void updateMeshDataNow() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void updateMaterial() MAZE_ABSTRACT;

        //////////////////////////////////////////
        // Rewrites m_localColors without relayout (GlyphColorsDirty)
        virtual void updateGlyphColors() { disableFlag(Flags::GlyphColorsDirty); }

        //////////////////////////////////////////
        // Uniform color multiplier for the whole mesh (ColorDirty pass)
        virtual Vec4F calculateMeshColor() const;

    protected:

        Transform2DPtr m_transform;
        CanvasRendererPtr m_canvasRenderer;
        MeshRendererInstancedPtr m_meshRenderer;

        Vec2F m_boundingSize = Vec2F::c_zero;

        bool m_pixelPerfect = true;

        Vector<TMat> m_localMatrices;
        Vector<Vec4F> m_localColors;

    private:
        U8 m_flags = 0u;
        bool m_updatingMeshData = false;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAbstractTextRenderer2D_hpp_
//////////////////////////////////////////
