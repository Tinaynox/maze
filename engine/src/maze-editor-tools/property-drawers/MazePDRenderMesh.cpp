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
#include "maze-editor-tools/property-drawers/MazePDRenderMesh.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/ecs/components/MazeMeshRendererInstanced.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeDragAndDropZone.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-editor-tools/managers/MazeRenderMeshPickerManager.hpp"
#include "maze-editor-tools/render-mesh-picker/MazeSceneRenderMeshPicker.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class PropertyDrawerRenderMesh
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerRenderMesh, PropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerRenderMesh);

    //////////////////////////////////////////
    PropertyDrawerRenderMesh::PropertyDrawerRenderMesh()
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerRenderMesh::~PropertyDrawerRenderMesh()
    {
        if (m_renderMeshButton)
            m_renderMeshButton->eventClick.unsubscribe(this);

        if (m_selectAssetButton)
            m_selectAssetButton->eventClick.unsubscribe(this);
    }

    //////////////////////////////////////////
    PropertyDrawerRenderMeshPtr PropertyDrawerRenderMesh::Create(DataBlock const& _dataBlock)
    {
        PropertyDrawerRenderMeshPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerRenderMesh, object, init(_dataBlock));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerRenderMesh::init(DataBlock const& _dataBlock)
    {
        if (!PropertyDrawer::init(_dataBlock))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerRenderMesh::buildUI(
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


        m_renderMeshButton = UIHelper::CreateClickButton(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Focused),
            Vec2F(180, 18),
            Vec2F::c_zero,
            layout->getTransform(),
            _parent->getEntityRaw()->getEcsScene());
        m_renderMeshButton->eventClick.subscribe(this, &PropertyDrawerRenderMesh::notifyRenderMeshButtonClick);


        m_dragAndDropFrame = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Focused),
            m_renderMeshButton->getTransform()->getSize(),
            Vec2F::c_zero,
            nullptr,
            m_renderMeshButton->getTransform(),
            _parent->getEntityRaw()->getEcsScene());
        m_dragAndDropFrame->setColor(255, 200, 40);
        m_dragAndDropFrame->getEntityRaw()->ensureComponent<SizePolicy2D>();
        m_dragAndDropFrame->getMeshRenderer()->setEnabled(false);

        m_dragAndDropZone = m_renderMeshButton->getEntityRaw()->ensureComponent<DragAndDropZone>();
        m_dragAndDropZone->eventDragAndDropValidate.subscribe(
            [this](DataBlock const& _data, EntityId _viewEid, bool& _outDropAllowed)
            {
                if (_data.getHashedCString(MAZE_HCS("type")) == MAZE_HCS("assetFile"))
                {
                    AssetFileId afid = _data.getS32(MAZE_HCS("afid"));
                    AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(afid);
                    if (!assetFile)
                        return;

                    RenderMeshPtr const& renderMesh = RenderMeshManager::GetCurrentInstancePtr()->getOrLoadRenderMesh(assetFile);
                    if (!renderMesh)
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

                    RenderMeshPtr const& renderMesh = RenderMeshManager::GetCurrentInstancePtr()->getOrLoadRenderMesh(assetFile);
                    if (!renderMesh)
                        return;

                    setValue(renderMesh);
                    eventUIData();
                }
            });
        m_dragAndDropZone->eventDragAndDropZoneOnDragAndDropCurrentZoneChanged.subscribe(
            [this](bool _active)
            {
                this->m_dragAndDropFrame->getMeshRenderer()->setEnabled(_active);
            });


        m_selectAssetButton = UIHelper::CreateClickButton(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::SelectAsset),
            Vec2F(18, 18),
            Vec2F(-9.0f, 0.0f),
            m_renderMeshButton->getTransform(),
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(1.0f, 0.5f),
            Vec2F(0.5f, 0.5f));
        m_selectAssetButton->setNormalColor(ColorU32{ 50, 50, 50 });
        m_selectAssetButton->setFocusedColor(ColorU32{ 100, 100, 100 });
        m_selectAssetButton->setSelectedColor(ColorU32{ 150, 150, 150 });
        m_selectAssetButton->setPressedColor(ColorU32{ 100, 150, 100 });
        m_selectAssetButton->eventClick.subscribe(this, &PropertyDrawerRenderMesh::notifySelectAssetClick);

        m_renderMeshIcon = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Mesh),
            Vec2F(12, 12),
            Vec2F(9.0f, 0.0f),
            materialManager->getSpriteMaterial(),
            m_renderMeshButton->getTransform(),
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F(0.5f, 0.5f));

        m_renderMeshNameDrawer = EditorToolsUIHelper::CreateText(
            "",
            EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
            12,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(160.0f, 0.0f),
            Vec2F(18.0f, 0.0f),
            m_renderMeshButton->getTransform(),
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F(0.0f, 0.5f));
        m_renderMeshNameDrawer->setColor(ColorU32::c_black);
    }

    //////////////////////////////////////////
    void PropertyDrawerRenderMesh::setValue(RenderMeshPtr const& _value)
    {
        m_renderMesh = _value;

        m_renderMeshIcon->getMeshRenderer()->setEnabled(m_renderMesh != nullptr);

        if (m_renderMesh)
            m_renderMeshNameDrawer->setText(EditorToolsHelper::GetNameWithoutExtension(m_renderMesh->getName()));
        else
            m_renderMeshNameDrawer->setText("None");
    }

    //////////////////////////////////////////
    RenderMeshPtr PropertyDrawerRenderMesh::getValue() const
    {
        return m_renderMesh;
    }

    //////////////////////////////////////////
    void PropertyDrawerRenderMesh::notifyRenderMeshButtonClick(Button2D* _button, CursorInputEvent& _event)
    {

    }

    //////////////////////////////////////////
    void PropertyDrawerRenderMesh::notifySelectAssetClick(Button2D* _button, CursorInputEvent& _event)
    {
        PropertyDrawerRenderMeshWPtr weakPtr = cast<PropertyDrawerRenderMesh>();
        RenderMeshPickerManager::GetInstancePtr()->openRenderMeshPicker(
            [weakPtr](RenderMeshPtr const& _renderMesh)
            {
                PropertyDrawerRenderMeshPtr ptr = weakPtr.lock();
                if (ptr)
                {
                    ptr->setValue(_renderMesh);
                    ptr->eventUIData();
                }
            },
            m_renderMesh);
    }


    //////////////////////////////////////////
    // Class PropertyDrawerRenderMeshAssetRef
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerRenderMeshAssetRef, PropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerRenderMeshAssetRef);

    //////////////////////////////////////////
    PropertyDrawerRenderMeshAssetRef::PropertyDrawerRenderMeshAssetRef()
    {

    }

    //////////////////////////////////////////
    PropertyDrawerRenderMeshAssetRef::~PropertyDrawerRenderMeshAssetRef()
    {
        if (m_drawer)
            m_drawer->eventUIData.unsubscribe(this);

        m_drawer.reset();
    }

    //////////////////////////////////////////
    PropertyDrawerRenderMeshAssetRefPtr PropertyDrawerRenderMeshAssetRef::Create(DataBlock const& _dataBlock)
    {
        PropertyDrawerRenderMeshAssetRefPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerRenderMeshAssetRef, object, init(_dataBlock));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerRenderMeshAssetRef::init(DataBlock const& _dataBlock)
    {
        if (!PropertyDrawer::init(_dataBlock))
            return false;

        m_drawer = PropertyDrawerRenderMesh::Create(_dataBlock);
        m_drawer->eventUIData.subscribe(this, &PropertyDrawerRenderMeshAssetRef::processDataFromUI);

        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerRenderMeshAssetRef::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        m_drawer->buildUI(_parent, _label);
    }

    //////////////////////////////////////////
    void PropertyDrawerRenderMeshAssetRef::setValue(RenderMeshAssetRef const& _value)
    {
        m_drawer->setValue(_value.getRenderMesh());
    }

    //////////////////////////////////////////
    RenderMeshAssetRef PropertyDrawerRenderMeshAssetRef::getValue() const
    {
        return RenderMeshAssetRef(m_drawer->getValue());
    }

    //////////////////////////////////////////
    void PropertyDrawerRenderMeshAssetRef::processDataFromUI()
    {
        eventUIData();
    }

} // namespace Maze
//////////////////////////////////////////
