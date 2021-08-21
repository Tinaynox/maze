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
#if (!defined(_ViewTrigger_hpp_))
#define _ViewTrigger_hpp_


//////////////////////////////////////////
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include "game/DamageData.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ViewTrigger);


    //////////////////////////////////////////
    // Class ViewTrigger
    //
    //////////////////////////////////////////
    class ViewTrigger
    {
    public:

        //////////////////////////////////////////
        static ViewTriggerPtr Create();
    };


    //////////////////////////////////////////
    MAZE_NOT_IMPLEMENTED_SERIALIZATION(ViewTrigger);


    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<ViewTriggerPtr>::value), void>::type
        ValueToString(ViewTriggerPtr const& _value, String& _data)
    {
        MAZE_NOT_IMPLEMENTED;
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<ViewTriggerPtr>::value), void>::type
        ValueFromString(ViewTriggerPtr& _value, CString _data, Size _count)
    {
        MAZE_NOT_IMPLEMENTED;
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<ViewTriggerPtr>::value), U32>::type
        GetValueSerializationSize(ViewTriggerPtr const& _value)
    {
        MAZE_NOT_IMPLEMENTED_RETURN_VALUE(0);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<ViewTriggerPtr>::value), void>::type
        SerializeValue(ViewTriggerPtr const& _value, U8* _data)
    {
        MAZE_NOT_IMPLEMENTED;
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<ViewTriggerPtr>::value), void>::type
        DeserializeValue(ViewTriggerPtr& _value, U8 const* _data)
    {
        MAZE_NOT_IMPLEMENTED;
    }

} // namespace Maze
//////////////////////////////////////////



#endif // _ViewTrigger_hpp_
//////////////////////////////////////////
