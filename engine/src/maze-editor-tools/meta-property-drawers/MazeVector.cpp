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
#include "maze-editor-tools/meta-property-drawers/MazeVector.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeSystemTextDropdown2D.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsLayout.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class MetaPropertyDrawerVector
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(MetaPropertyDrawerVector, MetaPropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MetaPropertyDrawerVector);

    //////////////////////////////////////////
    MetaPropertyDrawerVector::MetaPropertyDrawerVector()
    {
        
    }

    //////////////////////////////////////////
    MetaPropertyDrawerVector::~MetaPropertyDrawerVector()
    {
        if (m_expandButton)
            m_expandButton->eventClick.unsubscribe(this);

        if (m_vectorSizeDrawer)
        {
            m_vectorSizeDrawer->eventUIData.unsubscribe(this);
            m_vectorSizeDrawer.reset();
        }

        while (!m_itemDrawers.empty())
        {
            m_itemDrawers.back()->eventUIData.unsubscribe(this);
            m_itemDrawers.pop_back();
        }
    }

    //////////////////////////////////////////
    MetaPropertyDrawerVectorPtr MetaPropertyDrawerVector::Create(
        MetaProperty* _metaProperty)
    {
        MetaPropertyDrawerVectorPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MetaPropertyDrawerVector, object, init(_metaProperty));
        return object;
    }

    //////////////////////////////////////////
    bool MetaPropertyDrawerVector::init(
        MetaProperty* _metaProperty)
    {
        if (!MetaPropertyDrawer::init(_metaProperty))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerVector::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        VerticalLayout2DPtr layout = UIHelper::CreateVerticalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(_parent->getWidth(), 18),
            Vec2DF(0, 0),
            _parent,
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.0f, 1.0f));
        layout->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);

        HorizontalLayout2DPtr titleLayout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(_parent->getWidth(), 18),
            Vec2DF(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.0f, 1.0f));
        titleLayout->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);
        titleLayout->setAutoWidth(false);
        titleLayout->setSpacing(4.0f);
        titleLayout->setExpand(false);


        m_expandButtonSprite = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::DropDownButtonExpanded),
            Vec2DF(8, 8) * 1.75f,
            Vec2DF(10, 1),
            materialManager->getColorTextureMaterial(),
            titleLayout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 0.5f),
            Vec2DF(0.5f, 0.5f));
        m_expandButtonSprite->setColor(ColorU32::c_black);
        m_expandButton = m_expandButtonSprite->getEntityRaw()->ensureComponent<ClickButton2D>();
        m_expandButton->eventClick.subscribe(this, &MetaPropertyDrawerVector::notifyExpandButtonClick);

        SystemTextRenderer2DPtr systemText = SpriteHelper::CreateSystemText(
            EditorToolsHelper::BuildPropertyName(m_metaProperty->getName(), _label).c_str(),
            EditorToolsLayout::c_inspectorPropertyFontSize,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(8, 18),
            Vec2DF(0, 0),
            titleLayout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 0.5f),
            Vec2DF::c_zero);
        systemText->setColor(EditorToolsLayout::c_inspectorPropertyColor);


        m_bodyLayout = UIHelper::CreateVerticalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(_parent->getWidth() - 20, 18),
            Vec2DF(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.0f, 1.0f));
        m_bodyLayout->setPaddingLeft(20.0f);
        m_bodyLayout->setPaddingBottom(2.0f);
        m_bodyLayout->setAutoHeight(true);
        SizePolicy2DPtr bodyLayoutSizePolicy = m_bodyLayout->getEntityRaw()->ensureComponent<SizePolicy2D>();
        bodyLayoutSizePolicy->setFlag(SizePolicy2D::Height, false);
        bodyLayoutSizePolicy->setSizeDelta(-20, 0);

        m_vectorSizeDrawer = PropertyDrawerS32::Create("Size");
        m_vectorSizeDrawer->buildUI(m_bodyLayout->getTransform());
        m_vectorSizeDrawer->eventUIData.subscribe(this, &MetaPropertyDrawerVector::processDataFromUI);

        m_itemsLayout = UIHelper::CreateVerticalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(m_bodyLayout->getTransform()->getWidth(), 18),
            Vec2DF(0, 0),
            m_bodyLayout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.0f, 1.0f));
        m_itemsLayout->setSpacing(2.0f);
        m_itemsLayout->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);
        
    }

    //////////////////////////////////////////
    bool MetaPropertyDrawerVector::fetchVectorSizeValue(
        Size& _value,
        bool& _isMultiValue)
    {
        _isMultiValue = false;

        if (!m_metaInstances.empty())
        {
            _value = m_metaProperty->getVectorSize(*m_metaInstances.begin());
            for (Set<MetaInstance>::const_iterator    it = ++m_metaInstances.begin(),
                end = m_metaInstances.end();
                it != end;
                ++it)
            {
                Size value = m_metaProperty->getVectorSize(*it);
                if (value != _value)
                {
                    _isMultiValue = true;
                    return true;
                }
            }

            return true;
        }

        return false;
    }

    //////////////////////////////////////////
    bool MetaPropertyDrawerVector::fetchVectorElementClassUID(
        ClassUID& _value,
        bool& _isMultiValue)
    {
        _isMultiValue = false;

        if (!m_metaInstances.empty())
        {
            _value = m_metaProperty->getVectorElementClassUID(*m_metaInstances.begin());
            for (Set<MetaInstance>::const_iterator    it = ++m_metaInstances.begin(),
                end = m_metaInstances.end();
                it != end;
                ++it)
            {
                Size value = m_metaProperty->getVectorElementClassUID(*it);
                if (value != _value)
                {
                    _isMultiValue = true;
                    return true;
                }
            }

            return true;
        }

        return false;
    }

    //////////////////////////////////////////
    bool MetaPropertyDrawerVector::fetchVectorElementValue(
        Size _index,
        String& _value,
        bool& _isMultiValue)
    {
        _isMultiValue = false;

        if (!m_metaInstances.empty())
        {
            
            String serializedList = m_metaProperty->toString(*m_metaInstances.begin());
            Vector<String> elements;
            ValueFromString(elements, serializedList.c_str(), serializedList.size());
            _value = _index < elements.size() ? elements[_index] : "";

            for (Set<MetaInstance>::const_iterator    it = ++m_metaInstances.begin(),
                end = m_metaInstances.end();
                it != end;
                ++it)
            {
                String serializedList = m_metaProperty->toString(*it);
                ValueFromString(elements, serializedList.c_str(), serializedList.size());
                String value = _index < elements.size() ? elements[_index] : "";

                if (value != _value)
                {
                    _isMultiValue = true;
                    return true;
                }
            }

            return true;
        }

        return false;
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerVector::ensureItemDrawers(Size _count)
    {
        Size itemDrawersCount = m_itemDrawers.size();

        if (itemDrawersCount == _count)
            return;


        if (itemDrawersCount > _count)
        {
            while (itemDrawersCount > _count)
            {
                m_itemDrawers.back()->eventUIData.unsubscribe(this);
                m_itemDrawers.pop_back();

                --itemDrawersCount;
            }
        }
        else
        {
            ClassUID vectorItemClassUID = 0;
            bool isMultiValue = false;
            fetchVectorElementClassUID(vectorItemClassUID, isMultiValue);

            while (itemDrawersCount < _count)
            {
                PropertyDrawerPtr itemDrawer = InspectorManager::GetInstancePtr()->createPropertyDrawer(
                    vectorItemClassUID,
                    "Element" + StringHelper::ToString((U32)m_itemDrawers.size()));

                if (itemDrawer)
                {
                    itemDrawer->eventUIData.subscribe(this, &MetaPropertyDrawerVector::processDataFromUI);
                    m_itemDrawers.push_back(itemDrawer);
                }

                ++itemDrawersCount;
            }
        }

        m_itemsLayout->getTransform()->destroyAllChildren();

        if (m_itemDrawers.size() == _count)
        {
            for (PropertyDrawerPtr const& itemDrawer : m_itemDrawers)
            {
                itemDrawer->buildUI(m_itemsLayout->getTransform());
            }
        }
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerVector::notifyExpandButtonClick(Button2D* _button, CursorInputEvent const& _inputEvent)
    {
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerVector::processDataToUI()
    {
        m_processingDataToUI = true;
        {
            {
                Size vectorSize = 0;
                bool isMultiValue;
                fetchVectorSizeValue(vectorSize, isMultiValue);
                m_vectorSizeDrawer->setValue((S32)vectorSize);

                ensureItemDrawers(vectorSize);
            }

            {
                for (Size i = 0, in = m_itemDrawers.size(); i < in; ++i)
                {
                    String value;
                    bool isMultiValue = false;
                    fetchVectorElementValue(i, value, isMultiValue);

                    m_itemDrawers[i]->setString(value);
                }

            }
        }
        m_processingDataToUI = false;
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerVector::processDataFromUI()
    {
        if (m_processingDataToUI)
            return;

        Size vectorSize = (Size)Math::Max(m_vectorSizeDrawer->getValue(), 0);

        for (MetaInstance const& metaInstance : m_metaInstances)
            m_metaProperty->setVectorSize(metaInstance, vectorSize);

        ensureItemDrawers(vectorSize);

        Vector<String> values;

        for (Size i = 0, in = m_itemDrawers.size(); i < in; ++i)
            values.push_back(m_itemDrawers[i]->getString());

        String value;
        ValueToString(values, value);
        for (MetaInstance const& metaInstance : m_metaInstances)
            m_metaProperty->setString(metaInstance, value);
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerVector::notifyValueChanged(SystemTextDropdown2D* _dropdown, S32 _value)
    {
        processDataFromUI();
    }

} // namespace Maze
//////////////////////////////////////////
