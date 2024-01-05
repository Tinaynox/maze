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
#if (!defined(_MazeFontMaterial_hpp_))
#define _MazeFontMaterial_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"
#include "maze-ui/fonts/MazeFontGlyph.hpp"
#include "maze-core/serialization/MazeXMLSerializable.hpp"
#include "maze-core/serialization/MazeDataBlockSerializable.hpp"
#include "maze-core/data/MazeDataBlock.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(FontMaterial);
    MAZE_USING_SHARED_PTR(FontMaterialManager);
    MAZE_USING_SHARED_PTR(AssetFile);
    MAZE_USING_SHARED_PTR(Font);
    MAZE_USING_SHARED_PTR(Material);


    //////////////////////////////////////////
    // Struct FontMaterialRenderData
    //
    //////////////////////////////////////////
    struct MAZE_UI_API FontMaterialRenderData
    {
        Vector<Texture2D*> textures;
        Map<Texture2D*, S32> textureIndices;
        MaterialPtr material;
        bool texturesDirty = true;

        //////////////////////////////////////////
        void updateMaterialUniforms();
    };


    //////////////////////////////////////////
    using FontMaterialRenderDataPtr = Maze::SharedPtr<struct FontMaterialRenderData>;


    //////////////////////////////////////////
    // Class FontMaterial
    //
    //////////////////////////////////////////
    class MAZE_UI_API FontMaterial
        : public SharedObject<FontMaterial>
        , public MultiDelegateCallbackReceiver
        , public IDataBlockSerializable
        , public IStringSerializable
    {
    public:

        //////////////////////////////////////////
        static FontMaterialPtr Create();

        //////////////////////////////////////////
        static FontMaterialPtr Create(
            AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        virtual ~FontMaterial();


        //////////////////////////////////////////
        inline String const& getName() const { return m_name; }

        //////////////////////////////////////////
        inline void setName(String const& _value) { m_name = _value; }


        //////////////////////////////////////////
        inline FontPtr const& getFont() const { return m_font; }

        //////////////////////////////////////////
        void setFont(FontPtr const& _value);

        //////////////////////////////////////////
        inline MaterialPtr const& getAssetMaterial() const { return m_assetMaterial; }

        //////////////////////////////////////////
        void setAssetMaterial(MaterialPtr const& _value);


        //////////////////////////////////////////
        FontMaterialRenderData const& fetchRenderData(U32 _fontSize);

        //////////////////////////////////////////
        MaterialPtr const& fetchMaterial(U32 _fontSize);

        //////////////////////////////////////////
        virtual bool loadFromAssetFile(
            AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        bool loadFromXMLDocument(tinyxml2::XMLDocument& _doc);


        //////////////////////////////////////////
        S32 getTextureIndex(U32 _fontSize, Texture2D* _texture);

    public:

        //////////////////////////////////////////
        virtual bool loadFromDataBlock(DataBlock const& _dataBlock) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void toDataBlock(DataBlock& _dataBlock) const MAZE_OVERRIDE;

    public:

        //////////////////////////////////////////
        virtual String toString() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setString(CString _data, Size _count) MAZE_OVERRIDE;

    public:

        //////////////////////////////////////////
        static void FromString(FontMaterialPtr& _value, CString _data, Size _count);

        //////////////////////////////////////////
        static void ToString(FontMaterial const* _value, String& _data);

    public:

        //////////////////////////////////////////
        MultiDelegate<> eventTexturesChanged;
        MultiDelegate<> eventMaterialChanged;

    protected:

        //////////////////////////////////////////
        FontMaterial();

        //////////////////////////////////////////
        virtual bool init();


        //////////////////////////////////////////
        void updateFontTextures(U32 _fontSize);

        //////////////////////////////////////////
        void notifyTexturesChanged();

    protected:
        String m_name;

        FontPtr m_font;
        MaterialPtr m_assetMaterial;

    protected:

        Map<U32, FontMaterialRenderDataPtr> m_renderData;
    };


    //////////////////////////////////////////
    // Class FontMaterialAssetRef
    //
    //////////////////////////////////////////
    class MAZE_UI_API FontMaterialAssetRef
        : public IStringSerializable
    {
    public:

        //////////////////////////////////////////
        FontMaterialAssetRef(FontMaterialPtr const& _value = nullptr)
            : m_fontMaterial(_value)
        {}

        //////////////////////////////////////////
        void setFontMaterial(FontMaterialPtr const& _value) { m_fontMaterial = _value; }

        //////////////////////////////////////////
        inline FontMaterialPtr const& getFontMaterial() const { return m_fontMaterial; }

        //////////////////////////////////////////
        virtual String toString() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setString(CString _data, Size _count) MAZE_OVERRIDE;


        //////////////////////////////////////////
        inline bool operator==(FontMaterialAssetRef const& _value) const { return m_fontMaterial == _value.getFontMaterial(); }

        //////////////////////////////////////////
        inline bool operator!=(FontMaterialAssetRef const& _value) const { return m_fontMaterial != _value.getFontMaterial(); }

    private:
        FontMaterialPtr m_fontMaterial;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeFontMaterial_hpp_
//////////////////////////////////////////
