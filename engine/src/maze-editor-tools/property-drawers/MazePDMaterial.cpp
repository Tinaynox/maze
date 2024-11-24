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
#include "maze-editor-tools/property-drawers/MazePDMaterial.hpp"
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
#include "maze-editor-tools/managers/MazeMaterialPickerManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class PropertyDrawerMaterial
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerMaterial, PropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerMaterial);

    //////////////////////////////////////////
    PropertyDrawerMaterial::PropertyDrawerMaterial()
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerMaterial::~PropertyDrawerMaterial()
    {
        if (m_materialButton)
        {
            m_materialButton->eventClick.unsubscribe(this);
        }

        if (m_selectAssetButton)
            m_selectAssetButton->eventClick.unsubscribe(this);
    }

    //////////////////////////////////////////
    PropertyDrawerMaterialPtr PropertyDrawerMaterial::Create(String const& _label)
    {
        PropertyDrawerMaterialPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerMaterial, object, init(_label));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerMaterial::init(String const& _label)
    {
        if (!PropertyDrawer::init(_label))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerMaterial::buildUI(
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
            EditorToolsHelper::BuildPropertyName(m_label.c_str(), _label).c_str(),
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


        m_materialButton = UIHelper::CreateClickButton(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Focused),
            Vec2F(180, 18),
            Vec2F::c_zero,
            layout->getTransform(),
            _parent->getEntityRaw()->getEcsScene());
        m_materialButton->eventClick.subscribe(this, &PropertyDrawerMaterial::notifyMaterialButtonClick);

        m_dragAndDropFrame = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Focused),
            m_materialButton->getTransform()->getSize(),
            Vec2F::c_zero,
            nullptr,
            m_materialButton->getTransform(),
            _parent->getEntityRaw()->getEcsScene());
        m_dragAndDropFrame->setColor(255, 200, 40);
        m_dragAndDropFrame->getEntityRaw()->ensureComponent<SizePolicy2D>();
        m_dragAndDropFrame->getMeshRenderer()->setEnabled(false);

        m_dragAndDropZone = m_materialButton->getEntityRaw()->ensureComponent<DragAndDropZone>();
        m_dragAndDropZone->eventDragAndDropValidate.subscribe(
            [this](DataBlock const& _data, EntityId _viewEid, bool& _outDropAllowed)
            {
                if (_data.getHashedCString(MAZE_HCS("type")) == MAZE_HCS("assetFile"))
                {
                    AssetFileId afid = _data.getS32(MAZE_HCS("afid"));
                    AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(afid);
                    if (!assetFile)
                        return;

                    MaterialPtr const& material = MaterialManager::GetCurrentInstance()->getOrLoadMaterial(assetFile);
                    if (!material)
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

                    MaterialPtr const& material = MaterialManager::GetCurrentInstance()->getOrLoadMaterial(assetFile);
                    if (!material)
                        return;

                    setValue(material);
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
            m_materialButton->getTransform(),
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(1.0f, 0.5f),
            Vec2F(0.5f, 0.5f));
            m_selectAssetButton->setNormalColor(ColorU32{ 50, 50, 50 });
            m_selectAssetButton->setFocusedColor(ColorU32{ 100, 100, 100 });
            m_selectAssetButton->setSelectedColor(ColorU32{ 150, 150, 150 });
            m_selectAssetButton->setPressedColor(ColorU32{ 100, 150, 100 });
            m_selectAssetButton->eventClick.subscribe(this, &PropertyDrawerMaterial::notifySelectAssetClick);

            m_materialIcon = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Material),
                Vec2F(12, 12),
                Vec2F(9.0f, 0.0f),
                materialManager->getSpriteMaterial(),
                m_materialButton->getTransform(),
                _parent->getEntityRaw()->getEcsScene(),
                Vec2F(0.0f, 0.5f),
                Vec2F(0.5f, 0.5f));

            m_materialNameDrawer = EditorToolsUIHelper::CreateText(
                "",
                EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
                12,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                Vec2F(160.0f, 0.0f),
                Vec2F(18.0f, 0.0f),
                m_materialButton->getTransform(),
                _parent->getEntityRaw()->getEcsScene(),
                Vec2F(0.0f, 0.5f),
                Vec2F(0.0f, 0.5f));
            m_materialNameDrawer->setColor(ColorU32::c_black);
    }

    //////////////////////////////////////////
    void PropertyDrawerMaterial::setValue(MaterialPtr const& _value)
    {
        m_material = _value;

        m_materialIcon->getMeshRenderer()->setEnabled(m_material != nullptr);

        if (m_material)
            m_materialNameDrawer->setText(EditorToolsHelper::GetNameWithoutExtension(m_material->getName()));
        else
            m_materialNameDrawer->setText("None");
    }

    //////////////////////////////////////////
    MaterialPtr PropertyDrawerMaterial::getValue() const
    {
        return m_material;
    }

    //////////////////////////////////////////
    void PropertyDrawerMaterial::notifyMaterialButtonClick(Button2D* _button, CursorInputEvent const& _event)
    {
        if (m_material)
        {
            RenderSystemPtr const& renderSystem = Maze::GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
            MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();
            HashedCString materialName = materialManager->getMaterialName(m_material.get());
            if (!materialName.empty())
            {
                AssetFilePtr const& materialAsset = AssetManager::GetInstancePtr()->getAssetFileByFileName(materialName);
                if (materialAsset)
                {
                    SelectionManager::GetInstancePtr()->selectObject(materialAsset);
                }
                else
                {
                    SelectionManager::GetInstancePtr()->selectObject(m_material);
                }
            }
            else
            {
                SelectionManager::GetInstancePtr()->selectObject(m_material);
            }
        }
    }

    //////////////////////////////////////////
    void PropertyDrawerMaterial::notifySelectAssetClick(Button2D* _button, CursorInputEvent const& _event)
    {
        PropertyDrawerMaterialWPtr weakPtr = cast<PropertyDrawerMaterial>();
        MaterialPickerManager::GetInstancePtr()->openMaterialPicker(
            [weakPtr](MaterialPtr const& _material)
            {
                PropertyDrawerMaterialPtr ptr = weakPtr.lock();
                if (ptr)
                {
                    ptr->setValue(_material);
                    ptr->eventUIData();
                }
            },
            m_material);
    }



    //////////////////////////////////////////
    // Class PropertyDrawerMaterialAssetRef
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerMaterialAssetRef, PropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerMaterialAssetRef);

    //////////////////////////////////////////
    PropertyDrawerMaterialAssetRef::PropertyDrawerMaterialAssetRef()
    {

    }

    //////////////////////////////////////////
    PropertyDrawerMaterialAssetRef::~PropertyDrawerMaterialAssetRef()
    {
        if (m_drawer)
            m_drawer->eventUIData.unsubscribe(this);

        m_drawer.reset();
    }

    //////////////////////////////////////////
    PropertyDrawerMaterialAssetRefPtr PropertyDrawerMaterialAssetRef::Create(String const& _label)
    {
        PropertyDrawerMaterialAssetRefPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerMaterialAssetRef, object, init(_label));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerMaterialAssetRef::init(String const& _label)
    {
        if (!PropertyDrawer::init(_label))
            return false;

        m_drawer = PropertyDrawerMaterial::Create(_label);
        m_drawer->eventUIData.subscribe(this, &PropertyDrawerMaterialAssetRef::processDataFromUI);

        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerMaterialAssetRef::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        m_drawer->buildUI(_parent, _label);
    }

    //////////////////////////////////////////
    void PropertyDrawerMaterialAssetRef::setValue(MaterialAssetRef const& _value)
    {
        m_drawer->setValue(_value.getMaterial());
    }

    //////////////////////////////////////////
    MaterialAssetRef PropertyDrawerMaterialAssetRef::getValue() const
    {
        return MaterialAssetRef(m_drawer->getValue());
    }

    //////////////////////////////////////////
    void PropertyDrawerMaterialAssetRef::processDataFromUI()
    {
        eventUIData();
    }

} // namespace Maze
//////////////////////////////////////////
