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
#if (!defined(_MazeMetaPropertyDrawer_hpp_))
#define _MazeMetaPropertyDrawer_hpp_


//////////////////////////////////////////
#include "maze-debugger/MazeDebuggerHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(MetaPropertyDrawer);
    MAZE_USING_SHARED_PTR(Transform2D);


    //////////////////////////////////////////
    // Class MetaPropertyDrawer
    //
    //////////////////////////////////////////
    class MAZE_DEBUGGER_API MetaPropertyDrawer
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(MetaPropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(MetaPropertyDrawer);

    public:

        //////////////////////////////////////////
        virtual ~MetaPropertyDrawer();

        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label = nullptr) MAZE_ABSTRACT;

        //////////////////////////////////////////
        MetaProperty const* getMetaProperty() const { return m_metaProperty; }


        //////////////////////////////////////////
        void linkMetaInstances(
            Set<MetaInstance> const& _metaInstances);

        //////////////////////////////////////////
        virtual void processDataToUI() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void processDataFromUI() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void processMetaInstancesChanged() {};

    protected:

        //////////////////////////////////////////
        MetaPropertyDrawer();

        //////////////////////////////////////////
        virtual bool init(
            MetaProperty* _metaProperty);

    protected:
        MetaProperty* m_metaProperty;
        Set<MetaInstance> m_metaInstances;
    };


    //////////////////////////////////////////
    // Class GenericMetaPropertyDrawer
    //
    //////////////////////////////////////////
    template <typename TProperty>
    class GenericMetaPropertyDrawer
        : public MetaPropertyDrawer
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_AND_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(GenericMetaPropertyDrawer<TProperty>);


    public:

        //////////////////////////////////////////
        inline bool fetchPropertyValue(
            TProperty& _value,
            bool& _isMultiValue)
        {
            _isMultiValue = false;

            if (!m_metaInstances.empty())
            {
                m_metaProperty->getValue(*m_metaInstances.begin(), _value);
                for (Set<MetaInstance>::const_iterator  it = ++m_metaInstances.begin(),
                                                        end = m_metaInstances.end();
                                                        it != end;
                                                        ++it)
                {
                    TProperty value;
                    m_metaProperty->getValue(*it, value);
                    if (value != _value)
                    {
                        _isMultiValue = true;
                        return true;
                    }
                }
            }

            return false;
        }


    protected:

    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMetaPropertyDrawer_hpp_
//////////////////////////////////////////
