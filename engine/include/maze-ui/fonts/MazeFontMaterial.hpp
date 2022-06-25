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
    // Class FontMaterial
    //
    //////////////////////////////////////////
    class MAZE_UI_API FontMaterial
        : public SharedObject<FontMaterial>
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
        inline void setFont(FontPtr const& _value) { m_font = _value; }

        //////////////////////////////////////////
        inline MaterialPtr const& getMaterial() const { return m_material; }

        //////////////////////////////////////////
        inline void setMaterial(MaterialPtr const& _value) { m_material = _value; }


        //////////////////////////////////////////
        virtual bool loadFromAssetFile(
            AssetFilePtr const& _assetFile);

    public:

        //////////////////////////////////////////
        static void FromString(FontMaterialPtr& _value, CString _data, Size _count);

        //////////////////////////////////////////
        static void ToString(FontMaterial const* _value, String& _data);

    protected:

        //////////////////////////////////////////
        FontMaterial();

        //////////////////////////////////////////
        virtual bool init();

    protected:
        String m_name;

        FontPtr m_font;
        MaterialPtr m_material;
    };


    //////////////////////////////////////////
    MAZE_NOT_IMPLEMENTED_SERIALIZATION(FontMaterial);


    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<FontMaterialPtr>::value), void>::type
        ValueToString(FontMaterialPtr const& _value, String& _data)
    {
        FontMaterial::ToString(_value.get(), _data);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<FontMaterialPtr>::value), void>::type
        ValueFromString(FontMaterialPtr& _value, CString _data, Size _count)
    {
        FontMaterial::FromString(_value, _data, _count);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<FontMaterialPtr>::value), U32>::type
        GetValueSerializationSize(FontMaterialPtr const& _value)
    {
        MAZE_NOT_IMPLEMENTED_RETURN_VALUE(0);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<FontMaterialPtr>::value), void>::type
        SerializeValue(FontMaterialPtr const& _value, U8* _data)
    {
        MAZE_NOT_IMPLEMENTED;
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<FontMaterialPtr>::value), void>::type
        DeserializeValue(FontMaterialPtr& _value, U8 const* _data)
    {
        MAZE_NOT_IMPLEMENTED;
    }
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeFontMaterial_hpp_
//////////////////////////////////////////
