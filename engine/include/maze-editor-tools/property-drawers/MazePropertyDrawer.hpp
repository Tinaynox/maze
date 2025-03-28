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
#if (!defined(_MazePropertyDrawer_hpp_))
#define _MazePropertyDrawer_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/data/MazeDataBlock.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PropertyDrawer);
    MAZE_USING_SHARED_PTR(Transform2D);


    //////////////////////////////////////////
    // Class PropertyDrawer
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API PropertyDrawer
        : public SharedObject<PropertyDrawer>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(PropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PropertyDrawer);

    public:

        //////////////////////////////////////////
        virtual ~PropertyDrawer();

        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label = nullptr) MAZE_ABSTRACT;

        ////////////////////////////////////////////
        virtual void unselectUI() {};


        ////////////////////////////////////////////
        virtual bool toDataBlock(DataBlock& _value) const MAZE_ABSTRACT;

        ////////////////////////////////////////////
        virtual bool setDataBlock(DataBlock const& _value) MAZE_ABSTRACT;

    public:

        //////////////////////////////////////////
        MultiDelegate<> eventUIData;

    protected:

        //////////////////////////////////////////
        PropertyDrawer();

        //////////////////////////////////////////
        virtual bool init(DataBlock const& _dataBlock);

    protected:
        DataBlock m_dataBlock;
    };


    //////////////////////////////////////////
    // Class GenericPropertyDrawer
    //
    //////////////////////////////////////////
    template <typename TProperty>
    class GenericPropertyDrawer
        : public PropertyDrawer
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_AND_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(GenericPropertyDrawer<TProperty>);

        //////////////////////////////////////////
        using DataFromUICallback = std::function<void(TProperty const&)>;


    public:

        //////////////////////////////////////////
        virtual void setValue(TProperty const& _value) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual TProperty getValue() const MAZE_ABSTRACT;


        ////////////////////////////////////////////
        virtual bool toDataBlock(DataBlock& _dataBlock) const MAZE_OVERRIDE
        {
            bool result = TryValueToDataBlock<TProperty>(getValue(), _dataBlock);
            if (!result)
            {
#if MAZE_DEBUG
                MAZE_NOT_IMPLEMENTED;
#endif
            }

            return result;
        }

        ////////////////////////////////////////////
        virtual bool setDataBlock(DataBlock const& _dataBlock) MAZE_OVERRIDE
        {
            TProperty value;
            bool result = TryValueFromDataBlock<TProperty>(value, _dataBlock);
            if (result)
            {
                setValue(value);
            }
            else
            {
#if MAZE_DEBUG
                MAZE_NOT_IMPLEMENTED;
#endif
            }

            return result;
        }
    
    public:
        
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazePropertyDrawer_hpp_
//////////////////////////////////////////
