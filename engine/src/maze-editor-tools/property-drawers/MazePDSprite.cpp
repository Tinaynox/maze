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
#include "maze-editor-tools/property-drawers/MazePDSprite.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/ecs/components/MazeMeshRendererInstanced.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeDragAndDropZone.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-editor-tools/managers/MazeSpritePickerManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class PropertyDrawerSprite
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerSprite, PropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerSprite);

    //////////////////////////////////////////
    PropertyDrawerSprite::PropertyDrawerSprite()
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerSprite::~PropertyDrawerSprite()
    {
        if (m_selectAssetButton)
            m_selectAssetButton->eventClick.unsubscribe(this);
    }

    //////////////////////////////////////////
    PropertyDrawerSpritePtr PropertyDrawerSprite::Create(DataBlock const& _dataBlock)
    {
        PropertyDrawerSpritePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerSprite, object, init(_dataBlock));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerSprite::init(DataBlock const& _dataBlock)
    {
        if (!PropertyDrawer::init(_dataBlock))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerSprite::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

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


        HorizontalLayout2DPtr textureBlockLayout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(_parent->getWidth(), 18),
            Vec2F(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        textureBlockLayout->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);
        textureBlockLayout->setAutoWidth(true);
        textureBlockLayout->setExpand(false);

        m_selectAssetButton = UIHelper::CreateClickButton(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::SelectAsset),
            Vec2F(18, 18),
            Vec2F::c_zero,
            textureBlockLayout->getTransform(),
            _parent->getEntityRaw()->getEcsScene());
        m_selectAssetButton->setNormalColor(ColorU32{ 50, 50, 50 });
        m_selectAssetButton->setFocusedColor(ColorU32{ 100, 100, 100 });
        m_selectAssetButton->setSelectedColor(ColorU32{ 150, 150, 150 });
        m_selectAssetButton->setPressedColor(ColorU32{ 100, 150, 100 });
        m_selectAssetButton->eventClick.subscribe(this, &PropertyDrawerSprite::notifySelectAssetClick);

        SpriteRenderer2DPtr textureHolder = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel02),
            Vec2F(18, 18),
            Vec2F::c_zero,
            materialManager->getSpriteMaterial(),
            textureBlockLayout->getTransform(),
            _parent->getEntityRaw()->getEcsScene());
        textureHolder->setColor(ColorU32(50, 50, 50));

        m_textureRenderer = SpriteHelper::CreateSprite(
            m_sprite,
            Vec2F(16, 16),
            Vec2F::c_zero,
            materialManager->getSpriteMaterial(),
            textureHolder->getTransform(),
            _parent->getEntityRaw()->getEcsScene());

        m_dragAndDropFrame = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel02),
            m_textureRenderer->getTransform()->getSize(),
            Vec2F::c_zero,
            nullptr,
            m_textureRenderer->getTransform(),
            _parent->getEntityRaw()->getEcsScene());
        m_dragAndDropFrame->setColor(255, 200, 40);
        m_dragAndDropFrame->getEntityRaw()->ensureComponent<SizePolicy2D>();
        m_dragAndDropFrame->getMeshRenderer()->setEnabled(false);

        m_dragAndDropZone = m_textureRenderer->getEntityRaw()->ensureComponent<DragAndDropZone>();
        m_dragAndDropZone->getUIElement()->setCaptureCursorHits(true);
        m_dragAndDropZone->eventDragAndDropValidate.subscribe(
            [this](DataBlock const& _data, EntityId _viewEid, bool& _outDropAllowed)
            {
                if (_data.getHashedCString(MAZE_HCS("type")) == MAZE_HCS("assetFile"))
                {
                    AssetFileId afid = _data.getS32(MAZE_HCS("afid"));
                    AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(afid);
                    if (!assetFile)
                        return;

                    SpritePtr const& texture2d = SpriteManager::GetCurrentInstance()->getOrLoadSprite(assetFile);
                    if (!texture2d)
                        return;

                    _outDropAllowed = true;
                }
            });
        m_dragAndDropZone->eventDragAndDrop.subscribe(
            [this](DataBlock const& _data, EntityId _viewEid)
            {
                if (_data.getHashedCString(MAZE_HCS("type")) == MAZE_HCS("assetFile"))
                {
                    AssetFileId afid = _data.getS32(MAZE_HCS("afid"));
                    AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(afid);
                    if (!assetFile)
                        return;

                    SpritePtr const& texture2d = SpriteManager::GetCurrentInstance()->getOrLoadSprite(assetFile);
                    if (!texture2d)
                        return;

                    setValue(texture2d);
                    eventUIData();
                }
            });
        m_dragAndDropZone->eventDragAndDropZoneOnDragAndDropCurrentZoneChanged.subscribe(
            [this](bool _active)
            {
                this->m_dragAndDropFrame->getMeshRenderer()->setEnabled(_active);
            });

    }

    //////////////////////////////////////////
    void PropertyDrawerSprite::notifySelectAssetClick(Button2D* _button, CursorInputEvent& _event)
    {
        PropertyDrawerSpriteWPtr weakPtr = cast<PropertyDrawerSprite>();
        SpritePickerManager::GetInstancePtr()->openSpritePicker(
            [weakPtr](SpritePtr const& _sprite)
            {
                PropertyDrawerSpritePtr ptr = weakPtr.lock();
                if (ptr)
                {
                    ptr->setValue(_sprite);

                    if (!ptr->m_sprite)
                        ptr->m_sprite = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getSpriteManager()->getBuiltinSprite(
                            BuiltinSpriteType::White);

                    ptr->eventUIData();
                }
            },
            m_sprite);
    }

    //////////////////////////////////////////
    void PropertyDrawerSprite::setValue(SpritePtr const& _value)
    {
        if (m_sprite == _value)
            return;

        m_sprite = _value;

        if (m_textureRenderer)
            m_textureRenderer->setSprite(m_sprite);
    }

    //////////////////////////////////////////
    SpritePtr PropertyDrawerSprite::getValue() const
    {
        return m_sprite;
    }


    //////////////////////////////////////////
    // Class PropertyDrawerSpriteAssetRef
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerSpriteAssetRef, PropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerSpriteAssetRef);

    //////////////////////////////////////////
    PropertyDrawerSpriteAssetRef::PropertyDrawerSpriteAssetRef()
    {

    }

    //////////////////////////////////////////
    PropertyDrawerSpriteAssetRef::~PropertyDrawerSpriteAssetRef()
    {
        if (m_drawer)
            m_drawer->eventUIData.unsubscribe(this);

        m_drawer.reset();
    }

    //////////////////////////////////////////
    PropertyDrawerSpriteAssetRefPtr PropertyDrawerSpriteAssetRef::Create(DataBlock const& _dataBlock)
    {
        PropertyDrawerSpriteAssetRefPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerSpriteAssetRef, object, init(_dataBlock));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerSpriteAssetRef::init(DataBlock const& _dataBlock)
    {
        if (!PropertyDrawer::init(_dataBlock))
            return false;

        m_drawer = PropertyDrawerSprite::Create(_dataBlock);
        m_drawer->eventUIData.subscribe(this, &PropertyDrawerSpriteAssetRef::processDataFromUI);

        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerSpriteAssetRef::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        m_drawer->buildUI(_parent, _label);
    }

    //////////////////////////////////////////
    void PropertyDrawerSpriteAssetRef::setValue(SpriteAssetRef const& _value)
    {
        m_drawer->setValue(_value.getSprite());
    }

    //////////////////////////////////////////
    SpriteAssetRef PropertyDrawerSpriteAssetRef::getValue() const
    {
        return SpriteAssetRef(m_drawer->getValue());
    }

    //////////////////////////////////////////
    void PropertyDrawerSpriteAssetRef::processDataFromUI()
    {
        eventUIData();
    }

} // namespace Maze
//////////////////////////////////////////
