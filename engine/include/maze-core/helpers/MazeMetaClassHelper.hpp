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
#if (!defined(_MazeMetaClassHelper_hpp_))
#define _MazeMetaClassHelper_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"


//////////////////////////////////////////
namespace Maze
{
    
    //////////////////////////////////////////
    namespace MetaClassHelper
    {
        ////////////////////////////////////
        MAZE_CORE_API void CopyProperties(void* _destObject, MetaClass const* _metaClass, void const* _srcObject);

        //////////////////////////////////////////
        MAZE_CORE_API bool CopyProperties(MetaInstance _dstMetaInstance, ConstMetaInstance _srcMetaInstance);

        //////////////////////////////////////////
        MAZE_CORE_API bool IsEqual(ConstMetaInstance _metaInstance0, ConstMetaInstance _metaInstance1);

        //////////////////////////////////////////
        template <typename TValue> 
        inline bool IsValueEqual(
            TValue const& _value,
            MetaProperty* _metaProperty,
            Set<MetaInstance> const& _metaInstances)
        {
            if (_metaInstances.empty())
                return false;

            bool result = true;
            for (MetaInstance const& metaInstance : _metaInstances)
            {
                TValue prevValue;
                _metaProperty->getValue(metaInstance, &prevValue);
                if (_value != prevValue)
                {
                    result = false;
                    break;
                }
            }

            return result;
        }

        //////////////////////////////////////////
        template <typename TValue>
        inline void SetValue(
            TValue const& _value,
            MetaProperty* _metaProperty,
            Set<MetaInstance> const& _metaInstances)
        {
            for (MetaInstance const& metaInstance : _metaInstances)
                _metaProperty->setValue(metaInstance, &_value);
        }

        //////////////////////////////////////////
        MAZE_CORE_API void SetValueString(
            String const& _value,
            MetaProperty* _metaProperty,
            Set<MetaInstance> const& _metaInstances);


    } // namespace MetaClassHelper
    //////////////////////////////////////////
    
    
} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMetaClassHelper_hpp_
//////////////////////////////////////////
