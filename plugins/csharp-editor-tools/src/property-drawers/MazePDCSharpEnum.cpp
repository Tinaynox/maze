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
#include "MazeCSharpEditorToolsHeader.hpp"
#include "maze-plugin-csharp-editor-tools/property-drawers/MazePDCSharpEnum.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/ecs/components/MazeMeshRendererInstanced.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeDragAndDropZone.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class PropertyDrawerCSharpEnum
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerCSharpEnum, PropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerCSharpEnum);

    //////////////////////////////////////////
    PropertyDrawerCSharpEnum::PropertyDrawerCSharpEnum()
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerCSharpEnum::~PropertyDrawerCSharpEnum()
    {
        if (m_dropdown)
        {
            m_dropdown->eventValueChanged.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    PropertyDrawerCSharpEnumPtr PropertyDrawerCSharpEnum::Create(
        MonoType* _monoType,
        DataBlock const& _dataBlock)
    {
        PropertyDrawerCSharpEnumPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerCSharpEnum, object, init(_monoType, _dataBlock));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerCSharpEnum::init(
        MonoType* _monoType,
        DataBlock const& _dataBlock)
    {
        if (!PropertyDrawer::init(_dataBlock))
            return false;

        MonoClass* baseClass = mono_class_from_mono_type(_monoType);
        void* iter = nullptr;
        while (MonoClassField* field = mono_class_get_fields(baseClass, &iter))
        {
            CString fieldName = mono_field_get_name(field);
            if (!fieldName)
                continue;

            int flags = mono_field_get_flags(field);
            if (!(flags & MONO_FIELD_ATTR_STATIC))
                continue;

            S32 value = 0;
            mono_field_static_get_value(mono_class_vtable(mono_domain_get(), baseClass), field, &value);

            m_enumNames.emplace_back(fieldName);
            m_enumValues.emplace_back(value);
        }

        return true;
    }

    ////////////////////////////////////////////
    S32 PropertyDrawerCSharpEnum::getValue() const
    {
        S32 index = m_dropdown->getValue();
        if (index < 0 || index >= (S32)m_enumValues.size())
            return 0;

        return m_enumValues[index];
    }

    ////////////////////////////////////////////
    void PropertyDrawerCSharpEnum::setValue(S32 _value)
    {
        for (S32 i = 0, in = (S32)m_enumValues.size(); i != in; ++i)
        {
            if (m_enumValues[i] == _value)
            {
                m_dropdown->setValue(i);
                return;
            }
        }
    }

    ////////////////////////////////////////////
    bool PropertyDrawerCSharpEnum::toDataBlock(DataBlock& _value) const
    {
        ValueToDataBlock(getValue(), _value);
        return true;
    }

    ////////////////////////////////////////////
    bool PropertyDrawerCSharpEnum::setDataBlock(DataBlock const& _value)
    {
        S32 enumValue = 0;
        ValueFromDataBlock(enumValue, _value);
        setValue(enumValue);
        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerCSharpEnum::buildUI(
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


        SpriteRenderer2DPtr spriteRenderer = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
            Vec2F(180, 18),
            Vec2F(0, 0),
            renderSystem->getMaterialManager()->getSpriteMaterial(),
            layout->getTransform(),
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.5f, 0.5f),
            Vec2F::c_zero);
        spriteRenderer->setRenderMode(SpriteRenderMode::Sliced);
        spriteRenderer->setColor(ColorU32::c_gray);


        m_dropdown = EditorToolsUIHelper::CreateDefaultDropdown(
            12,
            Vec2F(180, 18),
            Vec2F(0, 0),
            spriteRenderer->getTransform(),
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F(0.0f, 0.5f));
        m_dropdown->addOptions(m_enumNames);
        m_dropdown->eventValueChanged.subscribe(this, &PropertyDrawerCSharpEnum::notifyValueChanged);

    }

    //////////////////////////////////////////
    void PropertyDrawerCSharpEnum::notifyValueChanged(Dropdown2D* _dropdown, S32 _value)
    {
        eventUIData();
    }


} // namespace Maze
//////////////////////////////////////////
