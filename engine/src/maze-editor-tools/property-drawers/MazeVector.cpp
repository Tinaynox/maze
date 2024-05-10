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
#include "maze-editor-tools/property-drawers/MazeVector.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"



//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class PropertyDrawerVector
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerVector, PropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerVector);

    //////////////////////////////////////////
    PropertyDrawerVector::PropertyDrawerVector()
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerVector::~PropertyDrawerVector()
    {
        ensureItemDrawers(0);

        if (m_expandButton)
            m_expandButton->eventClick.unsubscribe(this);
    }

    //////////////////////////////////////////
    PropertyDrawerVectorPtr PropertyDrawerVector::Create(
        ClassUID _childPropertyClassUID,
        String const& _label)
    {
        PropertyDrawerVectorPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerVector, object, init(_childPropertyClassUID, _label));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerVector::init(
        ClassUID _childPropertyClassUID,
        String const& _label)
    {
        if (!PropertyDrawer::init(_label))
            return false;

        m_childPropertyClassUID = _childPropertyClassUID;

        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerVector::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        VerticalLayout2DPtr layout = UIHelper::CreateVerticalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(_parent->getWidth(), 18),
            Vec2F(0, 0),
            _parent,
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        layout->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);

        HorizontalLayout2DPtr titleLayout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(_parent->getWidth(), 18),
            Vec2F(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        titleLayout->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);
        titleLayout->setAutoWidth(false);
        titleLayout->setSpacing(4.0f);
        titleLayout->setExpand(false);


        m_expandButtonSprite = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::DropDownButtonExpanded),
            Vec2F(8, 8) * 1.75f,
            Vec2F(10, 1),
            materialManager->getColorTextureMaterial(),
            titleLayout->getTransform(),
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F(0.5f, 0.5f));
        m_expandButtonSprite->setColor(ColorU32::c_black);
        m_expandButton = m_expandButtonSprite->getEntityRaw()->ensureComponent<ClickButton2D>();
        m_expandButton->eventClick.subscribe(this, &PropertyDrawerVector::notifyExpandButtonClick);

        AbstractTextRenderer2DPtr titleText = EditorToolsUIHelper::CreateText(
            _label,
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


        m_bodyLayout = UIHelper::CreateVerticalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(_parent->getWidth() - 20, 18),
            Vec2F(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        m_bodyLayout->setPaddingLeft(20.0f);
        m_bodyLayout->setPaddingBottom(2.0f);
        m_bodyLayout->setAutoHeight(true);
        SizePolicy2DPtr bodyLayoutSizePolicy = m_bodyLayout->getEntityRaw()->ensureComponent<SizePolicy2D>();
        bodyLayoutSizePolicy->setFlag(SizePolicy2D::Height, false);
        bodyLayoutSizePolicy->setSizeDelta(-20, 0);

        m_vectorSizeDrawer = PropertyDrawerS32::Create("Size");
        m_vectorSizeDrawer->buildUI(m_bodyLayout->getTransform());
        m_vectorSizeDrawer->eventUIData.subscribe(this, &PropertyDrawerVector::notifyVectorSizeChanged);

        m_itemsLayout = UIHelper::CreateVerticalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(m_bodyLayout->getTransform()->getWidth(), 18),
            Vec2F(0, 0),
            m_bodyLayout->getTransform(),
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        m_itemsLayout->setSpacing(2.0f);
        m_itemsLayout->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);
    }

    ////////////////////////////////////////////
    bool PropertyDrawerVector::toDataBlock(DataBlock& _dataBlock) const
    {
        S32 vectorSize = (S32)m_vectorSizeDrawer->getValue();

        S32 itemDrawersCount = (S32)m_itemDrawers.size();

        _dataBlock.clear();

        for (S32 i = 0; i < vectorSize; ++i)
        {
            if (i < itemDrawersCount)
            {
                DataBlock* childDataBlock = _dataBlock.addNewDataBlock(MAZE_HCS("item"));
                m_itemDrawers[i]->toDataBlock(*childDataBlock);
            }
            else
            {
                _dataBlock.addNewDataBlock(MAZE_HCS("item"));
            }
        }

        return true;
    }

    ////////////////////////////////////////////
    bool PropertyDrawerVector::setDataBlock(DataBlock const& _dataBlock)
    {
        S32 vectorSize = (S32)_dataBlock.getDataBlocksCount();

        ensureItemDrawers(vectorSize);

        m_vectorSizeDrawer->setValue((S32)vectorSize);

        for (Size i = 0, in = m_itemDrawers.size(); i < in; ++i)
        {
            DataBlock const* childDataBlock = _dataBlock.getDataBlock(DataBlock::DataBlockIndex(i));
            PropertyDrawerPtr const& propertyDrawer = m_itemDrawers[i];
            propertyDrawer->setDataBlock(*childDataBlock);
        }

        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerVector::notifyExpandButtonClick(Button2D* _button, CursorInputEvent const& _inputEvent)
    {
    }

    //////////////////////////////////////////
    void PropertyDrawerVector::notifyVectorSizeChanged()
    {
        eventUIData();

        ensureItemDrawers(m_vectorSizeDrawer->getValue());
    }

    //////////////////////////////////////////
    void PropertyDrawerVector::ensureItemDrawers(Size _count)
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
            while (itemDrawersCount < _count)
            {
                PropertyDrawerPtr itemDrawer = InspectorManager::GetInstancePtr()->createPropertyDrawer(
                    m_childPropertyClassUID,
                    "Element" + StringHelper::ToString((U32)m_itemDrawers.size()));

                if (itemDrawer)
                {
                    itemDrawer->eventUIData.subscribe(this, &PropertyDrawerVector::processItemPropertyUIData);
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

        m_itemsLayout->alignChildren();
    }

    //////////////////////////////////////////
    void PropertyDrawerVector::processItemPropertyUIData()
    {
        eventUIData();
    }

} // namespace Maze
//////////////////////////////////////////
