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
#include "maze-editor-tools/pickers/MazeSceneFontMaterialPicker.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeCanvasRenderer.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/ecs/components/MazeMeshRendererInstanced.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-ui/managers/MazeFontMaterialManager.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/MazeGPUTextureBuffer.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/helpers/MazeColorHelper.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-editor-tools/managers/MazeFontMaterialPickerManager.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class SceneFontMaterialPicker
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneFontMaterialPicker, EcsRenderScene);

    //////////////////////////////////////////
    SceneFontMaterialPicker::SceneFontMaterialPicker()
    {
    }

    //////////////////////////////////////////
    SceneFontMaterialPicker::~SceneFontMaterialPicker()
    {
        if (FontMaterialPickerManager::GetInstancePtr())
            FontMaterialPickerManager::GetInstancePtr()->eventFontMaterialChanged.unsubscribe(this);

        if (m_filterEditBox)
            m_filterEditBox->eventTextInput.unsubscribe(this);

        clearPreviews();

        if (m_canvasUIElement)
        {
            m_canvasUIElement->eventCursorReleaseIn.unsubscribe(this);
            m_canvasUIElement->eventCursorReleaseOut.unsubscribe(this);
        }

        m_canvas.reset();
    }

    //////////////////////////////////////////
    SceneFontMaterialPickerPtr SceneFontMaterialPicker::Create(RenderTargetPtr const& _renderTarget)
    {
        SceneFontMaterialPickerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneFontMaterialPicker, object, init(_renderTarget));
        return object;
    }

    //////////////////////////////////////////
    bool SceneFontMaterialPicker::init(RenderTargetPtr const& _renderTarget)
    {
        if (!EcsRenderScene::init(_renderTarget))
            return false;

        create2D();
        
        FontMaterialPickerManager::GetInstancePtr()->eventFontMaterialChanged.subscribe(this, &SceneFontMaterialPicker::notifyFontMaterialChanged);

        return true;
    }

    //////////////////////////////////////////
    void SceneFontMaterialPicker::setup()
    {
        
        updateUI();
    }

    //////////////////////////////////////////
    void SceneFontMaterialPicker::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void SceneFontMaterialPicker::create2D()
    {
        ColorU32 bandColor(176, 176, 176);

        EntityPtr canvasEntity = createEntity();
        m_canvas = canvasEntity->createComponent<Canvas>();
        m_canvas->setClearColorFlag(true);
        m_canvas->setClearDepthFlag(true);
        m_canvas->setClearColor(ColorU32(203, 203, 203, 255));
        m_canvas->setRenderTarget(m_renderTarget);
        m_canvasUIElement = canvasEntity->ensureComponent<UIElement2D>();
        m_canvasUIElement->eventCursorReleaseIn.subscribe(this, &SceneFontMaterialPicker::notifyCanvasCursorReleaseIn);
        m_canvasUIElement->eventCursorReleaseOut.subscribe(this, &SceneFontMaterialPicker::notifyCanvasCursorReleaseOut);

        m_filterEditBox = UIHelper::CreateDefaultEditBox(
            "",
            FontMaterialPtr(),
            12,
            Vec2F(m_canvas->getTransform()->getSize().x - 10.0f, 18),
            Vec2F(5, -2),
            m_canvas->getTransform(),
            m_canvas->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        m_filterEditBox->eventTextInput.subscribe(this, &SceneFontMaterialPicker::notifyFilterTextInput);
        m_filterEditBox->getTransform()->setZ(100000);
        SizePolicy2DPtr filterSizePolicy = m_filterEditBox->getEntityRaw()->ensureComponent<SizePolicy2D>();
        filterSizePolicy->setFlag(SizePolicy2D::Flags::Height, false);
        filterSizePolicy->setSizeDelta(-10.0f, 0.0f);

        F32 const topOffset = 22.0f;
        ScrollRect2DPtr scrollRect = UIHelper::CreateDefaultScrollRect(
            m_canvas->getTransform()->getSize() - Vec2F(0.0f, topOffset),
            Vec2F(0.0f, -topOffset),
            m_canvas->getTransform(),
            m_canvas->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f),
            false,
            true);
        scrollRect->getViewportTransform()->getEntityRaw()->getComponent<ScissorMask2D>()->setPadding(0, 0, 0, 0);
        scrollRect->getEntityRaw()->ensureComponent<SizePolicy2D>()->setSizeDelta(0.0f, -topOffset);
        scrollRect->getEntityRaw()->getComponent<MeshRendererInstanced>()->setEnabled(false);

        m_layout = scrollRect->getContentTransform()->getEntityRaw()->createComponent<VerticalLayout2D>();
        m_layout->setHorizontalAlignment(HorizontalAlignment2D::Left);
        m_layout->setVerticalAlignment(VerticalAlignment2D::Top);
        m_layout->setAutoWidth(false);
        m_layout->setAutoHeight(true);
        m_layout->setSpacing(5.0f);
        m_layout->setPaddingTop(5.0f);
        SizePolicy2DPtr layoutSizePolicy = m_layout->getEntityRaw()->ensureComponent<SizePolicy2D>();
        layoutSizePolicy->setFlag(SizePolicy2D::Flags::Height, false);

        FontMaterialManager::GetInstancePtr()->loadAllAssetFontMaterials();

        updateFontMaterials();
        updateUI();
    }

    //////////////////////////////////////////
    void SceneFontMaterialPicker::notifyFontMaterialChanged(FontMaterialPtr const& _material)
    {
        updateUI();
    }

    //////////////////////////////////////////
    void SceneFontMaterialPicker::updateFontMaterials()
    {
        clearPreviews();

        String const& filterText = m_filterEditBox->getText();

        Vector<FontMaterialPtr> materials;
        for (FontMaterialPtr const& material : FontMaterialManager::GetInstancePtr()->getFontMaterialsSorted())
            if (filterText.empty() || material->getName().getString().find(filterText) != String::npos)
                materials.push_back(material);
        materials.insert(materials.begin(), FontMaterialPtr());

        m_layout->getTransform()->removeAllChildren();

        HorizontalLayout2DPtr horizontalLayout;

        for (S32 i = 0; i < (S32)materials.size(); ++i)
        {
            FontMaterialPtr const& material = materials[i];

            if (i % 4 == 0)
                horizontalLayout.reset();

            if (!horizontalLayout)
            {
                EntityPtr horizontalLayoutObject = m_layout->getEntityRaw()->getEcsScene()->createEntity();
                Transform2DPtr horizontalLayoutTransform = horizontalLayoutObject->ensureComponent<Transform2D>();
                horizontalLayoutTransform->setParent(m_layout->getTransform());
                horizontalLayoutTransform->setWidth(m_layout->getTransform()->getWidth());
                horizontalLayout = horizontalLayoutObject->ensureComponent<HorizontalLayout2D>();
                horizontalLayout->setHorizontalAlignment(HorizontalAlignment2D::Left);
                horizontalLayout->setAutoWidth(false);
                horizontalLayout->getTransform()->setHeight(110.0f);
                horizontalLayout->setExpand(false);
                horizontalLayout->setPaddingLeft(4.0f);
                horizontalLayout->setPaddingRight(4.0f);
            }

            FontMaterialPreviewData data = createFontMaterialPreview(material);
            data.bodyTransform->setParent(horizontalLayout->getTransform());
            data.button->eventClick.subscribe(this, &SceneFontMaterialPicker::notifyButtonClick);

            m_previews.push_back(data);
        }
    }

    //////////////////////////////////////////
    void SceneFontMaterialPicker::updateUI()
    {
        if (getState() == EcsSceneState::Destroy)
            return;

        FontMaterialPtr const& currentFontMaterial = FontMaterialPickerManager::GetInstancePtr()->getFontMaterial();

        for (Size i = 0; i < m_previews.size(); ++i)
        {
            FontMaterialPreviewData const& previewData = m_previews[i];        
            FontMaterialPtr const& material = previewData.material;

            bool checked = (material == currentFontMaterial);
            previewData.button->setChecked(checked);

            if (checked)
                previewData.titleText->setColor(ColorU32(66, 137, 255));
            else
                previewData.titleText->setColor(ColorU32::c_black);
        }
    }

    //////////////////////////////////////////
    void SceneFontMaterialPicker::notifyCanvasCursorReleaseIn(Vec2F const& _positionOS, CursorInputEvent& _event)
    {
        
    }

    //////////////////////////////////////////
    void SceneFontMaterialPicker::notifyCanvasCursorReleaseOut(CursorInputEvent& _event)
    {
        
    }

    //////////////////////////////////////////
    SceneFontMaterialPicker::FontMaterialPreviewData SceneFontMaterialPicker::createFontMaterialPreview(FontMaterialPtr const& _material)
    {
        FontMaterialPreviewData data;
        data.material = _material;

        data.bodyTransform = SpriteHelper::CreateTransform2D(
            { 94.0f, 110.0f },
            Vec2F::c_zero,
            nullptr,
            m_layout->getEntityRaw()->getEcsScene());
        data.bodyTransform->getEntityRaw()->ensureComponent<ScissorMask2D>();

        data.button = UIHelper::CreateToggleButton(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
            { data.bodyTransform->getWidth(), data.bodyTransform->getWidth() },
            Vec2F::c_zero,
            data.bodyTransform,
            data.bodyTransform->getEntityRaw()->getEcsScene(),
            { 0.0f, 1.0f },
            { 0.0f, 1.0f });
        data.button->setCheckByClick(false);

        SpriteRenderer2DPtr sprite = SpriteHelper::CreateSprite(
            ColorU32::c_white,
            data.button->getTransform()->getSize() - Vec2F(6.0f, 6.0f),
            Vec2F::c_zero,
            GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getSpriteMaterial(),
            data.button->getTransform(),
            data.bodyTransform->getEntityRaw()->getEcsScene());

        String materialName = _material ? _material->getName().getString() : "None";

        materialName = EditorToolsHelper::GetNameWithoutExtension(materialName);

        data.titleText = EditorToolsUIHelper::CreateText(
            materialName.c_str(),
            EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
            12,
            HorizontalAlignment2D::Center,
            VerticalAlignment2D::Middle,
            { data.bodyTransform->getWidth(), 10.0f },
            { 0.0f, -105.0f },
            data.bodyTransform,
            data.bodyTransform->getEntityRaw()->getEcsScene(),
            { 0.5f, 1.0f },
            { 0.5f, 0.5f });
        data.titleText->setColor(ColorU32::c_black);

        F32 scalar = data.bodyTransform->getWidth() / (materialName.size() * 8.0f);
        if (scalar < 1.0f)
            data.titleText->getTransform()->setLocalScale(scalar);
        else
            data.titleText->getTransform()->setLocalScale(1.0f);

        return data;
    }

    //////////////////////////////////////////
    void SceneFontMaterialPicker::clearPreviews()
    {
        for (FontMaterialPreviewData const& preview : m_previews)
            preview.button->eventClick.unsubscribe(this);

        m_previews.clear();
    }

    //////////////////////////////////////////
    void SceneFontMaterialPicker::notifyButtonClick(Button2D* _button, CursorInputEvent& _event)
    {
        for (FontMaterialPreviewData const& preview : m_previews)
        {
            if (_button == preview.button.get())
            {
                FontMaterialPickerManager::GetInstancePtr()->setFontMaterial(preview.material);
                break;
            }
        }
    }

    //////////////////////////////////////////
    void SceneFontMaterialPicker::notifyFilterTextInput(EditBox2D* _editBox)
    {
        updateFontMaterials();
    }

} // namespace Maze
//////////////////////////////////////////
