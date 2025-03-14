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
#if (!defined(_MazeSystemTextRenderer2D_hpp_))
#define _MazeSystemTextRenderer2D_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeAlignment2D.hpp"
#include "maze-graphics/MazeSystemFont.hpp"
#include "maze-graphics/ecs/components/MazeAbstractTextRenderer2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(Material);
    MAZE_USING_MANAGED_SHARED_PTR(Mesh);
    MAZE_USING_MANAGED_SHARED_PTR(SubMesh);
    MAZE_USING_SHARED_PTR(SystemTextRenderer2D);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(MeshRendererInstanced);
    MAZE_USING_SHARED_PTR(CanvasRenderer);


    //////////////////////////////////////////
    // Class SystemTextRenderer2D
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API SystemTextRenderer2D
        : public AbstractTextRenderer2D
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SystemTextRenderer2D, AbstractTextRenderer2D);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(SystemTextRenderer2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~SystemTextRenderer2D();

        //////////////////////////////////////////
        static SystemTextRenderer2DPtr Create(RenderSystem* _renderSystem = nullptr);


        //////////////////////////////////////////
        MeshRendererInstancedPtr const& getMeshRenderer() const { return m_meshRenderer; }


        //////////////////////////////////////////
        virtual void setText(String const& _text) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual String const& getText() const MAZE_OVERRIDE { return m_text; }


        //////////////////////////////////////////
        using AbstractTextRenderer2D::setColor;

        //////////////////////////////////////////
        virtual void setColor(ColorU32 _color) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual ColorU32 getColor() const MAZE_OVERRIDE { return m_color; }


        //////////////////////////////////////////
        inline SystemFontPtr const& getSystemFont() const { return m_systemFont; }

        //////////////////////////////////////////
        void setSystemFont(SystemFontPtr const& _systemFont);


        //////////////////////////////////////////
        inline U32 getFontSize() const { return m_fontSize; }

        //////////////////////////////////////////
        void setFontSize(U32 _fontSize)
        {
            if (m_fontSize == _fontSize)
                return;

            m_fontSize = _fontSize;

            updateMeshData();
        }

        //////////////////////////////////////////
        virtual F32 getLineSpacingScale() const MAZE_OVERRIDE { return m_lineSpacingScale; }

        //////////////////////////////////////////
        virtual void setLineSpacingScale(F32 _value) MAZE_OVERRIDE
        {
            if (m_lineSpacingScale == _value)
                return;

            m_lineSpacingScale = _value;

            updateMeshData();
        }


        //////////////////////////////////////////
        virtual HorizontalAlignment2D getHorizontalAlignment() const MAZE_OVERRIDE { return m_horizontalAlignment; }

        //////////////////////////////////////////
        virtual void setHorizontalAlignment(HorizontalAlignment2D _horizontalAlignment) MAZE_OVERRIDE
        {
            if (m_horizontalAlignment == _horizontalAlignment)
                return;

            m_horizontalAlignment = _horizontalAlignment;

            updateMeshData();
        }


        //////////////////////////////////////////
        virtual VerticalAlignment2D getVerticalAlignment() const MAZE_OVERRIDE { return m_verticalAlignment; }

        //////////////////////////////////////////
        virtual void setVerticalAlignment(VerticalAlignment2D _verticalAlignment) MAZE_OVERRIDE
        {
            if (m_verticalAlignment == _verticalAlignment)
                return;

            m_verticalAlignment = _verticalAlignment;

            updateMeshData();
        }


        //////////////////////////////////////////
        virtual bool getPixelPerfect() const MAZE_OVERRIDE { return m_pixelPerfect; }

        //////////////////////////////////////////
        virtual void setPixelPerfect(bool _value) MAZE_OVERRIDE
        {
            if (m_pixelPerfect == _value)
                return;

            m_pixelPerfect = _value;

            updateMeshRendererModelMatrices();
        }


        //////////////////////////////////////////
        void updateMeshData();


        //////////////////////////////////////////
        virtual Vec2F getTextEnd(Size _rowIndex = 0) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void updateMeshRendererColors();

        //////////////////////////////////////////
        void updateMeshRendererModelMatrices();

    protected:

        //////////////////////////////////////////
        SystemTextRenderer2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(RenderSystem* _renderSystem = nullptr);

        //////////////////////////////////////////
        virtual bool init(
            Component* _component,
            EntityCopyData _copyData) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void updateMaterial();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void calculateTextData(
            S32& _rowsCount,
            S32& _maxColumnsCount,
            S32& _charsCount);

    protected:
        MeshRendererInstancedPtr m_meshRenderer;

        String m_text;

        ShaderUniformVariantPtr m_colorUniform;
        ShaderUniformVariantPtr m_baseMapUniform;
        ShaderUniformVariantPtr m_baseMapTexelSizeUniform;

        ColorU32 m_color;

        SystemFontPtr m_systemFont;
        U32 m_fontSize;
        F32 m_lineSpacingScale = 1.0f;

        HorizontalAlignment2D m_horizontalAlignment;
        VerticalAlignment2D m_verticalAlignment;
        bool m_pixelPerfect = true;

        Vector<TMat> m_localMatrices;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSystemTextRenderer2D_hpp_
//////////////////////////////////////////
