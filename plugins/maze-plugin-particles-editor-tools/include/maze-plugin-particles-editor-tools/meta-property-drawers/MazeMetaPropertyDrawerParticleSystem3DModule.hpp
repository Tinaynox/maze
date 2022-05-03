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
#if (!defined(_MazeMetaPropertyDrawerParticleSystem3DModule_hpp_))
#define _MazeMetaPropertyDrawerParticleSystem3DModule_hpp_


//////////////////////////////////////////
#include "maze-particles/MazeParticlesHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMetaPropertyDrawer.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMetaPropertyDrawerDefault.hpp"
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SystemTextEditBox2D);
    MAZE_USING_SHARED_PTR(ToggleButton2D);
    MAZE_USING_SHARED_PTR(HorizontalLayout2D);


    //////////////////////////////////////////
    // Class MetaPropertyDrawerParticleSystem3DModule
    //
    //////////////////////////////////////////
    template <typename TProperty>
    class MetaPropertyDrawerParticleSystem3DModule
        : public GenericMetaPropertyDrawer<TProperty>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_AND_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MetaPropertyDrawerParticleSystem3DModule<TProperty>);

    public:

        //////////////////////////////////////////
        template <typename TPropertyDrawer>
        inline void registerCustomPropertyDrawer(CString _propertyName)
        {
            MetaProperty* metaProperty = TProperty::GetMetaClass()->getProperty(_propertyName);
            MAZE_ERROR_RETURN_IF(!metaProperty, "Undefined property - %s", _propertyName);

            registerCustomPropertyDrawer<TPropertyDrawer>(metaProperty);
        }

        //////////////////////////////////////////
        template <typename TPropertyDrawer>
        inline void registerCustomPropertyDrawer(MetaProperty* _metaProperty)
        {
            registerCustomPropertyDrawer(
                _metaProperty,
                [](MetaProperty* _metaProperty)
                {
                    return TPropertyDrawer::Create(_metaProperty);
                });
        }

        //////////////////////////////////////////
        inline void registerCustomPropertyDrawer(
            MetaProperty* _metaProperty,
            std::function<MetaPropertyDrawerPtr(MetaProperty*)> _func)
        {
            m_customPropertyDrawers.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(_metaProperty),
                std::forward_as_tuple(_func));
        }

        //////////////////////////////////////////
        MetaPropertyDrawerPtr buildPropertyDrawer(
            MetaProperty* _metaProperty,
            CString _label = nullptr)
        {
            MetaPropertyDrawerPtr property;

            auto const& it = m_customPropertyDrawers.find(_metaProperty);
            if (it != m_customPropertyDrawers.end())
            {
                property = it->second(_metaProperty);
            }
            else
            {
                property = InspectorManager::GetInstancePtr()->createMetaPropertyDrawer(_metaProperty);
            }

            return property;
        }

    protected:

        Map<MetaProperty*, std::function<MetaPropertyDrawerPtr(MetaProperty*)>> m_customPropertyDrawers;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMetaPropertyDrawerParticleSystem3DModule_hpp_
//////////////////////////////////////////
