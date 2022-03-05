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
#if (!defined(_MazeSystemFontParams_hpp_))
#define _MazeSystemFontParams_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/math/MazeVec3D.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeMaterial.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SystemFont);


    //////////////////////////////////////////
    class MAZE_GRAPHICS_API SystemFont
        : public SharedObject<SystemFont>
    {
    public:

        Texture2DPtr texture;
        MaterialPtr material;
        Vec2DS charSize = Vec2DS::c_zero;
        Vec2DS stroke = Vec2DS::c_zero;
        Vec2DS offset = Vec2DS::c_zero;
        S32 outline = 0;

    public:

        //////////////////////////////////////////
        static void FromString(SystemFontPtr& _value, CString _data, Size _count);

        //////////////////////////////////////////
        static void ToString(SystemFont const* _value, String& _data);
    };

    //////////////////////////////////////////
    MAZE_NOT_IMPLEMENTED_SERIALIZATION(SystemFont);


    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<SystemFontPtr>::value), void>::type
        ValueToString(SystemFontPtr const& _value, String& _data)
    {
        SystemFont::ToString(_value.get(), _data);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<SystemFontPtr>::value), void>::type
        ValueFromString(SystemFontPtr& _value, CString _data, Size _count)
    {
        SystemFont::FromString(_value, _data, _count);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<SystemFontPtr>::value), U32>::type
        GetValueSerializationSize(SystemFontPtr const& _value)
    {
        MAZE_NOT_IMPLEMENTED_RETURN_VALUE(0);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<SystemFontPtr>::value), void>::type
        SerializeValue(SystemFontPtr const& _value, U8* _data)
    {
        MAZE_NOT_IMPLEMENTED;
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<SystemFontPtr>::value), void>::type
        DeserializeValue(SystemFontPtr& _value, U8 const* _data)
    {
        MAZE_NOT_IMPLEMENTED;
    }

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSystemFontParams_hpp_
//////////////////////////////////////////
