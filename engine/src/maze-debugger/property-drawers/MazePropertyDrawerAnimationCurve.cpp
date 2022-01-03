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
#include "MazeDebuggerHeader.hpp"
#include "maze-debugger/property-drawers/MazePropertyDrawerAnimationCurve.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-debugger/layout/MazeDebuggerLayout.hpp"
#include "maze-debugger/helpers/MazeDebuggerHelper.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class PropertyDrawerAnimationCurve
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerAnimationCurve, PropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerAnimationCurve);

    //////////////////////////////////////////
    PropertyDrawerAnimationCurve::PropertyDrawerAnimationCurve()
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerAnimationCurve::~PropertyDrawerAnimationCurve()
    {
        if (m_curveEdit)
            m_curveEdit->eventCurveChanged.unsubscribe(this);
    }

    //////////////////////////////////////////
    PropertyDrawerAnimationCurvePtr PropertyDrawerAnimationCurve::Create(String const& _label)
    {
        PropertyDrawerAnimationCurvePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerAnimationCurve, object, init(_label));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerAnimationCurve::init(String const& _label)
    {
        if (!PropertyDrawer::init(_label))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerAnimationCurve::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        HorizontalLayout2DPtr layout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(_parent->getWidth(), 18),
            Vec2DF(0, 0),
            _parent,
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.0f, 1.0f));
        layout->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);
        layout->setAutoWidth(false);
        layout->setExpand(true);

        SystemTextRenderer2DPtr systemText = SpriteHelper::CreateSystemText(
            DebuggerHelper::BuildPropertyName(m_label.c_str(), _label).c_str(),
            DebuggerLayout::c_inspectorPropertyFontSize,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(8, 18),
            Vec2DF(0, 0),            
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 0.5f),
            Vec2DF::c_zero);
        systemText->setColor(DebuggerLayout::c_inspectorPropertyColor);

        m_curveEdit = UIHelper::CreateDefaultAnimationCurveEdit(
            AnimationCurve(
                0.0f,
                0.0f,
                1.0f,
                1.0f),
            Vec2DF(60, 18),
            Vec2DF(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.5f, 0.5f),
            Vec2DF::c_zero);
        m_curveEdit->eventCurveChanged.subscribe(this, &PropertyDrawerAnimationCurve::notifyCurveChanged);
    }

    //////////////////////////////////////////
    void PropertyDrawerAnimationCurve::setString(String const& _value)
    {
        AnimationCurve value;
        ValueFromString(value, _value.c_str(), _value.size());
        setValue(value);
    }

    //////////////////////////////////////////
    String PropertyDrawerAnimationCurve::getString()
    {
        String value;
        ValueToString(getValue(), value);
        return value;
    }

    //////////////////////////////////////////
    void PropertyDrawerAnimationCurve::setValue(AnimationCurve const& _value)
    {
        m_curveEdit->setCurve(_value);
    }

    //////////////////////////////////////////
    AnimationCurve PropertyDrawerAnimationCurve::getValue() const
    {
        AnimationCurve gradient = m_curveEdit->getCurve();
        return gradient;
    }

    //////////////////////////////////////////
    void PropertyDrawerAnimationCurve::notifyCurveChanged(AnimationCurveEdit2D* _colorEdit, AnimationCurve const& _gradient)
    {
        eventUIData();
    }

    //////////////////////////////////////////
    void PropertyDrawerAnimationCurve::normalize()
    {
        m_curveEdit->normalize();
    }

} // namespace Maze
//////////////////////////////////////////
