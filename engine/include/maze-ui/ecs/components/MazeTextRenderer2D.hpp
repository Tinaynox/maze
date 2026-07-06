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
    MAZE_USING_MANAGED_SHARED_PTR(Material);
    MAZE_USING_MANAGED_SHARED_PTR(Mesh);
    MAZE_USING_MANAGED_SHARED_PTR(SubMesh);
    MAZE_USING_SHARED_PTR(TextRenderer2D);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(MeshRendererInstanced);
    MAZE_USING_SHARED_PTR(CanvasRenderer);


    //////////////////////////////////////////
    // Enum TextRenderer2DSymbolsLimitPolicy
    //
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_4_API(MAZE_UI_API, TextRenderer2DSymbolsLimitPolicy,
        DenyInput,
        Crop,
        Dot,
        ThreeDots);


    //////////////////////////////////////////
    // Enum TextRenderer2DWidthPolicy
    //
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_2_API(MAZE_UI_API, TextRenderer2DWidthPolicy,
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
    class MAZE_UI_API TextRenderer2D
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
        void removeSymbol();

        //////////////////////////////////////////
        Size getSymbolsCount();


        //////////////////////////////////////////
        inline FontMaterialAssetRef const& getFontMaterialRef() const { return m_fontMaterialRef; }

        //////////////////////////////////////////
        void setFontMaterialRef(FontMaterialAssetRef const& _fontMaterial);
        
        //////////////////////////////////////////
        inline FontMaterialPtr const& getFontMaterial() const { return m_fontMaterialRef.getFontMaterial(); }

        //////////////////////////////////////////
        inline void setFontMaterial(FontMaterialPtr const& _fontMaterial) { setFontMaterialRef(FontMaterialAssetRef(_fontMaterial)); }


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
        inline void setSymbolsLimit(U32 _symbolsLimit)
        {
            if (m_symbolsLimit == _symbolsLimit)
                return;

            m_symbolsLimit = _symbolsLimit;

            enableFlag(Flags::MeshDataDirty);
        }

        //////////////////////////////////////////
        inline U32 getSymbolsLimit() const { return m_symbolsLimit; }


        //////////////////////////////////////////
        inline TextRenderer2DSymbolsLimitPolicy getSymbolsLimitPolicy() const { return m_symbolsLimitPolicy; }

        //////////////////////////////////////////
        inline void setSymbolsLimitPolicy(TextRenderer2DSymbolsLimitPolicy _value)
        {
            if (m_symbolsLimitPolicy == _value)
                return;

            m_symbolsLimitPolicy = _value;

            enableFlag(Flags::MeshDataDirty);
        }


        //////////////////////////////////////////
        inline TextRenderer2DWidthPolicy getWidthPolicy() const { return m_widthPolicy; }

        //////////////////////////////////////////
        inline void setWidthPolicy(TextRenderer2DWidthPolicy _value)
        {
            if (m_widthPolicy == _value)
                return;

            m_widthPolicy = _value;

            enableFlag(Flags::MeshDataDirty);
        }

        //////////////////////////////////////////
        inline F32 getOutlineThickness() const { return m_outlineThickness; }

        //////////////////////////////////////////
        inline void setOutlineThickness(F32 _outlineThickness)
        {
            if (m_outlineThickness == _outlineThickness)
                return;

            m_outlineThickness = _outlineThickness;

            enableFlag(Flags::MeshDataDirty);
        }

        //////////////////////////////////////////
        inline ColorU32 const& getOutlineColor() const { return m_outlineColor; }

        //////////////////////////////////////////
        inline void setOutlineColor(ColorU32 const& _color)
        {
            if (m_outlineColor == _color)
                return;

            m_outlineColor = _color;

            // Outline quads are never affected by color tags - cheap recolor is enough
            enableFlag(Flags::GlyphColorsDirty);
        }


        //////////////////////////////////////////
        inline bool getBold() const { return m_bold; }

        //////////////////////////////////////////
        inline void setBold(bool _bold)
        {
            if (m_bold == _bold)
                return;

            m_bold = _bold;

            enableFlag(Flags::MeshDataDirty);
            enableFlag(Flags::MaterialDirty);
        }


        //////////////////////////////////////////
        virtual Vec2F getTextEnd(Size _rowIndex = 0) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Vec2F getSymbolPosition(Size _symbolIndex) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Size getSymbolIndexAtPosition(Vec2F const& _position) MAZE_OVERRIDE;


        //////////////////////////////////////////
        void ensureAllGlyphs();

        //////////////////////////////////////////
        Vec2F calculateRequiredSizeForText(CString _text);

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
            EntityCopyData _copyData) MAZE_OVERRIDE;

        //////////////////////////////////////////
        // Full glyph relayout (ensureAllGlyphs + mesh rebuild)
        virtual void updateMeshDataNow() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void updateMaterial() MAZE_OVERRIDE;

        //////////////////////////////////////////
        // Rewrites m_localColors from m_color/m_outlineColor without relayout
        virtual void updateGlyphColors() MAZE_OVERRIDE;

        //////////////////////////////////////////
        // Glyph colors may derive from the base color via color tags ('#{-}') -
        // only then a full relayout is required
        virtual void processColorChanged() MAZE_OVERRIDE
        {
            if (m_hasActiveColorTags)
                enableFlag(Flags::MeshDataDirty);
            else
                enableFlag(Flags::GlyphColorsDirty);
        }

        //////////////////////////////////////////
        void setGlyphQuad(
            Size _quadIndex,
            Vec2F const& _position,
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

        //////////////////////////////////////////
        // Caret slots of the final text: one per symbol plus a trailing end slot,
        // each slot is the row index and the x offset within that row
        void calculateSymbolSlots(
            Vector<Pair<S32, F32>>& _outSlots,
            Vector<F32>& _outRowLengths);

    protected:
        FontMaterialAssetRef m_fontMaterialRef;

        U32 m_styles = TextRenderer2DStyle::ColorTags;

        U32 m_symbolsLimit = 0u;
        TextRenderer2DSymbolsLimitPolicy m_symbolsLimitPolicy = TextRenderer2DSymbolsLimitPolicy::Crop;
        TextRenderer2DWidthPolicy m_widthPolicy = TextRenderer2DWidthPolicy::None;

        F32 m_outlineThickness = 0.0f;
        ColorU32 m_outlineColor = ColorU32::c_white;

        bool m_bold = false;

    private:
        // Outline quads occupy [0, m_outlineQuadsCount) of the local buffers
        Size m_outlineQuadsCount = 0u;

        // True when the current text actually contains color tags -
        // glyph colors then depend on the tags and require a full relayout
        bool m_hasActiveColorTags = false;

        Vec2F m_lastGlyphOffset = Vec2F::c_zero;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTextRenderer2D_hpp_
//////////////////////////////////////////
