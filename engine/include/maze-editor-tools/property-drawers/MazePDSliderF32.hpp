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
#if (!defined(_MazePropertyDrawerSliderF32_hpp_))
#define _MazePropertyDrawerSliderF32_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-editor-tools/property-drawers/MazePropertyDrawer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PropertyDrawerSliderF32);
    MAZE_USING_SHARED_PTR(EditBox2D);
    MAZE_USING_SHARED_PTR(Slider2D);


    //////////////////////////////////////////
    // Class PropertyDrawerSliderF32
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API PropertyDrawerSliderF32
        : public GenericPropertyDrawer<F32>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PropertyDrawerSliderF32, PropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PropertyDrawerSliderF32);

    public:

        //////////////////////////////////////////
        virtual ~PropertyDrawerSliderF32();

        //////////////////////////////////////////
        static PropertyDrawerSliderF32Ptr Create(DataBlock const& _dataBlock);


        //////////////////////////////////////////
        virtual void setValue(F32 const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual F32 getValue() const MAZE_OVERRIDE;


        //////////////////////////////////////////
        inline F32 getFromValue() const { return m_fromValue; }

        //////////////////////////////////////////
        void setFromValue(F32 _value) { m_fromValue = _value; }

        //////////////////////////////////////////
        inline F32 getToValue() const { return m_toValue; }

        //////////////////////////////////////////
        void setToValue(F32 _value) { m_toValue = _value; }


        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label = nullptr) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        PropertyDrawerSliderF32();

        //////////////////////////////////////////
        virtual bool init(DataBlock const& _dataBlock) MAZE_OVERRIDE;
        

        //////////////////////////////////////////
        void notifyTextInput(EditBox2D* _editBox);

        //////////////////////////////////////////
        void notifySelectedChanged(EditBox2D* _editBox, bool _selected);

        //////////////////////////////////////////
        void notifySliderValueChanged(Slider2D* _slider, F32 _value);

        //////////////////////////////////////////
        virtual String prepareValueForEditBox(F32 _sliderValue);

    protected:
        Slider2DPtr m_slider;
        EditBox2DPtr m_editBox;

        F32 m_fromValue;
        F32 m_toValue;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazePropertyDrawerSliderF32_hpp_
//////////////////////////////////////////
