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
#include "MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/property-drawers/MazePDAnimationCurve.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"


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
    PropertyDrawerAnimationCurvePtr PropertyDrawerAnimationCurve::Create(DataBlock const& _dataBlock)
    {
        PropertyDrawerAnimationCurvePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerAnimationCurve, object, init(_dataBlock));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerAnimationCurve::init(DataBlock const& _dataBlock)
    {
        if (!PropertyDrawer::init(_dataBlock))
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
            Vec2F(_parent->getWidth(), 18),
            Vec2F(0, 0),
            _parent,
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        layout->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);
        layout->setAutoWidth(false);
        layout->setExpand(true);

        AbstractTextRenderer2DPtr titleText = EditorToolsUIHelper::CreateText(
            EditorToolsHelper::BuildPropertyName(m_dataBlock.getCString(MAZE_HCS("label")), _label).c_str(),
            EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
            EditorToolsStyles::GetInstancePtr()->getInspectorPropertyFontSize(),
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(8, 18),
            Vec2F(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F::c_zero);
        titleText->setColor(EditorToolsStyles::GetInstancePtr()->getInspectorPropertyColor());

        m_curveEdit = EditorToolsUIHelper::CreateDefaultAnimationCurveEdit(
            AnimationCurve(
                0.0f,
                0.0f,
                1.0f,
                1.0f),
            Vec2F(60, 18),
            Vec2F(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.5f, 0.5f),
            Vec2F::c_zero);
        m_curveEdit->eventCurveChanged.subscribe(this, &PropertyDrawerAnimationCurve::notifyCurveChanged);
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
