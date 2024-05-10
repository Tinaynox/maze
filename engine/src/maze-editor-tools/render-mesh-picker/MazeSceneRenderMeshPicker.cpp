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
#include "maze-editor-tools/render-mesh-picker/MazeSceneRenderMeshPicker.hpp"
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
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
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
#include "maze-editor-tools/managers/MazeRenderMeshPickerManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
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
    // Class SceneRenderMeshPicker
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneRenderMeshPicker, EcsRenderScene);

    //////////////////////////////////////////
    SceneRenderMeshPicker::SceneRenderMeshPicker()
    {
    }

    //////////////////////////////////////////
    SceneRenderMeshPicker::~SceneRenderMeshPicker()
    {
        if (RenderMeshPickerManager::GetInstancePtr())
            RenderMeshPickerManager::GetInstancePtr()->eventRenderMeshChanged.unsubscribe(this);

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
    SceneRenderMeshPickerPtr SceneRenderMeshPicker::Create(RenderTargetPtr const& _renderTarget)
    {
        SceneRenderMeshPickerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneRenderMeshPicker, object, init(_renderTarget));
        return object;
    }

    //////////////////////////////////////////
    bool SceneRenderMeshPicker::init(RenderTargetPtr const& _renderTarget)
    {
        if (!EcsRenderScene::init(_renderTarget))
            return false;

        create2D();
        
        RenderMeshPickerManager::GetInstancePtr()->eventRenderMeshChanged.subscribe(this, &SceneRenderMeshPicker::notifyRenderMeshChanged);

        return true;
    }

    //////////////////////////////////////////
    void SceneRenderMeshPicker::setup()
    {
        
        updateUI();
    }

    //////////////////////////////////////////
    void SceneRenderMeshPicker::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void SceneRenderMeshPicker::create2D()
    {
        ColorU32 bandColor(176, 176, 176);

        EntityPtr canvasEntity = createEntity();
        m_canvas = canvasEntity->createComponent<Canvas>();
        m_canvas->setClearColorFlag(true);
        m_canvas->setClearDepthFlag(true);
        m_canvas->setClearColor(ColorU32(203, 203, 203, 255));
        m_canvas->setRenderTarget(m_renderTarget);
        m_canvasUIElement = canvasEntity->ensureComponent<UIElement2D>();
        m_canvasUIElement->eventCursorReleaseIn.subscribe(this, &SceneRenderMeshPicker::notifyCanvasCursorReleaseIn);
        m_canvasUIElement->eventCursorReleaseOut.subscribe(this, &SceneRenderMeshPicker::notifyCanvasCursorReleaseOut);

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
        m_filterEditBox->eventTextInput.subscribe(this, &SceneRenderMeshPicker::notifyFilterTextInput);
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
        scrollRect->getEntityRaw()->getComponent<MeshRenderer>()->setEnabled(false);

        m_layout = scrollRect->getContentTransform()->getEntityRaw()->createComponent<VerticalLayout2D>();
        m_layout->setHorizontalAlignment(HorizontalAlignment2D::Left);
        m_layout->setVerticalAlignment(VerticalAlignment2D::Top);
        m_layout->setAutoWidth(false);
        m_layout->setAutoHeight(true);
        m_layout->setSpacing(5.0f);
        m_layout->setPaddingTop(5.0f);
        SizePolicy2DPtr layoutSizePolicy = m_layout->getEntityRaw()->ensureComponent<SizePolicy2D>();
        layoutSizePolicy->setFlag(SizePolicy2D::Flags::Height, false);

        updateRenderMeshs();
        updateUI();
    }

    //////////////////////////////////////////
    void SceneRenderMeshPicker::notifyRenderMeshChanged(RenderMeshPtr const& _material)
    {
        updateUI();
    }

    //////////////////////////////////////////
    void SceneRenderMeshPicker::updateRenderMeshs()
    {
        clearPreviews();

        String const& filterText = m_filterEditBox->getText();

        Vector<RenderMeshPtr> meshes;
        for (RenderMeshPtr const& mesh : RenderMeshManager::GetCurrentInstancePtr()->getRenderMeshesSorted())
            if (filterText.empty() || mesh->getName().find(filterText) != String::npos)
                meshes.push_back(mesh);
        meshes.insert(meshes.begin(), RenderMeshPtr());

        m_layout->getTransform()->removeAllChildren();

        HorizontalLayout2DPtr horizontalLayout;

        for (S32 i = 0; i < (S32)meshes.size(); ++i)
        {
            RenderMeshPtr const& mesh = meshes[i];

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

            RenderMeshPreviewData data = createRenderMeshPreview(mesh);
            data.bodyTransform->setParent(horizontalLayout->getTransform());
            data.button->eventClick.subscribe(this, &SceneRenderMeshPicker::notifyButtonClick);

            m_previews.push_back(data);
        }
    }

    //////////////////////////////////////////
    void SceneRenderMeshPicker::updateUI()
    {
        if (getState() == EcsSceneState::Destroy)
            return;

        RenderMeshPtr const& currentRenderMesh = RenderMeshPickerManager::GetInstancePtr()->getRenderMesh();

        for (Size i = 0; i < m_previews.size(); ++i)
        {
            RenderMeshPreviewData const& previewData = m_previews[i];        
            RenderMeshPtr const& material = previewData.renderMesh;

            bool checked = (material == currentRenderMesh);
            previewData.button->setChecked(checked);

            if (checked)
                previewData.titleText->setColor(ColorU32(66, 137, 255));
            else
                previewData.titleText->setColor(ColorU32::c_black);
        }
    }

    //////////////////////////////////////////
    void SceneRenderMeshPicker::notifyCanvasCursorReleaseIn(Vec2F const& _positionOS, CursorInputEvent const& _event)
    {
        
    }

    //////////////////////////////////////////
    void SceneRenderMeshPicker::notifyCanvasCursorReleaseOut(CursorInputEvent const& _event)
    {
        
    }

    //////////////////////////////////////////
    SceneRenderMeshPicker::RenderMeshPreviewData SceneRenderMeshPicker::createRenderMeshPreview(RenderMeshPtr const& _material)
    {
        RenderMeshPreviewData data;
        data.renderMesh = _material;

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
            GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getColorTextureMaterial(),
            data.button->getTransform(),
            data.bodyTransform->getEntityRaw()->getEcsScene());

        String renderMeshName = _material ? _material->getName() : "None";
        renderMeshName = FileHelper::GetFileNameWithoutExtension(renderMeshName);

        data.titleText = EditorToolsUIHelper::CreateText(
            renderMeshName.c_str(),
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

        F32 scalar = data.bodyTransform->getWidth() / (renderMeshName.size() * 8.0f);
        if (scalar < 1.0f)
            data.titleText->getTransform()->setLocalScale(scalar);
        else
            data.titleText->getTransform()->setLocalScale(1.0f);

        return data;
    }

    //////////////////////////////////////////
    void SceneRenderMeshPicker::clearPreviews()
    {
        for (RenderMeshPreviewData const& preview : m_previews)
            preview.button->eventClick.unsubscribe(this);

        m_previews.clear();
    }

    //////////////////////////////////////////
    void SceneRenderMeshPicker::notifyButtonClick(Button2D* _button, CursorInputEvent const& _event)
    {
        for (RenderMeshPreviewData const& preview : m_previews)
        {
            if (_button == preview.button.get())
            {
                RenderMeshPickerManager::GetInstancePtr()->setRenderMesh(preview.renderMesh);
                break;
            }
        }
    }

    //////////////////////////////////////////
    void SceneRenderMeshPicker::notifyFilterTextInput(EditBox2D* _editBox)
    {
        updateRenderMeshs();
    }

} // namespace Maze
//////////////////////////////////////////
