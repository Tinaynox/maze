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
#include "maze-editor-tools/property-drawers/MazeEnumClass.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeDropdown2D.hpp"
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
    // Class PropertyDrawerEnumClass
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerEnumClass, PropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerEnumClass);

    //////////////////////////////////////////
    PropertyDrawerEnumClass::PropertyDrawerEnumClass()
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerEnumClass::~PropertyDrawerEnumClass()
    {
        m_dropdown->eventValueChanged.unsubscribe(this);
    }

    //////////////////////////////////////////
    PropertyDrawerEnumClassPtr PropertyDrawerEnumClass::Create(
        String const& _label,
        Vector<String> const& _enumValues)
    {
        PropertyDrawerEnumClassPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerEnumClass, object, init(_label, _enumValues));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerEnumClass::init(
        String const& _label,
        Vector<String> const& _enumValues)
    {
        if (!PropertyDrawer::init(_label))
            return false;

        m_enumValues = _enumValues;

        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerEnumClass::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

        HorizontalLayout2DPtr layout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(_parent->getWidth(), 18),
            Vec2F(0, 0),
            _parent,
            _parent->getEntityRaw()->getECSScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        layout->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);
        layout->setAutoWidth(false);
        layout->setExpand(true);

        AbstractTextRenderer2DPtr titleText = EditorToolsUIHelper::CreateText(
            EditorToolsHelper::BuildPropertyName(m_label.c_str(), _label).c_str(),
            EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
            EditorToolsStyles::GetInstancePtr()->getInspectorPropertyFontSize(),
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(8, 18),
            Vec2F(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F::c_zero);
        titleText->setColor(EditorToolsStyles::GetInstancePtr()->getInspectorPropertyColor());


        SpriteRenderer2DPtr spriteRenderer = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
            Vec2F(180, 18),
            Vec2F(0, 0),
            renderSystem->getMaterialManager()->getColorTextureMaterial(),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2F(0.5f, 0.5f),
            Vec2F::c_zero);
        spriteRenderer->setRenderMode(SpriteRenderMode::Sliced);
        spriteRenderer->setColor(ColorU32::c_gray);


        m_dropdown = UIHelper::CreateDefaultDropdown(
            12,
            Vec2F(180, 18),
            Vec2F(0, 0),
            spriteRenderer->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F(0.0f, 0.5f));
        m_dropdown->addOptions(m_enumValues);
        m_dropdown->eventValueChanged.subscribe(this, &PropertyDrawerEnumClass::notifyValueChanged);

    }

    //////////////////////////////////////////
    void PropertyDrawerEnumClass::setString(String const& _value)
    {
        setValue(_value);
    }

    //////////////////////////////////////////
    String PropertyDrawerEnumClass::getString()
    {
        return getValue();
    }

    //////////////////////////////////////////
    void PropertyDrawerEnumClass::setValue(String const& _value)
    {
        m_dropdown->setValue(_value);
    }

    //////////////////////////////////////////
    String PropertyDrawerEnumClass::getValue() const
    {
        String const& value = m_dropdown->getValueString();
        return value;
    }

    //////////////////////////////////////////
    void PropertyDrawerEnumClass::notifyValueChanged(Dropdown2D* _dropdown, S32 _value)
    {
        eventUIData();
    }

} // namespace Maze
//////////////////////////////////////////
