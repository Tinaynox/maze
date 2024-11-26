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
#if (!defined(_MazePropertyDrawerAnimationCurve_hpp_))
#define _MazePropertyDrawerAnimationCurve_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-core/math/MazeAnimationCurve.hpp"
#include "maze-editor-tools/property-drawers/MazePropertyDrawer.hpp"
#include "maze-editor-tools/ecs/components/MazeAnimationCurveEdit2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PropertyDrawerAnimationCurve);
    MAZE_USING_SHARED_PTR(EditBox2D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);


    //////////////////////////////////////////
    // Class PropertyDrawerAnimationCurve
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API PropertyDrawerAnimationCurve
        : public GenericPropertyDrawer<AnimationCurve>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PropertyDrawerAnimationCurve, PropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PropertyDrawerAnimationCurve);

    public:

        //////////////////////////////////////////
        virtual ~PropertyDrawerAnimationCurve();

        //////////////////////////////////////////
        static PropertyDrawerAnimationCurvePtr Create(DataBlock const& _dataBlock);


        //////////////////////////////////////////
        virtual void setValue(AnimationCurve const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual AnimationCurve getValue() const MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label = nullptr) MAZE_OVERRIDE;


        //////////////////////////////////////////
        void normalize();

    protected:

        //////////////////////////////////////////
        PropertyDrawerAnimationCurve();

        //////////////////////////////////////////
        virtual bool init(DataBlock const& _dataBlock) MAZE_OVERRIDE;

        

        //////////////////////////////////////////
        void notifyCurveChanged(AnimationCurveEdit2D* _colorEdit, AnimationCurve const& _color);

    protected:
        AnimationCurveEdit2DPtr m_curveEdit;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazePropertyDrawerAnimationCurve_hpp_
//////////////////////////////////////////
