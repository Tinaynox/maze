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
#include "maze-editor-tools/property-drawers/MazeRenderMesh.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/managers/MazeRenderMeshPickerManager.hpp"
#include "maze-ui/render-mesh-picker/MazeSceneRenderMeshPicker.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsLayout.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
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
    PropertyDrawerRenderMeshPtr PropertyDrawerRenderMesh::Create(String const& _label)
    {
        PropertyDrawerRenderMeshPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerRenderMesh, object, init(_label));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerRenderMesh::init(String const& _label)
    {
        if (!PropertyDrawer::init(_label))
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
            Vec2DF(_parent->getWidth(), 18),
            Vec2DF(0, 0),
            _parent,
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.0f, 1.0f));
        layout->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);
        layout->setAutoWidth(false);
        layout->setExpand(true);

        SystemTextRenderer2DPtr systemText = SystemUIHelper::CreateSystemText(
            EditorToolsHelper::BuildPropertyName(m_label.c_str(), _label).c_str(),
            EditorToolsLayout::c_inspectorPropertyFontSize,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(8, 18),
            Vec2DF(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 0.5f),
            Vec2DF::c_zero);
        systemText->setColor(EditorToolsLayout::c_inspectorPropertyColor);


        m_renderMeshButton = UIHelper::CreateClickButton(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Focused),
            Vec2DF(180, 18),
            Vec2DF::c_zero,
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene());
        m_renderMeshButton->eventClick.subscribe(this, &PropertyDrawerRenderMesh::notifyRenderMeshButtonClick);

        m_selectAssetButton = UIHelper::CreateClickButton(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::SelectAsset),
            Vec2DF(18, 18),
            Vec2DF(-9.0f, 0.0f),
            m_renderMeshButton->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(1.0f, 0.5f),
            Vec2DF(0.5f, 0.5f));
        m_selectAssetButton->setNormalColor({ 50, 50, 50 });
        m_selectAssetButton->setFocusedColor({ 100, 100, 100 });
        m_selectAssetButton->setSelectedColor({ 150, 150, 150 });
        m_selectAssetButton->setPressedColor({ 100, 150, 100 });
        m_selectAssetButton->eventClick.subscribe(this, &PropertyDrawerRenderMesh::notifySelectAssetClick);

        m_renderMeshIcon = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Mesh),
            Vec2DF(12, 12),
            Vec2DF(9.0f, 0.0f),
            materialManager->getColorTextureMaterial(),
            m_renderMeshButton->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 0.5f),
            Vec2DF(0.5f, 0.5f));

        m_renderMeshNameDrawer = SystemUIHelper::CreateSystemText(
            "",
            8,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(160.0f, 0.0f),
            Vec2DF(18.0f, 0.0f),
            m_renderMeshButton->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 0.5f),
            Vec2DF(0.0f, 0.5f));
        m_renderMeshNameDrawer->setColor(ColorU32::c_black);
    }

    //////////////////////////////////////////
    void PropertyDrawerRenderMesh::setString(String const& _value)
    {
        RenderMeshPtr value;
        ValueFromString(value, _value.c_str(), _value.size());
        setValue(value);
    }

    //////////////////////////////////////////
    String PropertyDrawerRenderMesh::getString()
    {
        String value;
        ValueToString(getValue(), value);
        return value;
    }

    //////////////////////////////////////////
    void PropertyDrawerRenderMesh::setValue(RenderMeshPtr const& _value)
    {
        m_renderMesh = _value;

        m_renderMeshIcon->getMeshRenderer()->setEnabled(m_renderMesh != nullptr);

        String name = m_renderMesh ? m_renderMesh->getName() : "None";
        name = FileHelper::GetFileNameWithoutExtension(name);
        m_renderMeshNameDrawer->setText(name);
    }

    //////////////////////////////////////////
    RenderMeshPtr PropertyDrawerRenderMesh::getValue() const
    {
        return m_renderMesh;
    }

    //////////////////////////////////////////
    void PropertyDrawerRenderMesh::notifyRenderMeshButtonClick(Button2D* _button, CursorInputEvent const& _event)
    {

    }

    //////////////////////////////////////////
    void PropertyDrawerRenderMesh::notifySelectAssetClick(Button2D* _button, CursorInputEvent const& _event)
    {
        RenderMeshPickerManager::GetInstancePtr()->openRenderMeshPicker(
            [this](RenderMeshPtr const& _material)
            {
                setValue(_material);
                eventUIData();
            },
            m_renderMesh);
    }

} // namespace Maze
//////////////////////////////////////////
