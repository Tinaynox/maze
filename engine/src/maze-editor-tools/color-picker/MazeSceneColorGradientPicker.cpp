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
#include "maze-editor-tools/color-picker/MazeSceneColorGradientPicker.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSystemManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeStaticName.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeMeshRendererInstanced.hpp"
#include "maze-graphics/ecs/components/MazeCanvasRenderer.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
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
#include "maze-editor-tools/managers/MazeColorGradientPickerManager.hpp"
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
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneColorGradientPicker
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneColorGradientPicker, EcsRenderScene);

    //////////////////////////////////////////
    SceneColorGradientPicker::SceneColorGradientPicker()
    {
    }

    //////////////////////////////////////////
    SceneColorGradientPicker::~SceneColorGradientPicker()
    {
        if (ColorGradientPickerManager::GetInstancePtr())
            ColorGradientPickerManager::GetInstancePtr()->eventGradientChanged.unsubscribe(this);

        while (!m_alphaTags.empty())
        {
            if (m_alphaTags.back()->getEntityRaw())
            {
                m_alphaTags.back()->getToggleButton()->eventClick.unsubscribe(this);
                m_alphaTags.back()->getToggleButton()->eventPressedChanged.unsubscribe(this);
                m_alphaTags.back()->getToggleButton()->eventCursorPressIn.unsubscribe(this);

                m_alphaTags.back()->getEntityRaw()->removeFromEcsWorld();
            }

            m_alphaTags.pop_back();
        }

        while (!m_colorTags.empty())
        {
            if (m_colorTags.back()->getEntityRaw())
            {
                m_colorTags.back()->getToggleButton()->eventClick.unsubscribe(this);
                m_colorTags.back()->getToggleButton()->eventPressedChanged.unsubscribe(this);
                m_colorTags.back()->getToggleButton()->eventCursorPressIn.unsubscribe(this);

                m_colorTags.back()->getEntityRaw()->removeFromEcsWorld();
            }

            m_colorTags.pop_back();
        }

        if (m_gradientClickButton)
        {
            m_gradientClickButton->eventClick.unsubscribe(this);
            m_gradientClickButton->getUIElement()->eventCursorPressIn.unsubscribe(this);
        }
        
        if (m_modeDropdown)
        {
            m_modeDropdown->eventValueChanged.unsubscribe(this);
        }


        if (m_colorEdit)
            m_colorEdit->eventColorChanged.unsubscribe(this);


        if (m_canvasUIElement)
        {
            m_canvasUIElement->eventCursorRelease.unsubscribe(this);
        }

        m_canvas.reset();
    }

    //////////////////////////////////////////
    SceneColorGradientPickerPtr SceneColorGradientPicker::Create(RenderTargetPtr const& _renderTarget)
    {
        SceneColorGradientPickerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneColorGradientPicker, object, init(_renderTarget));
        return object;
    }

    //////////////////////////////////////////
    bool SceneColorGradientPicker::init(RenderTargetPtr const& _renderTarget)
    {
        if (!EcsRenderScene::init(_renderTarget))
            return false;

        create2D();
        
        ColorGradientPickerManager::GetInstancePtr()->eventGradientChanged.subscribe(this, &SceneColorGradientPicker::notifyGradientChanged);

        return true;
    }

    //////////////////////////////////////////
    void SceneColorGradientPicker::setup()
    {
        
    }

    //////////////////////////////////////////
    void SceneColorGradientPicker::update(F32 _dt)
    {
        if (m_draggingTag)
        {
            if (m_colorTagIndex >= 0)
            {
                Vec2F const& cursorPosition = InputManager::GetInstancePtr()->getCursorPosition(0);

                Vec2F cursorPositionGradientSpace;
                m_gradientRenderer->getTransform()->getWorldTransform().inversed().transform(
                    cursorPosition,
                    cursorPositionGradientSpace);

                m_draggingTagDistance += m_draggingTagPrevCursorPositionGradientSpace.distance(cursorPositionGradientSpace);
                m_draggingTagPrevCursorPositionGradientSpace = cursorPositionGradientSpace;

                if (m_draggingTagDistance > 1.0f)
                {
                    ColorGradient const& gradient = ColorGradientPickerManager::GetInstancePtr()->getGradient();

                    if (m_editMode == EditMode::AlphaTag)
                    {
                        if (m_colorTagIndex < gradient.getKeysAlpha().size())
                        {
                            Vec2F localPosition = m_gradientRenderer->getTransform()->getWorldTransform().inversed().transform(cursorPosition);

                            F32 t = Math::Clamp01(localPosition.x / m_gradientRenderer->getTransform()->getWidth());
                            changeCurrentAlphaTagTime(t);
                        }
                    }
                    else
                    if (m_editMode == EditMode::ColorTag)
                    {
                        if (m_colorTagIndex < gradient.getKeysRGB().size())
                        {
                            Vec2F localPosition = m_gradientRenderer->getTransform()->getWorldTransform().inversed().transform(cursorPosition);

                            F32 t = Math::Clamp01(localPosition.x / m_gradientRenderer->getTransform()->getWidth());
                            changeCurrentColorTagTime(t);
                        }
                    }
                }
            }
        }
    }


    //////////////////////////////////////////
    void SceneColorGradientPicker::create2D()
    {
        ColorU32 bandColor(176, 176, 176);

        EntityPtr canvasEntity = createEntity();
        m_canvas = canvasEntity->createComponent<Canvas>();
        m_canvas->setClearColorFlag(true);
        m_canvas->setClearDepthFlag(true);
        m_canvas->setClearColor(ColorU32(203, 203, 203, 255));
        m_canvas->setRenderTarget(m_renderTarget);
        m_canvasUIElement = canvasEntity->ensureComponent<UIElement2D>();
        m_canvasUIElement->eventCursorRelease.subscribe(this, &SceneColorGradientPicker::notifyCanvasCursorRelease);

        ColorGradient const& gradient = ColorGradientPickerManager::GetInstancePtr()->getGradient();

        // Mode
        {

            HorizontalLayout2DPtr rowLayout = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                Vec2F(336.0f, 18.0f),
                Vec2F(12.0f, -10.0f),
                m_canvas->getTransform(),
                this,
                Vec2F(0.0f, 1.0f),
                Vec2F(0.0f, 1.0f));
            rowLayout->setExpand(false);
            rowLayout->setAutoWidth(false);
            rowLayout->setSpacing(5.0f);

            AbstractTextRenderer2DPtr label = EditorToolsUIHelper::CreateText(
                "Mode",
                EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
                12,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                { 80.0f, 18.0f },
                Vec2F::c_zero,
                rowLayout->getTransform(),
                this);
            label->setColor(ColorU32::c_black);

            m_modeDropdown = EditorToolsUIHelper::CreateDefaultDropdown(
                12,
                Vec2F(158.0f, 18.0f),
                Vec2F(0.0f, 0.0f),
                rowLayout->getTransform(),
                this);
            m_modeDropdown->eventValueChanged.subscribe(this, &SceneColorGradientPicker::notifyModeChanged);

            m_modeDropdown->addOption("Fixed");
            m_modeDropdown->addOption("Linear");

            m_modeDropdown->setValue((S32)gradient.getMode());


            m_copyButton = UIHelper::CreateDefaultClickButton(
                "C",
                { 18.0f, 18.0f },
                { -32.0f, -12.0f },
                m_canvas->getTransform(),
                this,
                Vec2F(1.0f, 1.0f),
                Vec2F(1.0f, 1.0f));
            m_copyButton->eventClick.subscribe(
                [](Button2D* _button, CursorInputEvent& _event)
                {
                    ColorGradient gradient = ColorGradientPickerManager::GetInstancePtr()->getGradient();
                    SystemManager::GetInstancePtr()->setClipboardString(gradient.toString());
                });

            m_pasteButton = UIHelper::CreateDefaultClickButton(
                "P",
                { 18.0f, 18.0f },
                { -12.0f, -12.0f },
                m_canvas->getTransform(),
                this,
                Vec2F(1.0f, 1.0f),
                Vec2F(1.0f, 1.0f));
            m_pasteButton->eventClick.subscribe(
                [](Button2D* _button, CursorInputEvent& _event)
            {
                String text = SystemManager::GetInstancePtr()->getClipboardAsString();
                ColorGradient gradient = ColorGradient::FromString(text);
                ColorGradientPickerManager::GetInstancePtr()->setGradient(gradient);

            });


            m_copyXMLButton = UIHelper::CreateDefaultClickButton(
                "CX",
                { 18.0f, 18.0f },
                { -72.0f, -12.0f },
                m_canvas->getTransform(),
                this,
                Vec2F(1.0f, 1.0f),
                Vec2F(1.0f, 1.0f));
            m_copyXMLButton->eventClick.subscribe(
                [](Button2D* _button, CursorInputEvent& _event)
            {
                ColorGradient gradient = ColorGradientPickerManager::GetInstancePtr()->getGradient();
                String text = gradient.toString();
                StringHelper::ReplaceSubstring(text, "\"", "&quot;");
                SystemManager::GetInstancePtr()->setClipboardString(text);
            });

            m_pasteXMLButton = UIHelper::CreateDefaultClickButton(
                "PX",
                { 18.0f, 18.0f },
                { -52.0f, -12.0f },
                m_canvas->getTransform(),
                this,
                Vec2F(1.0f, 1.0f),
                Vec2F(1.0f, 1.0f));
            m_pasteXMLButton->eventClick.subscribe(
                [](Button2D* _button, CursorInputEvent& _event)
            {
                String text = SystemManager::GetInstancePtr()->getClipboardAsString();
                StringHelper::ReplaceSubstring(text, "&quot;", "\"");
                ColorGradient gradient = ColorGradient::FromString(text);
                ColorGradientPickerManager::GetInstancePtr()->setGradient(gradient);

            });
        }

        // Gradient
        {
            m_gradientRendererHolder = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Frame01),
                Vec2F(340.0f, 44.0f),
                Vec2F(10.0f, -50.0f),
                GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getSpriteMaterial(),
                m_canvas->getTransform(),
                this,
                Vec2F(0.0f, 1.0f),
                Vec2F(0.0f, 1.0f));
            m_gradientRendererHolder->setColor(bandColor);

            SpriteRenderer2DPtr chessRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::TransparentChess),
                m_gradientRendererHolder->getTransform()->getSize() - Vec2F(2.0f, 2.0f),
                Vec2F(1.0f, 1.0f),
                GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getSpriteMaterial(),
                m_gradientRendererHolder->getTransform(),
                this,
                Vec2F(0.0f, 0.0f),
                Vec2F(0.0f, 0.0f));
            chessRenderer->getMaterial()->setUniform(
                MAZE_HCS("u_baseMapST"),
                Vec4F(
                    chessRenderer->getTransform()->getWidth() / 8.0f,
                    chessRenderer->getTransform()->getHeight() / 8.0f,
                    0.0f,
                    0.0f));

            m_gradientRenderer = SpriteHelper::CreateSprite(
                ColorU32::c_white,
                m_gradientRendererHolder->getTransform()->getSize() - Vec2F(2.0f, 2.0f),
                Vec2F(1.0f, 1.0f),
                GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getColorMaterial(),
                m_gradientRendererHolder->getTransform(),
                this,
                Vec2F(0.0f, 0.0f),
                Vec2F(0.0f, 0.0f));


            m_gradientClickButton = UIHelper::CreateDefaultClickButton(
                nullptr,
                Vec2F(m_gradientRenderer->getTransform()->getWidth() + 8, m_gradientRenderer->getTransform()->getHeight() + 30),
                Vec2F::c_zero,
                m_gradientRenderer->getTransform(),
                this);
            m_gradientClickButton->eventClick.subscribe(this, &SceneColorGradientPicker::notifyGradientClickButtonClick);
            m_gradientClickButton->getUIElement()->eventCursorPressIn.subscribe(this, &SceneColorGradientPicker::notifyGradientClickButtonCursorPressIn);
            m_gradientClickButton->setNormalColor(ColorU32::c_transparent);
            m_gradientClickButton->setFocusedColor(ColorU32::c_transparent);
            m_gradientClickButton->setPressedColor(ColorU32::c_transparent);
            m_gradientClickButton->setSelectedColor(ColorU32::c_transparent);
            m_gradientClickButton->getTransitionSprite()->setColor(ColorU32::c_transparent);
        }

        // Alpha
        {
            m_alphaHolder = createEntity();
            m_alphaHolder->setActiveSelf(false);
            Transform2DPtr alphaHolderTransform = m_alphaHolder->ensureComponent<Transform2D>();
            alphaHolderTransform->setParent(m_canvas->getTransform());
            alphaHolderTransform->setSize(m_canvas->getTransform()->getSize());
            alphaHolderTransform->setAnchor(0.0f, 0.0f);
            alphaHolderTransform->setPivot(0.0f, 0.0f);

            HorizontalLayout2DPtr rowLayout = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                Vec2F(155.0f, 18.0f),
                Vec2F(20.0f, -120.0f),
                alphaHolderTransform,
                this,
                Vec2F(0.0f, 1.0f),
                Vec2F(0.0f, 1.0f));
            rowLayout->setExpand(true);
            rowLayout->setAutoWidth(false);

            AbstractTextRenderer2DPtr label = EditorToolsUIHelper::CreateText(
                "Alpha",
                EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
                12,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                { 8.0f * 5.0f, 18.0f },
                Vec2F::c_zero,
                rowLayout->getTransform(),
                this);
            label->setColor(ColorU32::c_black);

            m_alphaSlider = UIHelper::CreateDefaultSlider(
                0.0f,
                Vec2F(60.0f, 18.0f),
                Vec2F::c_zero,
                rowLayout->getTransform(),
                this);
            m_alphaSlider->eventValueChanged.subscribe(this, &SceneColorGradientPicker::notifyAlphaSliderValueChanged);

            m_alphaEdit = UIHelper::CreateDefaultEditBox(
                "255",
                FontMaterialPtr(),
                12,
                Vec2F(30.0f, 18.0f),
                Vec2F::c_zero,
                rowLayout->getTransform(),
                this);
            m_alphaEdit->getTextRenderer()->setHorizontalAlignment(HorizontalAlignment2D::Center);
            m_alphaEdit->eventTextInput.subscribe(this, &SceneColorGradientPicker::notifyAlphaEditTextInput);
        }

        // Color
        {
            m_colorHolder = createEntity();
            Transform2DPtr colorHolderTransform = m_colorHolder->ensureComponent<Transform2D>();
            colorHolderTransform->setParent(m_canvas->getTransform());
            colorHolderTransform->setSize(m_canvas->getTransform()->getSize());
            colorHolderTransform->setAnchor(0.0f, 0.0f);
            colorHolderTransform->setPivot(0.0f, 0.0f);

            HorizontalLayout2DPtr rowLayout = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                Vec2F(125.0f, 18.0f),
                Vec2F(30.0f, -120.0f),
                colorHolderTransform,
                this,
                Vec2F(0.0f, 1.0f),
                Vec2F(0.0f, 1.0f));
            rowLayout->setExpand(true);
            rowLayout->setAutoWidth(false);

            AbstractTextRenderer2DPtr label = EditorToolsUIHelper::CreateText(
                "Color",
                EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
                12,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                { 8.0f * 5, 18.0f },
                Vec2F::c_zero,
                rowLayout->getTransform(),
                this);
            label->setColor(ColorU32::c_black);

            m_colorEdit = EditorToolsUIHelper::CreateDefaultColorHDREdit(
                ColorF128::c_red,
                EditorToolsStyles::GetInstancePtr()->getDefaultBoldFontMaterial(),
                Vec2F(60.0f, 18.0f),
                Vec2F::c_zero,
                rowLayout->getTransform(),
                this,
                Vec2F(0.5f, 0.5f),
                Vec2F(0.5f, 0.5f),
                false,
                false);
            m_colorEdit->eventColorChanged.subscribe(this, &SceneColorGradientPicker::notifyColorEditColorChanged);
        }

        // Location
        {
            m_locationHolder = createEntity();
            Transform2DPtr locationHolderTransform = m_locationHolder->ensureComponent<Transform2D>();
            locationHolderTransform->setParent(m_canvas->getTransform());
            locationHolderTransform->setSize(m_canvas->getTransform()->getSize());
            locationHolderTransform->setAnchor(0.0f, 0.0f);
            locationHolderTransform->setPivot(0.0f, 0.0f);

            HorizontalLayout2DPtr rowLayout = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                Vec2F(135.0f, 18.0f),
                Vec2F(200.0f, -120.0f),
                locationHolderTransform,
                this,
                Vec2F(0.0f, 1.0f),
                Vec2F(0.0f, 1.0f));
            rowLayout->setExpand(true);
            rowLayout->setAutoWidth(false);

            AbstractTextRenderer2DPtr label = EditorToolsUIHelper::CreateText(
                "Location",
                EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
                12,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                { 8.0f * 8.0f, 18.0f },
                Vec2F::c_zero,
                rowLayout->getTransform(),
                this);
            label->setColor(ColorU32::c_black);

            m_locationEdit = UIHelper::CreateDefaultEditBox(
                "51.5",
                FontMaterialPtr(),
                12,
                Vec2F(45.0f, 18.0f),
                Vec2F::c_zero,
                rowLayout->getTransform(),
                this);
            m_locationEdit->eventTextInput.subscribe(this, &SceneColorGradientPicker::notifyLocationEditTextInput);

            AbstractTextRenderer2DPtr label2 = EditorToolsUIHelper::CreateText(
                "%",
                EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
                12,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                { 8.0f, 18.0f },
                Vec2F::c_zero,
                rowLayout->getTransform(),
                this);
            label2->setColor(ColorU32::c_black);
        }

        updateUI();
        updateEditModeUI();
    }

    //////////////////////////////////////////
    void SceneColorGradientPicker::notifyGradientChanged(ColorGradient const& _gradient)
    {
        updateUI();
    }

    //////////////////////////////////////////
    void SceneColorGradientPicker::updateUI()
    {
        if (getState() == EcsSceneState::Destroy)
            return;

        if (m_gradientRenderer)
        {
            ColorGradient gradient = ColorGradientPickerManager::GetInstancePtr()->getGradient();
            gradient.clamp01();
            gradient.addKey(0.0f, gradient.evaluate(0.0f));
            gradient.addKey(1.0f, gradient.evaluate(1.0f));

            Vec2F const& size = m_gradientRenderer->getTransform()->getSize();
            Vec4F uv = Vec4F(0.0f, 0.0f, 1.0f, 1.0f);
            F32 canvasRendererAlpha = m_gradientRenderer->getCanvasRenderer()->getAlpha();
            FastVector<Pair<F32, Vec4F>> gradientColors = gradient.toRawColors();

            F32 startTime = gradientColors.front().first;
            F32 deltaTime = gradientColors.back().first - gradientColors.front().first;

            MeshPtr mesh = Mesh::Create();

            SubMeshPtr subMesh = SubMesh::Create();
            subMesh->setRenderDrawTopology(RenderDrawTopology::Triangles);

            Vector<Vec3F> positions;
            Vector<Vec3F> normals;
            Vector<Vec4F> colors;
            Vector<Vec2F> uvs;

            Vector<U16> indices;

            for (Size i = 0, in = gradientColors.size() - 1; i < in; ++i)
            {
                F32 t0 = gradientColors[i].first;
                F32 t1 = gradientColors[i + 1].first;

                F32 p0 = (t0 - startTime) / deltaTime;
                F32 p1 = (t1 - startTime) / deltaTime;

                Vec4F color0 = gradientColors[i].second;
                Vec4F color1 = gradientColors[i + 1].second;

                color0.w *= canvasRendererAlpha;
                color1.w *= canvasRendererAlpha;

                F32 x0 = p0;
                F32 x1 = p1;

                positions.emplace_back(Vec3F(x1, 1.0f, 0.0f));    // Top right
                positions.emplace_back(Vec3F(x1, 0.0f, 0.0f));      // Bottom right
                positions.emplace_back(Vec3F(x0, 0.0f, 0.0f));      // Bottom left
                positions.emplace_back(Vec3F(x0, 1.0f, 0.0f));    // Top left

                normals.emplace_back(Vec3F(+0.0f, +0.0f, +1.0f));   // Top right
                normals.emplace_back(Vec3F(+0.0f, +0.0f, +1.0f));   // Bottom right
                normals.emplace_back(Vec3F(+0.0f, +0.0f, +1.0f));   // Bottom left
                normals.emplace_back(Vec3F(+0.0f, +0.0f, +1.0f));   // Top left                    

                if (gradient.getMode() == ColorGradient::EvaluateMode::Fixed)
                {
                    colors.emplace_back(color0);    // Top right
                    colors.emplace_back(color0);    // Bottom right
                    colors.emplace_back(color0);    // Bottom left
                    colors.emplace_back(color0);    // Top left
                }
                else
                {
                    colors.emplace_back(color1);    // Top right
                    colors.emplace_back(color1);    // Bottom right
                    colors.emplace_back(color0);    // Bottom left
                    colors.emplace_back(color0);    // Top left
                }

                uvs.emplace_back(Vec2F(uv.z, uv.w));    // Top right
                uvs.emplace_back(Vec2F(uv.z, uv.y));    // Bottom right
                uvs.emplace_back(Vec2F(uv.x, uv.y));    // Bottom left
                uvs.emplace_back(Vec2F(uv.x, uv.w));    // Top left

                U16 quadStart = (U16)i * 4;
                indices.emplace_back(0 + quadStart);
                indices.emplace_back(1 + quadStart);
                indices.emplace_back(3 + quadStart);
                indices.emplace_back(1 + quadStart);
                indices.emplace_back(2 + quadStart);
                indices.emplace_back(3 + quadStart);
            }

            subMesh->setPositions(positions);
            subMesh->setNormals(normals);
            subMesh->setColors(colors);
            subMesh->setTexCoords(0, uvs);

            subMesh->setIndices(indices);

            mesh->addSubMesh(subMesh);

            RenderMeshPtr renderMesh = getRenderTarget()->createRenderMeshFromPool();
            renderMesh->loadFromMesh(mesh);
            m_gradientRenderer->getMeshRenderer()->setRenderMesh(renderMesh);
        }
        
        updateColorSliderTags();
        updateColorTagUI();
    }

    //////////////////////////////////////////
    void SceneColorGradientPicker::notifyModeChanged(Dropdown2D* _dropdown, S32 _index)
    {
        ColorGradient gradient = ColorGradientPickerManager::GetInstancePtr()->getGradient();

        gradient.setMode((ColorGradient::EvaluateMode)_index);

        ColorGradientPickerManager::GetInstancePtr()->setGradient(gradient);
    }

    //////////////////////////////////////////
    ColorSliderTag2DPtr SceneColorGradientPicker::createColorSliderTag(bool _down)
    {
        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

        Vec2F size(9.0f, 14.0f);

        EntityPtr entity = createEntity();
        entity->ensureComponent<StaticName>("ClickButton");

        ColorSliderTag2DPtr colorSliderTag = entity->createComponent<ColorSliderTag2D>();
        colorSliderTag->getTransform()->setSize(size);
        colorSliderTag->getTransform()->setParent(m_gradientRenderer->getTransform());
        
        SpriteRenderer2DPtr colorRenderer = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::ColorSliderTagBody),
            size,
            Vec2F::c_zero,
            renderSystem->getMaterialManager()->getSpriteMaterial(),
            colorSliderTag->getTransform(),
            this);
        colorSliderTag->setColorRenderer(colorRenderer);

        ToggleButton2DPtr button = UIHelper::CreateToggleButton(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::ColorSliderTagFrame),
            size,
            Vec2F::c_zero,
            colorSliderTag->getTransform(),
            this,
            Vec2F(0.5f, 0.5f),
            Vec2F(0.5f, 0.5f),
            ColorU32{ 100, 100, 100 },
            ColorU32{ 30, 175, 255 },
            ColorU32{ 0, 145, 255 },
            ColorU32{ 30, 175, 255 });
        button->setCheckByClick(false);
        colorSliderTag->setToggleButton(button);


        if (_down)
        {
            colorSliderTag->getTransform()->setPivot(0.5f, 0.0f);
        }
        else
        {
            colorSliderTag->getTransform()->setPivot(0.5f, 1.0f);
            colorRenderer->getTransform()->setLocalScale({ 1.0f, -1.0f });
            button->getTransitionSprite()->getTransform()->setLocalScale({ 1.0f, -1.0f });
        }

        return colorSliderTag;
    }

    //////////////////////////////////////////
    void SceneColorGradientPicker::updateColorSliderTags()
    {
        ColorGradient gradient = ColorGradientPickerManager::GetInstancePtr()->getGradient();
        gradient.clamp01();

        // Alpha
        Size alphaTagsCount = gradient.getKeysAlpha().size();

        while (alphaTagsCount < m_alphaTags.size())
        {
            m_alphaTags.back()->getToggleButton()->eventClick.unsubscribe(this);
            m_alphaTags.back()->getToggleButton()->eventPressedChanged.unsubscribe(this);
            m_alphaTags.back()->getToggleButton()->eventCursorPressIn.unsubscribe(this);
            m_alphaTags.back()->getEntityRaw()->removeFromEcsWorld();
            m_alphaTags.pop_back();
        }

        while (alphaTagsCount > m_alphaTags.size())
        {
            ColorSliderTag2DPtr alphaSliderTag = createColorSliderTag(true);
            alphaSliderTag->getTransform()->setAnchor(0.0f, 1.0f);
            alphaSliderTag->getToggleButton()->eventClick.subscribe(this, &SceneColorGradientPicker::notifyColorSliderTagClick);
            alphaSliderTag->getToggleButton()->eventPressedChanged.subscribe(this, &SceneColorGradientPicker::notifyColorSliderTagPressedChanged);
            alphaSliderTag->getToggleButton()->eventCursorPressIn.subscribe(this, &SceneColorGradientPicker::notifyColorSliderTagCursorPressIn);
            m_alphaTags.push_back(alphaSliderTag);
        }

        for (Size i = 0; i < alphaTagsCount; ++i)
        {
            ColorGradient::KeyframeAlpha const& keyframe = gradient.getKeysAlpha()[i];

            ColorSliderTag2DPtr const& alphaSliderTag = m_alphaTags[i];
            alphaSliderTag->getTransform()->setLocalX(keyframe.time * m_gradientRenderer->getTransform()->getWidth());
            alphaSliderTag->getTransform()->setLocalY(1.0f);
            alphaSliderTag->getColorRenderer()->setColor(
                ColorU32::Lerp(ColorU32::c_black, ColorU32::c_white, keyframe.value));

            if (m_editMode == EditMode::AlphaTag)
                alphaSliderTag->getToggleButton()->setChecked(i == m_colorTagIndex);
            else
                alphaSliderTag->getToggleButton()->setChecked(false);
        }


        // RGB
        Size colorTagsCount = gradient.getKeysRGB().size();

        while (colorTagsCount < m_colorTags.size())
        {
            m_colorTags.back()->getToggleButton()->eventClick.unsubscribe(this);
            m_colorTags.back()->getToggleButton()->eventPressedChanged.unsubscribe(this);
            m_colorTags.back()->getToggleButton()->eventCursorPressIn.unsubscribe(this);
            m_colorTags.back()->getEntityRaw()->removeFromEcsWorld();
            m_colorTags.pop_back();
        }

        while (colorTagsCount > m_colorTags.size())
        {
            ColorSliderTag2DPtr colorSliderTag = createColorSliderTag(false);
            colorSliderTag->getTransform()->setAnchor(0.0f, 0.0f);
            colorSliderTag->getToggleButton()->eventClick.subscribe(this, &SceneColorGradientPicker::notifyColorSliderTagClick);
            colorSliderTag->getToggleButton()->eventPressedChanged.subscribe(this, &SceneColorGradientPicker::notifyColorSliderTagPressedChanged);
            colorSliderTag->getToggleButton()->eventCursorPressIn.subscribe(this, &SceneColorGradientPicker::notifyColorSliderTagCursorPressIn);
            m_colorTags.push_back(colorSliderTag);
        }

        for (Size i = 0; i < colorTagsCount; ++i)
        {
            ColorGradient::KeyframeRGB const& keyframe = gradient.getKeysRGB()[i];

            ColorSliderTag2DPtr const& colorSliderTag = m_colorTags[i];
            colorSliderTag->getTransform()->setLocalX(keyframe.time * m_gradientRenderer->getTransform()->getWidth());
            colorSliderTag->getTransform()->setLocalY(-1.0f);

            colorSliderTag->getColorRenderer()->setColor(ColorU32::FromVec4F32(keyframe.value));

            if (m_editMode == EditMode::ColorTag)
                colorSliderTag->getToggleButton()->setChecked(i == m_colorTagIndex);
            else
                colorSliderTag->getToggleButton()->setChecked(false);
        }
        
    }

    //////////////////////////////////////////
    void SceneColorGradientPicker::notifyColorSliderTagClick(Button2D* _button, CursorInputEvent& _event)
    {
        
    }

    //////////////////////////////////////////
    void SceneColorGradientPicker::notifyColorSliderTagPressedChanged(Button2D* _button, bool _pressed)
    {
        
    }

    //////////////////////////////////////////
    void SceneColorGradientPicker::notifyColorSliderTagCursorPressIn(Button2D* _button, Vec2F const& _positionOS, CursorInputEvent& _event)
    {
        if (m_draggingTag)
            return;

        Vector<ColorSliderTag2DPtr>::iterator it =
            std::find_if(
                m_alphaTags.begin(),
                m_alphaTags.end(),
                [_button](ColorSliderTag2DPtr const& _tag)
                {
                    if (_tag->getToggleButton().get() == _button)
                        return true;

                    return false;
                });

        if (it != m_alphaTags.end())
        {
            Size index = it - m_alphaTags.begin();

            processAlphaColorSliderTagClick(index, _event.button);

            return;
        }

        it =
            std::find_if(
                m_colorTags.begin(),
                m_colorTags.end(),
                [_button](ColorSliderTag2DPtr const& _tag)
                {
                    if (_tag->getToggleButton().get() == _button)
                        return true;

                    return false;
                });

        if (it != m_colorTags.end())
        {
            Size index = it - m_colorTags.begin();

            processRGBColorSliderTagClick(index, _event.button);            

            return;
        }
    }

    //////////////////////////////////////////
    void SceneColorGradientPicker::processAlphaColorSliderTagClick(Size _index, S32 _buttonId)
    {
        if (_buttonId == 0)
        {
            setEditMode(EditMode::AlphaTag);
            m_colorTagIndex = _index;

            updateColorSliderTags();
            updateColorTagUI();

            m_draggingTag = true;
            m_draggingTagDistance = 0.0f;

            Vec2F const& cursorPosition = InputManager::GetInstancePtr()->getCursorPosition(0);

            m_gradientRenderer->getTransform()->getWorldTransform().inversed().transform(
                cursorPosition,
                m_draggingTagPrevCursorPositionGradientSpace);
        }
        else
        if (_buttonId == 1)
        {
            ColorGradient gradient = ColorGradientPickerManager::GetInstancePtr()->getGradient();

            if (_index >= 0 && _index < gradient.getKeysAlpha().size())
            {
                if (gradient.getKeysAlpha().size() > 1)
                {
                    setEditMode(EditMode::None);
                    m_colorTagIndex = -1;

                    gradient.removeKeyAlpha(_index);

                    ColorGradientPickerManager::GetInstancePtr()->setGradient(gradient);
                }
            }
        }
    }

    //////////////////////////////////////////
    void SceneColorGradientPicker::processRGBColorSliderTagClick(Size _index, S32 _buttonId)
    {
        if (_buttonId == 0)
        {
            setEditMode(EditMode::ColorTag);
            m_colorTagIndex = _index;

            updateColorSliderTags();
            updateColorTagUI();

            m_draggingTag = true;
            m_draggingTagDistance = 0.0f;

            Vec2F const& cursorPosition = InputManager::GetInstancePtr()->getCursorPosition(0);

            m_gradientRenderer->getTransform()->getWorldTransform().inversed().transform(
                cursorPosition,
                m_draggingTagPrevCursorPositionGradientSpace);
        }
        else
        if (_buttonId == 1)
        {
            ColorGradient gradient = ColorGradientPickerManager::GetInstancePtr()->getGradient();

            if (_index >= 0 && _index < gradient.getKeysRGB().size())
            {
                if (gradient.getKeysRGB().size() > 1)
                {
                    setEditMode(EditMode::None);
                    m_colorTagIndex = -1;

                    gradient.removeKeyRGB(_index);

                    ColorGradientPickerManager::GetInstancePtr()->setGradient(gradient);
                }
            }
        }
    }

    //////////////////////////////////////////
    void SceneColorGradientPicker::setEditMode(EditMode _value)
    {
        if (m_editMode == _value)
            return;

        m_editMode = _value;

        updateEditModeUI();
    }

    //////////////////////////////////////////
    void SceneColorGradientPicker::updateEditModeUI()
    {
        bool  alphaHolderVisible = false;
        bool  colorHolderVisible = false;
        bool  locationHolderVisible = false;

        switch (m_editMode)
        {    
            case EditMode::AlphaTag:
            {
                alphaHolderVisible = true;
                locationHolderVisible = true;
                break;
            }
            case EditMode::ColorTag:
            {
                colorHolderVisible = true;
                locationHolderVisible = true;
                break;
            }
            default:
            {
                break;
            }
        }

        m_alphaHolder->setActiveSelf(alphaHolderVisible);
        m_colorHolder->setActiveSelf(colorHolderVisible);
        m_locationHolder->setActiveSelf(locationHolderVisible);
    }

    //////////////////////////////////////////
    void SceneColorGradientPicker::updateColorTagUI()
    {
        if (m_colorTagIndex < 0)
            return;

        ColorGradient const& gradient = ColorGradientPickerManager::GetInstancePtr()->getGradient();


        if (m_editMode == EditMode::AlphaTag)
        {
            if (m_colorTagIndex < gradient.getKeysAlpha().size())
            {
                ColorGradient::KeyframeAlpha const& keyframe = gradient.getKeysAlpha()[m_colorTagIndex];

                if (!m_alphaEdit->getSelected())
                    m_alphaEdit->setText(StringHelper::ToString((S32)(keyframe.value * 255.0f)));

                if (!m_alphaSlider->getSelected())
                    m_alphaSlider->setValue(keyframe.value);

                if (!m_locationEdit->getSelected())
                    m_locationEdit->setText(StringHelper::F32ToStringFormatted(keyframe.time * 100.0f, 1));
            }
        }
        else
        if (m_editMode == EditMode::ColorTag)
        {
            if (m_colorTagIndex < gradient.getKeysRGB().size())
            {
                ColorGradient::KeyframeRGB const& keyframe = gradient.getKeysRGB()[m_colorTagIndex];

                m_colorEdit->setColor(ColorF128(keyframe.value));

                if (!m_locationEdit->getSelected())
                    m_locationEdit->setText(StringHelper::F32ToStringFormatted(keyframe.time * 100.0f, 1));
            }
        }
    }

    //////////////////////////////////////////
    void SceneColorGradientPicker::notifyColorEditColorChanged(ColorHDREdit2D* _edit, ColorF128 const& _color)
    {
        if (m_colorTagIndex < 0)
            return;

        if (m_editMode == EditMode::ColorTag)
        {
            ColorGradient gradient = ColorGradientPickerManager::GetInstancePtr()->getGradient();

            if (m_colorTagIndex < gradient.getKeysRGB().size())
            {
                gradient.setKeyRGB(m_colorTagIndex, { _color.r, _color.g, _color.b });
                
                ColorGradientPickerManager::GetInstancePtr()->setGradient(gradient);
            }
        }
    }

    //////////////////////////////////////////
    void SceneColorGradientPicker::notifyAlphaSliderValueChanged(Slider2D* _slider, F32 _value)
    {
        if (m_colorTagIndex < 0)
            return;

        if (m_editMode == EditMode::AlphaTag)
        {
            ColorGradient gradient = ColorGradientPickerManager::GetInstancePtr()->getGradient();

            if (m_colorTagIndex < gradient.getKeysAlpha().size())
            {
                gradient.setKeyAlpha(m_colorTagIndex, _value);

                ColorGradientPickerManager::GetInstancePtr()->setGradient(gradient);
            }
        }
    }

    //////////////////////////////////////////
    void SceneColorGradientPicker::notifyAlphaEditTextInput(EditBox2D* _edit)
    {
        if (m_colorTagIndex < 0)
            return;

        if (m_editMode == EditMode::AlphaTag)
        {
            ColorGradient gradient = ColorGradientPickerManager::GetInstancePtr()->getGradient();

            if (m_colorTagIndex < gradient.getKeysAlpha().size())
            {
                _edit->setText(StringHelper::ToString(Math::Clamp(StringHelper::StringToS32(_edit->getText()), 0, 255)));

                F32 value = StringHelper::StringToF32(_edit->getText()) / 255.0f;

                gradient.setKeyAlpha(m_colorTagIndex, value);

                ColorGradientPickerManager::GetInstancePtr()->setGradient(gradient);
            }
        }
    }

    //////////////////////////////////////////
    void SceneColorGradientPicker::notifyLocationEditTextInput(EditBox2D* _edit)
    {
        if (m_colorTagIndex < 0)
            return;

        if (m_editMode == EditMode::None)
            return;
        
        ColorGradient gradient = ColorGradientPickerManager::GetInstancePtr()->getGradient();

        if (m_editMode == EditMode::AlphaTag)
        {
            if (m_colorTagIndex < gradient.getKeysAlpha().size())
            {
                if (StringHelper::IsFloatNumber(_edit->getText()))
                    _edit->setText(StringHelper::F32ToStringFormatted(Math::Clamp(StringHelper::StringToF32(_edit->getText()), 0.0f, 100.0f), 1));
                else
                    _edit->setText("0");

                F32 newTime = StringHelper::StringToF32(_edit->getText()) / 100.0f;

                changeCurrentAlphaTagTime(newTime);
            }
        }
        else
        if (m_editMode == EditMode::ColorTag)
        {
            if (m_colorTagIndex < gradient.getKeysRGB().size())
            {
                if (StringHelper::IsFloatNumber(_edit->getText()))
                    _edit->setText(StringHelper::F32ToStringFormatted(Math::Clamp(StringHelper::StringToF32(_edit->getText()), 0.0f, 100.0f), 1));
                else
                    _edit->setText("0");

                F32 newTime = StringHelper::StringToF32(_edit->getText()) / 100.0f;

                changeCurrentColorTagTime(newTime);
            }
        }
    }

    //////////////////////////////////////////
    void SceneColorGradientPicker::changeCurrentAlphaTagTime(F32 _newTime)
    {
        ColorGradient gradient = ColorGradientPickerManager::GetInstancePtr()->getGradient();

        // Fix key collision
        bool fixDirection = (_newTime >= 0.5f);
        while (gradient.hasKeyAlphaTime(_newTime))
        {
            if (fixDirection)
                _newTime -= std::numeric_limits<F32>::epsilon();
            else
                _newTime += std::numeric_limits<F32>::epsilon();
        }

        F32 prevTime = gradient.getKeysAlpha()[m_colorTagIndex].time;
        Size insertionIndex = gradient.getInsertionIndexAlpha(_newTime);

        gradient.setKeyAlphaTime(m_colorTagIndex, _newTime);

        if (_newTime < prevTime)
            m_colorTagIndex = insertionIndex;
        else
            m_colorTagIndex = insertionIndex - 1;

        updateColorTagUI();

        ColorGradientPickerManager::GetInstancePtr()->setGradient(gradient);
    }

    //////////////////////////////////////////
    void SceneColorGradientPicker::changeCurrentColorTagTime(F32 _newTime)
    {
        ColorGradient gradient = ColorGradientPickerManager::GetInstancePtr()->getGradient();

        // Fix key collision
        bool fixDirection = (_newTime >= 0.5f);
        while (gradient.hasKeyRGBTime(_newTime))
        {
            if (fixDirection)
                _newTime -= std::numeric_limits<F32>::epsilon();
            else
                _newTime += std::numeric_limits<F32>::epsilon();
        }

        F32 prevTime = gradient.getKeysRGB()[m_colorTagIndex].time;
        Vec3F prevValue = gradient.getKeysRGB()[m_colorTagIndex].value;
        Size insertionIndex = gradient.getInsertionIndexRGB(_newTime);

        gradient.setKeyRGBTime(m_colorTagIndex, _newTime);

        if (_newTime < prevTime)
            m_colorTagIndex = insertionIndex;
        else
            m_colorTagIndex = insertionIndex - 1;

        updateColorTagUI();

        ColorGradientPickerManager::GetInstancePtr()->setGradient(gradient);
    }

    //////////////////////////////////////////
    void SceneColorGradientPicker::notifyCanvasCursorRelease(CursorInputEvent& _event)
    {
        m_draggingTag = false;
        m_draggingTagDistance = 0.0f;
    }

    //////////////////////////////////////////
    void SceneColorGradientPicker::notifyGradientClickButtonClick(Button2D* _button, CursorInputEvent& _event)
    {
        if (_event.button == 0)
        {
            if ((_event.position - m_gradientClickPosition).squaredLength() <= 2.0f * 2.0f)
            {
                ColorGradient gradient = ColorGradientPickerManager::GetInstancePtr()->getGradient();

                Vec2F localPosition = m_gradientRenderer->getTransform()->getWorldTransform().inversed().transform(_event.position);

                F32 t = Math::Clamp01(localPosition.x / m_gradientRenderer->getTransform()->getWidth());

                F32 y = Math::Clamp01(localPosition.y / m_gradientRenderer->getTransform()->getHeight());

                if (y <= 0.5f)
                {
                    gradient.addKeyRGB(t, gradient.evaluateRGB(t));

                    for (Size i = 0, in = gradient.getKeysRGB().size(); i < in; ++i)
                        if (gradient.getKeysRGB()[i].time == t)
                        {
                            setEditMode(EditMode::ColorTag);
                            m_colorTagIndex = i;
                        }
                }
                else
                {
                    gradient.addKeyAlpha(t, gradient.evaluateAlpha(t));

                    for (Size i = 0, in = gradient.getKeysAlpha().size(); i < in; ++i)
                        if (gradient.getKeysAlpha()[i].time == t)
                        {
                            setEditMode(EditMode::AlphaTag);
                            m_colorTagIndex = i;
                        }
                }

                ColorGradientPickerManager::GetInstancePtr()->setGradient(gradient);
            }
        }
    }

    //////////////////////////////////////////
    void SceneColorGradientPicker::notifyGradientClickButtonCursorPressIn(Vec2F const& _positionOS, CursorInputEvent& _event)
    {
        m_gradientClickPosition = _event.position;
    }

} // namespace Maze
//////////////////////////////////////////
