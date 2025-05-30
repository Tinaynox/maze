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
#include "maze-editor-tools/property-drawers/MazePDAssetFileId.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-ui/ecs/components/MazeTextRenderer2D.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/ecs/components/MazeMeshRendererInstanced.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeDragAndDropZone.hpp"
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
    // Class PropertyDrawerAssetFileId
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerAssetFileId, PropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerAssetFileId);

    //////////////////////////////////////////
    PropertyDrawerAssetFileId::PropertyDrawerAssetFileId()
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerAssetFileId::~PropertyDrawerAssetFileId()
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerAssetFileIdPtr PropertyDrawerAssetFileId::Create(DataBlock const& _dataBlock)
    {
        PropertyDrawerAssetFileIdPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerAssetFileId, object, init(_dataBlock));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerAssetFileId::init(DataBlock const& _dataBlock)
    {
        if (!PropertyDrawer::init(_dataBlock))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerAssetFileId::buildUI(
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


        m_panelRenderer = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
            Vec2F(180, 18),
            Vec2F(0, 0),
            nullptr,
            layout->getTransform(),
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.5f, 0.5f),
            Vec2F::c_zero);

        m_dragAndDropFrame = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Focused),
            m_panelRenderer->getTransform()->getSize(),
            Vec2F::c_zero,
            nullptr,
            m_panelRenderer->getTransform(),
            _parent->getEntityRaw()->getEcsScene());
        m_dragAndDropFrame->setColor(255, 200, 40);
        m_dragAndDropFrame->getEntityRaw()->ensureComponent<SizePolicy2D>();
        m_dragAndDropFrame->getMeshRenderer()->setEnabled(false);

        m_dragAndDropZone = m_panelRenderer->getEntityRaw()->ensureComponent<DragAndDropZone>();
        m_dragAndDropZone->eventDragAndDropValidate.subscribe(
            [this](DataBlock const& _data, EntityId _viewEid, bool& _outDropAllowed)
            {
                if (_data.getHashedCString(MAZE_HCS("type")) == MAZE_HCS("assetUnit"))
                {
                    _outDropAllowed = true;
                }
                else
                if (_data.getHashedCString(MAZE_HCS("type")) == MAZE_HCS("assetFile"))
                {
                    AssetFileId assetFileId = _data.getS32(MAZE_HCS("afid"), c_invalidAssetFileId);

                    if (assetFileId != c_invalidAssetFileId)
                    {
                        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(assetFileId);
                        if (assetFile)
                        {
                            _outDropAllowed = true;
                        }
                    }
                }
            });
        m_dragAndDropZone->eventDragAndDrop.subscribe(
            [this](DataBlock const& _data, EntityId _viewEid)
            {
                if (_data.getHashedCString(MAZE_HCS("type")) == MAZE_HCS("assetUnit"))
                {
                    AssetUnitPtr const& assetUnit = AssetUnitManager::GetInstancePtr()->getAssetUnit(
                        _data.getU32(MAZE_HCS("auid"), c_invalidAssetFileId));
                    if (assetUnit)
                    {
                        AssetFilePtr const& assetFile = assetUnit->getAssetFile();
                        if (assetFile)
                        {
                            setValue(assetFile->getAssetFileId());
                            eventUIData();
                        }
                    }
                }
                else
                if (_data.getHashedCString(MAZE_HCS("type")) == MAZE_HCS("assetFile"))
                {
                    AssetFileId assetFileId = _data.getS32(MAZE_HCS("afid"), c_invalidAssetFileId);

                    if (assetFileId != c_invalidAssetFileId)
                    {
                        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(assetFileId);
                        if (assetFile)
                        {
                            setValue(assetFileId);
                            eventUIData();
                        }
                    }
                }
            });
        m_dragAndDropZone->eventDragAndDropZoneOnDragAndDropCurrentZoneChanged.subscribe(
            [this](bool _active)
            {
                this->m_dragAndDropFrame->getMeshRenderer()->setEnabled(_active);
            });

        m_text = EditorToolsUIHelper::CreateText(
            "None",
            EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
            EditorToolsStyles::GetInstancePtr()->getInspectorPropertyFontSize(),
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(0, 18),
            Vec2F(3, 0),
            m_panelRenderer->getTransform(),
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F(0.0f, 0.5f));
        m_text->setColor(ColorU32::c_black);

        SizePolicy2DPtr textSizePolicy = m_text->getEntityRaw()->ensureComponent<SizePolicy2D>();
        textSizePolicy->setSizeDelta(-10.0f, 0.0f);
    }

    ////////////////////////////////////////////
    void PropertyDrawerAssetFileId::unselectUI()
    {
        
    }

    //////////////////////////////////////////
    void PropertyDrawerAssetFileId::setValue(AssetFileId const& _value)
    {
        if (m_afid == _value)
            return;

        m_afid = _value;

        if (AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(m_afid))
        {
            m_text->setTextFormatted("%s [%u]", assetFile->getFileName().toUTF8().c_str(), (U32)m_afid);
        }
        else
        {
            CString name = _value ? "Invalid" : "None";
            m_text->setTextFormatted("%s [%u]", name, (U32)m_afid);
        }
    }

    //////////////////////////////////////////
    AssetFileId PropertyDrawerAssetFileId::getValue() const
    {
        return m_afid;
    }


} // namespace Maze
//////////////////////////////////////////
