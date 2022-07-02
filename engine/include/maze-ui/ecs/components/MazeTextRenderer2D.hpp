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
#if (!defined(_MazeTextRenderer2D_hpp_))
#define _MazeTextRenderer2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeAlignment2D.hpp"
#include "maze-graphics/ecs/components/MazeAbstractTextRenderer2D.hpp"
#include "maze-ui/fonts/MazeFont.hpp"
#include "maze-ui/fonts/MazeFontMaterial.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Material);
    MAZE_USING_SHARED_PTR(Mesh);
    MAZE_USING_SHARED_PTR(SubMesh);
    MAZE_USING_SHARED_PTR(TextRenderer2D);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(MeshRendererInstanced);
    MAZE_USING_SHARED_PTR(CanvasRenderer);


    //////////////////////////////////////////
    // Enum TextRenderer2DSymbolsLimitPolicy
    //
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_4_API(MAZE_GRAPHICS_API, TextRenderer2DSymbolsLimitPolicy,
        DenyInput,
        Crop,
        Dot,
        ThreeDots);


    //////////////////////////////////////////
    // Enum TextRenderer2DWidthPolicy
    //
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_2_API(MAZE_GRAPHICS_API, TextRenderer2DWidthPolicy,
        Abut,
        WordWrap);


    //////////////////////////////////////////
    // Enum TextRenderer2DStyle
    //
    //////////////////////////////////////////
    namespace TextRenderer2DStyle
    {
        ////////////////////////////////////
        enum Enum
        {
            CropBottomIndent    = MAZE_BIT(0),
            CropTopIndent       = MAZE_BIT(1),
            PasswordMode        = MAZE_BIT(2),
            ColorTags           = MAZE_BIT(3),
        };
    };


    //////////////////////////////////////////
    // Class TextRenderer2D
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API TextRenderer2D
        : public AbstractTextRenderer2D
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(TextRenderer2D, AbstractTextRenderer2D);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(TextRenderer2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~TextRenderer2D();

        //////////////////////////////////////////
        static TextRenderer2DPtr Create(RenderSystem* _renderSystem = nullptr);


        //////////////////////////////////////////
        MeshRendererInstancedPtr const& getMeshRenderer() const { return m_meshRenderer; }


        //////////////////////////////////////////
        virtual void setText(String const& _text) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual String const& getText() const MAZE_OVERRIDE { return m_text; }


        //////////////////////////////////////////
        void removeSymbol();

        //////////////////////////////////////////
        Size getSymbolsCount();


        //////////////////////////////////////////
        using AbstractTextRenderer2D::setColor;

        //////////////////////////////////////////
        virtual void setColor(ColorU32 _color) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual ColorU32 getColor() const MAZE_OVERRIDE { return m_color; }


        //////////////////////////////////////////
        inline FontMaterialPtr const& getFontMaterial() const { return m_fontMaterial; }

        //////////////////////////////////////////
        void setFontMaterial(FontMaterialPtr const& _fontMaterial);


        //////////////////////////////////////////
        inline U32 getFontSize() const { return m_fontSize; }

        //////////////////////////////////////////
        void setFontSize(U32 _fontSize)
        {
            if (m_fontSize == _fontSize)
                return;

            m_fontSize = _fontSize;

            updateMaterial();
            updateMeshData();
        }


        //////////////////////////////////////////
        inline U32 getStyles() const { return m_styles; }

        //////////////////////////////////////////
        void setStyles(U32 _style);

        //////////////////////////////////////////
        void addStyle(TextRenderer2DStyle::Enum _style) { setStyles(m_styles | _style); }

        //////////////////////////////////////////
        void removeStyle(U32 _style) { setStyles(m_styles & ~_style); }

        //////////////////////////////////////////
        inline bool getStyle(TextRenderer2DStyle::Enum _style) const { return (m_styles & _style) != 0; }

        //////////////////////////////////////////
        inline void setStyle(TextRenderer2DStyle::Enum _style, bool _value) { _value ? addStyle(_style) : removeStyle(_style); }


        //////////////////////////////////////////
        inline bool getCropBottomIndent() const { return getStyle(TextRenderer2DStyle::CropBottomIndent); }

        //////////////////////////////////////////
        inline void setCropBottomIndent(bool _value) { setStyle(TextRenderer2DStyle::CropBottomIndent, _value); }

        //////////////////////////////////////////
        inline bool getCropTopIndent() const { return getStyle(TextRenderer2DStyle::CropTopIndent); }

        //////////////////////////////////////////
        inline void setCropTopIndent(bool _value) { setStyle(TextRenderer2DStyle::CropTopIndent, _value); }

        //////////////////////////////////////////
        inline bool getPasswordMode() const { return getStyle(TextRenderer2DStyle::PasswordMode); }

        //////////////////////////////////////////
        inline void setPasswordMode(bool _value) { setStyle(TextRenderer2DStyle::PasswordMode, _value); }

        //////////////////////////////////////////
        inline bool getColorTags() const { return getStyle(TextRenderer2DStyle::ColorTags); }

        //////////////////////////////////////////
        inline void setColorTags(bool _value) { setStyle(TextRenderer2DStyle::ColorTags, _value); }



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



        ////////////////////////////////////
        inline void setSymbolsLimit(U32 _symbolsLimit)
        {
            if (m_symbolsLimit == _symbolsLimit)
                return;

            m_symbolsLimit = _symbolsLimit;

            updateMeshData();
        }

        ////////////////////////////////////
        inline U32 getSymbolsLimit() const { return m_symbolsLimit; }


        //////////////////////////////////////////
        inline TextRenderer2DSymbolsLimitPolicy getSymbolsLimitPolicy() const { return m_symbolsLimitPolicy; }

        //////////////////////////////////////////
        inline void setSymbolsLimitPolicy(TextRenderer2DSymbolsLimitPolicy _value)
        {
            if (m_symbolsLimitPolicy == _value)
                return;

            m_symbolsLimitPolicy = _value;

            updateMeshData();
        }


        //////////////////////////////////////////
        inline TextRenderer2DWidthPolicy getWidthPolicy() const { return m_widthPolicy; }

        //////////////////////////////////////////
        inline void setWidthPolicy(TextRenderer2DWidthPolicy _value)
        {
            if (m_widthPolicy == _value)
                return;

            m_widthPolicy = _value;

            updateMeshData();
        }

        //////////////////////////////////////////
        inline F32 getOutlineThickness() const { return m_outlineThickness; }

        //////////////////////////////////////////
        inline void setOutlineThickness(F32 _outlineThickness)
        {
            if (m_outlineThickness == _outlineThickness)
                return;

            m_outlineThickness = _outlineThickness;

            updateMeshData();
        }

        //////////////////////////////////////////
        inline ColorU32 const& getOutlineColor() const { return m_outlineColor; }

        //////////////////////////////////////////
        inline void setOutlineColor(ColorU32 const& _color)
        {
            if (m_outlineColor == _color)
                return;

            m_outlineColor = _color;

            updateMeshData();
        }


        //////////////////////////////////////////
        virtual Vec2DF getTextEnd(Size _rowIndex = 0) MAZE_OVERRIDE;


        //////////////////////////////////////////
        void updateMeshData();


        //////////////////////////////////////////
        void updateMeshRendererColors();

        //////////////////////////////////////////
        void updateMeshRendererModelMatrices();

    protected:

        //////////////////////////////////////////
        TextRenderer2D();

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
        void updateMaterial();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;


        //////////////////////////////////////////
        F32 calculateXAlignOffset(FastVector<F32> const& _rowLengths, U32 _currentRow);

        //////////////////////////////////////////
        void setGlyphQuad(
            Size _quadIndex,
            Vec2DF const& _position,
            ColorF128 const& _color,
            FontGlyph const& _glyph);

        //////////////////////////////////////////
        void notifyFontMaterialTexturesChanged();

        //////////////////////////////////////////
        void notifyFontMaterialMaterialChanged();


        //////////////////////////////////////////
        F32 calculateY(
            F32 _totalTextHeight,
            Size _actualRowsCount);

    protected:
        MeshRendererInstancedPtr m_meshRenderer;

    protected:
        String m_text;

        ShaderUniformVariantPtr m_colorUniform;
        ShaderUniformVariantPtr m_baseMapUniform;
        ShaderUniformVariantPtr m_baseMapTexelSizeUniform;

        ColorU32 m_color = ColorU32::c_white;

        FontMaterialPtr m_fontMaterial;
        U32 m_fontSize = 18u;

        U32 m_styles = TextRenderer2DStyle::ColorTags;

        HorizontalAlignment2D m_horizontalAlignment = HorizontalAlignment2D::Left;
        VerticalAlignment2D m_verticalAlignment = VerticalAlignment2D::Top;

        U32 m_symbolsLimit = 0u;
        TextRenderer2DSymbolsLimitPolicy m_symbolsLimitPolicy = TextRenderer2DSymbolsLimitPolicy::Crop;
        TextRenderer2DWidthPolicy m_widthPolicy = TextRenderer2DWidthPolicy::None;

        F32 m_outlineThickness = 0.0f;
        ColorU32 m_outlineColor = ColorU32::c_white;

    private:
        Vector<Mat4DF> m_localMatrices;
        Vector<Vec4DF> m_localColors;

        Vec2DF m_lastGlyphOffset = Vec2DF::c_zero;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTextRenderer2D_hpp_
//////////////////////////////////////////
