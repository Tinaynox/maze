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
#include "MazeUIHeader.hpp"
#include "maze-ui/curve-editor/MazeSceneCurveEditor.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSystemManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
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
#include "maze-ui/managers/MazeAnimationCurveManager.hpp"
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
    static F32 const c_leftOffset = 40.0f;
    static F32 const c_rightOffset = 25.0f;
    static F32 const c_bottomOffset = 25.0f;
    static F32 const c_topOffset = 25.0f;
    static F32 const c_tangentEpsilon = 0.001f;


    //////////////////////////////////////////
    // Class SceneCurveEditor
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneCurveEditor, ECSRenderScene);

    //////////////////////////////////////////
    SceneCurveEditor::SceneCurveEditor()
    {
    }

    //////////////////////////////////////////
    SceneCurveEditor::~SceneCurveEditor()
    {
        if (AnimationCurveManager::GetInstancePtr())
            AnimationCurveManager::GetInstancePtr()->eventCurveChanged.unsubscribe(this);

        if (m_modeDropdown)
        {
            m_modeDropdown->eventValueChanged.unsubscribe(this);
        }

        if (m_minMaxModeDropdown)
        {
            m_minMaxModeDropdown->eventValueChanged.unsubscribe(this);
        }

        if (m_curveClickButton)
        {
            m_curveClickButton->eventClick.unsubscribe(this);
            m_curveClickButton->getUIElement()->eventCursorPressIn.unsubscribe(this);
        }

        if (m_curveScalarEdit)
            m_curveScalarEdit->eventTextInput.unsubscribe(this);

        if (m_canvasUIElement)
        {
            m_canvasUIElement->eventCursorRelease.unsubscribe(this);
        }

        if (m_valueEdit)
            m_valueEdit->eventTextInput.unsubscribe(this);

        if (m_locationEdit)
            m_locationEdit->eventTextInput.unsubscribe(this);

        if (m_inTangentEdit)
            m_inTangentEdit->eventTextInput.unsubscribe(this);

        if (m_outTangentEdit)
            m_outTangentEdit->eventTextInput.unsubscribe(this);

        m_canvas.reset();
    }

    //////////////////////////////////////////
    SceneCurveEditorPtr SceneCurveEditor::Create(RenderTargetPtr const& _renderTarget)
    {
        SceneCurveEditorPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneCurveEditor, object, init(_renderTarget));
        return object;
    }

    //////////////////////////////////////////
    bool SceneCurveEditor::init(RenderTargetPtr const& _renderTarget)
    {
        if (!ECSRenderScene::init(_renderTarget))
            return false;


        m_curvePixelSheet.setFormat(PixelFormat::RGBA_U8);

        m_curveTexture = Texture2D::Create();
        m_curveTexture->setMagFilter(TextureFilter::Nearest);
        m_curveTexture->setMinFilter(TextureFilter::Linear);

        m_curveSprite = Sprite::Create(m_curveTexture);

        create2D();
        
        AnimationCurveManager::GetInstancePtr()->eventCurveChanged.subscribe(this, &SceneCurveEditor::notifyCurveChanged);

        return true;
    }

    //////////////////////////////////////////
    void SceneCurveEditor::setup()
    {
        updateNormalization();
        updateUI();
    }

    //////////////////////////////////////////
    void SceneCurveEditor::updateNormalization()
    {
        AnimationCurve curve = AnimationCurveManager::GetInstancePtr()->getCurve();
        if (curve.getMinMaxMode() == AnimationCurveMinMaxMode::Normalized ||
            curve.getMinMaxMode() == AnimationCurveMinMaxMode::NormalizedPositive)
        {
            curve.normalize();
            if (curve != AnimationCurveManager::GetInstancePtr()->getCurve())
                AnimationCurveManager::GetInstancePtr()->setCurve(curve);
        }
    }

    //////////////////////////////////////////
    void SceneCurveEditor::update(F32 _dt)
    {
        processDragging();
        processCurve();

        AnimationCurve const& curve = AnimationCurveManager::GetInstancePtr()->getCurve();

        bool keyHolderVisible = (m_keyIndex != -1 && m_keyIndex < (S32)curve.getKeys().size());

        m_keyHolder->setActiveSelf(keyHolderVisible);

        if (keyHolderVisible)
        {
            AnimationCurve::Keyframe const& keyframe = curve.getKeys()[m_keyIndex];

            if (!m_valueEdit->getSelected())
            {
                F32 value = keyframe.value;
                switch (curve.getMinMaxMode())
                {
                    case AnimationCurveMinMaxMode::NormalizedPositive:
                        value *= curve.getScalar();
                        break;
                    case AnimationCurveMinMaxMode::Normalized:
                        value *= curve.getScalar();
                        break;
                    default:
                        break;
                }

                m_valueEdit->setText(StringHelper::F32ToStringFormatted(value, 4));
            }

            if (!m_locationEdit->getSelected())
                m_locationEdit->setText(StringHelper::F32ToStringFormatted(keyframe.time * 100.0f, 1));

            bool tangentsHoldersVisible = (curve.getMode() == AnimationCurve::EvaluateMode::Smooth);
            m_tangentsHolder->setActiveSelf(tangentsHoldersVisible);

            if (tangentsHoldersVisible)
            {
                if (!m_inTangentEdit->getSelected())
                    m_inTangentEdit->setText(StringHelper::F32ToStringFormatted(keyframe.inTangent, 2));

                if (!m_outTangentEdit->getSelected())
                    m_outTangentEdit->setText(StringHelper::F32ToStringFormatted(keyframe.outTangent, 2));
            }
        }
    }


    //////////////////////////////////////////
    void SceneCurveEditor::create2D()
    {
        ColorU32 bandColor(176, 176, 176);

        EntityPtr canvasEntity = createEntity();
        m_canvas = canvasEntity->createComponent<Canvas>();
        m_canvas->setClearColorFlag(true);
        m_canvas->setClearDepthFlag(true);
        m_canvas->setClearColor(ColorU32(203, 203, 203, 255));
        m_canvas->setRenderTarget(m_renderTarget);
        m_canvasUIElement = canvasEntity->ensureComponent<UIElement2D>();
        m_canvasUIElement->eventCursorRelease.subscribe(this, &SceneCurveEditor::notifyCanvasCursorRelease);        

        AnimationCurve const& curve = AnimationCurveManager::GetInstancePtr()->getCurve();

        // Curve        
        {
            m_curveRendererHolder = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Frame01),
                Vec2DF(400.0f, 218.0f),
                Vec2DF(0.0f, -50.0f),
                GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getColorTextureMaterial(),
                m_canvas->getTransform(),
                this,
                Vec2DF(0.0f, 1.0f),
                Vec2DF(0.0f, 1.0f));
            m_curveRendererHolder->setColor(bandColor);

            SpriteRenderer2DPtr chessRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::TransparentChess),
                m_curveRendererHolder->getTransform()->getSize() - Vec2DF(2.0f, 2.0f),
                Vec2DF(1.0f, 1.0f),
                GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getColorTextureMaterial(),
                m_curveRendererHolder->getTransform(),
                this,
                Vec2DF(0.0f, 0.0f),
                Vec2DF(0.0f, 0.0f));
            chessRenderer->getMaterial()->setUniform(
                "u_baseMapST",
                Vec4DF(
                    chessRenderer->getTransform()->getWidth() / 8.0f,
                    chessRenderer->getTransform()->getHeight() / 8.0f,
                    0.0f,
                    0.0f));

            m_curveRenderer = SpriteHelper::CreateSprite(
                m_curveSprite,
                m_curveRendererHolder->getTransform()->getSize() - Vec2DF(2.0f, 2.0f),
                Vec2DF(1.0f, 1.0f),
                GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getColorTextureMaterial(),
                m_curveRendererHolder->getTransform(),
                this,
                Vec2DF(0.0f, 0.0f),
                Vec2DF(0.0f, 0.0f));

            m_curveClickButton = UIHelper::CreateDefaultClickButton(
                nullptr,
                Vec2DF(m_curveRenderer->getTransform()->getWidth() + 8, m_curveRenderer->getTransform()->getHeight() + 30),
                Vec2DF::c_zero,
                m_curveRenderer->getTransform(),
                this);
            m_curveClickButton->eventClick.subscribe(this, &SceneCurveEditor::notifyCurveClickButtonClick);
            m_curveClickButton->getUIElement()->eventCursorPressIn.subscribe(this, &SceneCurveEditor::notifyCurveClickButtonCursorPressIn);
            m_curveClickButton->setNormalColor(ColorU32::c_transparent);
            m_curveClickButton->setFocusedColor(ColorU32::c_transparent);
            m_curveClickButton->setPressedColor(ColorU32::c_transparent);
            m_curveClickButton->setSelectedColor(ColorU32::c_transparent);
            m_curveClickButton->getTransitionSprite()->setColor(ColorU32::c_transparent);


            m_curveScalarEdit = SystemUIHelper::CreateDefaultEditBox(
                StringHelper::F32ToStringFormatted(curve.getScalar(), 4).c_str(),
                Vec2DF(30, 12),
                Vec2DF(40 - 3, -25),
                m_curveRenderer->getTransform(),
                this,
                Vec2DF(0.0f, 1.0f),
                Vec2DF(1.0f, 0.5f));
            m_curveScalarEdit->setHorizontalAlignment(HorizontalAlignment2D::Right);
            m_curveScalarEdit->eventTextInput.subscribe(this, &SceneCurveEditor::notifyCurveScalarEditTextInput);
        }

        // Mode
        {
            HorizontalLayout2DPtr rowLayout = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                Vec2DF(336.0f, 18.0f),
                Vec2DF(12.0f, -10.0f),
                m_canvas->getTransform(),
                this,
                Vec2DF(0.0f, 1.0f),
                Vec2DF(0.0f, 1.0f));
            rowLayout->setExpand(false);
            rowLayout->setAutoWidth(false);
            rowLayout->setSpacing(5.0f);

            HorizontalLayout2DPtr row2Layout = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                Vec2DF(336.0f, 18.0f),
                Vec2DF(12.0f, -28.0f),
                m_canvas->getTransform(),
                this,
                Vec2DF(0.0f, 1.0f),
                Vec2DF(0.0f, 1.0f));
            row2Layout->setExpand(false);
            row2Layout->setAutoWidth(false);
            row2Layout->setSpacing(5.0f);

            SystemTextRenderer2DPtr label = SystemUIHelper::CreateSystemText(
                "Mode",
                8,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                { 80.0f, 18.0f },
                Vec2DF::c_zero,
                rowLayout->getTransform(),
                this);
            label->setColor(ColorU32::c_black);

            m_modeDropdown = SystemUIHelper::CreateDefaultDropdown(
                Vec2DF(188.0f, 18.0f),
                Vec2DF(0.0f, 0.0f),
                rowLayout->getTransform(),
                this);
            m_modeDropdown->eventValueChanged.subscribe(this, &SceneCurveEditor::notifyModeChanged);

            m_modeDropdown->addOption("Fixed");
            m_modeDropdown->addOption("Linear");
            m_modeDropdown->addOption("Smooth");

            m_modeDropdown->setValue((S32)curve.getMode());


            SystemTextRenderer2DPtr label2 = SystemUIHelper::CreateSystemText(
                "MinMax",
                8,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                { 80.0f, 18.0f },
                Vec2DF::c_zero,
                row2Layout->getTransform(),
                this);
            label2->setColor(ColorU32::c_black);

            m_minMaxModeDropdown = SystemUIHelper::CreateDefaultDropdown(
                Vec2DF(188.0f, 18.0f),
                Vec2DF(0.0f, 0.0f),
                row2Layout->getTransform(),
                this);
            m_minMaxModeDropdown->eventValueChanged.subscribe(this, &SceneCurveEditor::notifyMinMaxModeChanged);

            for (AnimationCurveMinMaxMode mode = AnimationCurveMinMaxMode(1); mode < AnimationCurveMinMaxMode::MAX; ++mode)
                m_minMaxModeDropdown->addOption(mode.toString());

            m_minMaxModeDropdown->setValue((S32)curve.getMinMaxMode() - 1);



            m_copyButton = UIHelper::CreateDefaultClickButton(
                "C",
                { 18.0f, 18.0f },
                { -32.0f, -12.0f },
                m_canvas->getTransform(),
                this,
                Vec2DF(1.0f, 1.0f),
                Vec2DF(1.0f, 1.0f));
            m_copyButton->eventClick.subscribe(
                [](Button2D* _button, CursorInputEvent const& _event)
            {
                AnimationCurve curve = AnimationCurveManager::GetInstancePtr()->getCurve();
                SystemManager::GetInstancePtr()->setClipboardString(curve.toString());
            });

            m_pasteButton = UIHelper::CreateDefaultClickButton(
                "P",
                { 18.0f, 18.0f },
                { -12.0f, -12.0f },
                m_canvas->getTransform(),
                this,
                Vec2DF(1.0f, 1.0f),
                Vec2DF(1.0f, 1.0f));
            m_pasteButton->eventClick.subscribe(
                [](Button2D* _button, CursorInputEvent const& _event)
            {
                String text = SystemManager::GetInstancePtr()->getClipboardAsString();
                AnimationCurve curve = AnimationCurve::FromString(text);
                AnimationCurveManager::GetInstancePtr()->setCurve(curve);

            });


            m_copyXMLButton = UIHelper::CreateDefaultClickButton(
                "CX",
                { 18.0f, 18.0f },
                { -72.0f, -12.0f },
                m_canvas->getTransform(),
                this,
                Vec2DF(1.0f, 1.0f),
                Vec2DF(1.0f, 1.0f));
            m_copyXMLButton->eventClick.subscribe(
                [](Button2D* _button, CursorInputEvent const& _event)
            {
                AnimationCurve curve = AnimationCurveManager::GetInstancePtr()->getCurve();
                String text = curve.toString();
                StringHelper::ReplaceSubstring(text, "\"", "&quot;");
                SystemManager::GetInstancePtr()->setClipboardString(text);
            });

            m_pasteXMLButton = UIHelper::CreateDefaultClickButton(
                "PX",
                { 18.0f, 18.0f },
                { -52.0f, -12.0f },
                m_canvas->getTransform(),
                this,
                Vec2DF(1.0f, 1.0f),
                Vec2DF(1.0f, 1.0f));
            m_pasteXMLButton->eventClick.subscribe(
                [](Button2D* _button, CursorInputEvent const& _event)
            {
                String text = SystemManager::GetInstancePtr()->getClipboardAsString();
                StringHelper::ReplaceSubstring(text, "&quot;", "\"");
                AnimationCurve curve = AnimationCurve::FromString(text);
                AnimationCurveManager::GetInstancePtr()->setCurve(curve);

            });


        }

        // Key
        {
            m_keyHolder = createEntity();
            Transform2DPtr keyHolderTransform = m_keyHolder->ensureComponent<Transform2D>();
            keyHolderTransform->setParent(m_canvas->getTransform());
            keyHolderTransform->setSize(m_canvas->getTransform()->getSize());
            keyHolderTransform->setAnchor(0.0f, 0.0f);
            keyHolderTransform->setPivot(0.0f, 0.0f);

            // Value
            {
                HorizontalLayout2DPtr rowLayout = UIHelper::CreateHorizontalLayout(
                    HorizontalAlignment2D::Left,
                    VerticalAlignment2D::Middle,
                    Vec2DF(135.0f, 18.0f),
                    Vec2DF(20.0f, -285.0f),
                    keyHolderTransform,
                    this,
                    Vec2DF(0.0f, 1.0f),
                    Vec2DF(0.0f, 1.0f));
                rowLayout->setExpand(true);
                rowLayout->setAutoWidth(false);

                SystemTextRenderer2DPtr label = SystemUIHelper::CreateSystemText(
                    "Value",
                    8,
                    HorizontalAlignment2D::Left,
                    VerticalAlignment2D::Middle,
                    { 8.0f * 8.0f, 18.0f },
                    Vec2DF::c_zero,
                    rowLayout->getTransform(),
                    this);
                label->setColor(ColorU32::c_black);

                m_valueEdit = SystemUIHelper::CreateDefaultEditBox(
                    "51.5",
                    Vec2DF(80.0f, 18.0f),
                    Vec2DF::c_zero,
                    rowLayout->getTransform(),
                    this);
                m_valueEdit->eventTextInput.subscribe(this, &SceneCurveEditor::notifyValueEditTextInput);
            }

            // Location
            {
                HorizontalLayout2DPtr rowLayout = UIHelper::CreateHorizontalLayout(
                    HorizontalAlignment2D::Left,
                    VerticalAlignment2D::Middle,
                    Vec2DF(135.0f, 18.0f),
                    Vec2DF(200.0f, -285.0f),
                    keyHolderTransform,
                    this,
                    Vec2DF(0.0f, 1.0f),
                    Vec2DF(0.0f, 1.0f));
                rowLayout->setExpand(true);
                rowLayout->setAutoWidth(false);

                SystemTextRenderer2DPtr label = SystemUIHelper::CreateSystemText(
                    "Location",
                    8,
                    HorizontalAlignment2D::Left,
                    VerticalAlignment2D::Middle,
                    { 8.0f * 8.0f, 18.0f },
                    Vec2DF::c_zero,
                    rowLayout->getTransform(),
                    this);
                label->setColor(ColorU32::c_black);

                m_locationEdit = SystemUIHelper::CreateDefaultEditBox(
                    "51.5",
                    Vec2DF(45.0f, 18.0f),
                    Vec2DF::c_zero,
                    rowLayout->getTransform(),
                    this);
                m_locationEdit->eventTextInput.subscribe(this, &SceneCurveEditor::notifyLocationEditTextInput);

                SystemTextRenderer2DPtr label2 = SystemUIHelper::CreateSystemText(
                    "%",
                    8,
                    HorizontalAlignment2D::Left,
                    VerticalAlignment2D::Middle,
                    { 8.0f, 18.0f },
                    Vec2DF::c_zero,
                    rowLayout->getTransform(),
                    this);
                label2->setColor(ColorU32::c_black);
            }

            m_tangentsHolder = createEntity();
            Transform2DPtr tangentsHolderTransform = m_tangentsHolder->ensureComponent<Transform2D>();
            tangentsHolderTransform->setParent(keyHolderTransform);
            tangentsHolderTransform->setSize(m_canvas->getTransform()->getSize());
            tangentsHolderTransform->setAnchor(0.0f, 0.0f);
            tangentsHolderTransform->setPivot(0.0f, 0.0f);

            // InTangent
            {
                HorizontalLayout2DPtr rowLayout = UIHelper::CreateHorizontalLayout(
                    HorizontalAlignment2D::Left,
                    VerticalAlignment2D::Middle,
                    Vec2DF(135.0f, 18.0f),
                    Vec2DF(20.0f, -305.0f),
                    tangentsHolderTransform,
                    this,
                    Vec2DF(0.0f, 1.0f),
                    Vec2DF(0.0f, 1.0f));
                rowLayout->setExpand(true);
                rowLayout->setAutoWidth(false);

                SystemTextRenderer2DPtr label = SystemUIHelper::CreateSystemText(
                    "In Tan",
                    8,
                    HorizontalAlignment2D::Left,
                    VerticalAlignment2D::Middle,
                    { 8.0f * 8.0f, 18.0f },
                    Vec2DF::c_zero,
                    rowLayout->getTransform(),
                    this);
                label->setColor(ColorU32::c_black);

                m_inTangentEdit = SystemUIHelper::CreateDefaultEditBox(
                    "51.5",
                    Vec2DF(80.0f, 18.0f),
                    Vec2DF::c_zero,
                    rowLayout->getTransform(),
                    this);
                m_inTangentEdit->eventTextInput.subscribe(this, &SceneCurveEditor::notifyInTangentEditTextInput);
            }

            // OutTangent
            {
                HorizontalLayout2DPtr rowLayout = UIHelper::CreateHorizontalLayout(
                    HorizontalAlignment2D::Left,
                    VerticalAlignment2D::Middle,
                    Vec2DF(135.0f, 18.0f),
                    Vec2DF(200.0f, -305.0f),
                    tangentsHolderTransform,
                    this,
                    Vec2DF(0.0f, 1.0f),
                    Vec2DF(0.0f, 1.0f));
                rowLayout->setExpand(true);
                rowLayout->setAutoWidth(false);

                SystemTextRenderer2DPtr label = SystemUIHelper::CreateSystemText(
                    "Out Tan",
                    8,
                    HorizontalAlignment2D::Left,
                    VerticalAlignment2D::Middle,
                    { 8.0f * 8.0f, 18.0f },
                    Vec2DF::c_zero,
                    rowLayout->getTransform(),
                    this);
                label->setColor(ColorU32::c_black);

                m_outTangentEdit = SystemUIHelper::CreateDefaultEditBox(
                    "51.5",
                    Vec2DF(80.0f, 18.0f),
                    Vec2DF::c_zero,
                    rowLayout->getTransform(),
                    this);
                m_outTangentEdit->eventTextInput.subscribe(this, &SceneCurveEditor::notifyOutTangentEditTextInput);
            }
        }

        updateUI();
    }

    //////////////////////////////////////////
    void SceneCurveEditor::notifyCurveChanged(AnimationCurve const& _curve)
    {
        setup();
    }

    //////////////////////////////////////////
    void SceneCurveEditor::updateUI()
    {
        if (m_state == ECSSceneState::Destroy)
            return;

        processCurve();
    }

    //////////////////////////////////////////
    void SceneCurveEditor::notifyModeChanged(Dropdown2D* _dropdown, S32 _index)
    {
        AnimationCurve curve = AnimationCurveManager::GetInstancePtr()->getCurve();

        curve.setMode((AnimationCurve::EvaluateMode)_index);

        AnimationCurveManager::GetInstancePtr()->setCurve(curve);
    }

    //////////////////////////////////////////
    void SceneCurveEditor::notifyMinMaxModeChanged(Dropdown2D* _dropdown, S32 _index)
    {
        AnimationCurve curve = AnimationCurveManager::GetInstancePtr()->getCurve();

        curve.setMinMaxMode((AnimationCurveMinMaxMode)(_index + 1));

        AnimationCurveManager::GetInstancePtr()->setCurve(curve);

        updateNormalization();
    }

    //////////////////////////////////////////
    void SceneCurveEditor::processCurve()
    {        
        F32 width = m_curveRenderer->getTransform()->getSize().x - c_leftOffset - c_rightOffset;
        F32 height = m_curveRenderer->getTransform()->getSize().y - c_bottomOffset - c_topOffset;

        F32 heightScalar = getCurveHeightScalar();
        Vec2DF origin = getCurveOrigin();


        F32 cellSize = width / 20.0f;

        m_curvePixelSheet.setSize(m_curveRenderer->getTransform()->getSize());
        AnimationCurve curve = AnimationCurveManager::GetInstancePtr()->getCurve();

        
        m_curvePixelSheet.fill(ColorU32(86, 86, 86));

        F32 startTime = curve.getStartTime();
        F32 endTime = curve.getEndTime();
        

        // Frame and grid
        {
            ColorU32 const mainLineColor(74, 74, 74);
            ColorU32 const secondaryLineColor(80, 80, 80);
            ColorU32 const thirdyLineColor(82, 82, 82);
            ColorU32 const frameLineColor(76, 76, 76);
            ColorU32 const labelsColor(64, 64, 64);

            // X
            m_curvePixelSheet.drawLine(
                (Vec2DS)origin, (Vec2DS)origin + Vec2DS((S32)width, 0),
                mainLineColor);

            // Vertical grid
            for (S32 i = 0; i < 20; ++i)
            {
                F32 x = c_leftOffset + (i + 1) * cellSize;

                m_curvePixelSheet.drawLine(
                    (S32)x, (S32)c_bottomOffset,
                    (S32)x, (S32)(c_bottomOffset + height),
                    (i + 1) == 10 ? mainLineColor
                    : ((i + 1) & 1) == 0 ? secondaryLineColor : thirdyLineColor);
            }

            // X labels
            for (S32 i = 0; i <= 10; ++i)
            {
                F32 f = (F32)i * 0.1f;

                String labelText = StringHelper::F32ToStringFormatted(f, 1);

                if (labelText.find(".") == String::npos)
                    labelText += ".0";

                F32 x = origin.x + cellSize * 2 * i/* - cellSize*/;
                m_curvePixelSheet.drawText(
                    (S32)x, (S32)(c_bottomOffset - 6),
                    labelText.c_str(),
                    labelsColor,
                    1,
                    1,
                    0);
            }

            m_curvePixelSheet.drawText(
                (S32)origin.x - 5, (S32)origin.y,
                "0",
                labelsColor,
                1,
                2,
                1);


            {                
                F32 delta = 0.0f;
                switch (curve.getMinMaxMode())
                {
                    case AnimationCurveMinMaxMode::NormalizedPositive:
                    {
                        delta = curve.getScalar() / 10.0f;
                        break;
                    }
                    case AnimationCurveMinMaxMode::Normalized:
                    {
                        delta = curve.getScalar() / 5.0f;
                        break;
                    }
                    default:
                    {
                        F32 deltaY = heightScalar / cellSize;
                        delta = curve.getScalar() / deltaY;
                        break;
                    }
                }

                S32 i = 0;
                F32 y = 0.0f;
                do
                {
                    F32 shift = (i + 1) * cellSize;

                    y = origin.y + shift;

                    m_curvePixelSheet.drawLine(
                        (S32)origin.x, (S32)y,
                        (S32)(origin.x + width), (S32)y,
                        ((i + 1) & 1) == 0 ? secondaryLineColor : thirdyLineColor);

                    F32 value = (F32)(i + 1) * delta;
                    
                    String valueStr;
                    if (delta >= 1.0f)
                        valueStr = StringHelper::S32ToString((S32)Math::Round(value));
                    else
                        valueStr = StringHelper::F32ToStringFormatted(value, 2);

                    m_curvePixelSheet.drawText(
                        (S32)origin.x - 5, (S32)y,
                        valueStr.c_str(),
                        labelsColor,
                        1,
                        2,
                        1);

                    if (curve.getMinMaxMode() == AnimationCurveMinMaxMode::Normalized)
                    {
                        y = origin.y - shift;

                        m_curvePixelSheet.drawLine(
                            (S32)origin.x, (S32)y,
                            (S32)(origin.x + width), (S32)y,
                            ((i + 1) & 1) == 0 ? secondaryLineColor : thirdyLineColor);

                        m_curvePixelSheet.drawText(
                            (S32)origin.x - 5, (S32)y,
                            ("-" + valueStr).c_str(),
                            labelsColor,
                            1,
                            2,
                            1);
                    }

                    ++i;
                } while ((i + 1) * cellSize < heightScalar);
            }

            if (curve.getMinMaxMode() == AnimationCurveMinMaxMode::Normalized)
            {
                m_curvePixelSheet.drawText(
                    (S32)origin.x - 5, (S32)c_bottomOffset,
                    StringHelper::F32ToStringFormatted(-(F32)curve.getScalar(), 2).c_str(),
                    labelsColor,
                    1,
                    2,
                    1);
            }

            m_curvePixelSheet.drawRect(
                (S32)c_leftOffset, (S32)c_bottomOffset,
                (S32)width, (S32)height,
                frameLineColor);

            F32 dt = 1.0f / (width * 4);
            for (F32 x = 0.0f; x < (1.0f - dt); x += dt)
            {
                F32 x0 = x;
                F32 x1 = x + dt;

                F32 y0 = curve.evaluateUnscaled(x0);
                F32 y1 = curve.evaluateUnscaled(x1);

                if (Math::Abs(y0) >= 10000 || Math::Abs(y1) >= 10000)
                    continue;

                ColorU32 color = ColorU32::c_green;

                if (x1 <= startTime || x0 >= endTime)
                    color = ColorU32(0, 128, 0);

                m_curvePixelSheet.drawLine(
                    (S32)(origin.x + x0 * width), (S32)(origin.y + y0 * heightScalar),
                    (S32)(origin.x + x1 * width), (S32)(origin.y + y1 * heightScalar),
                    color);
            }

            

            S32 focusedKey;
            CurveElement curveElement = getFocusedElement(focusedKey);

            for (S32 i = 0; i < (S32)curve.getKeys().size(); ++i)
            {
                AnimationCurve::Keyframe const& keyframe = curve.getKeys()[i];

                F32 x = keyframe.time;
                F32 y = keyframe.value;

                Vec2DF point(
                    origin.x + x * width,
                    origin.y + y * heightScalar);

                bool focused = (focusedKey == i);
                bool selected = (m_keyIndex == i);

                // Tangents
                if (i == m_keyIndex && curve.getMode() == AnimationCurve::EvaluateMode::Smooth)
                {
                    // auto tangentIn = [y, x, keyframe](F32 _x) { return y + keyframe.inTangent * (_x - x); };
                    // auto tangentOut = [y, x, keyframe](F32 _x) { return y + keyframe.outTangent * (_x - x); };

                    if (i > 0)
                    {
                        /*
                        Vec2DF inTangentDirection(
                            (-1.0f) * width,
                            (tangentIn(x - 1.0f) - y) * heightScalar);
                        inTangentDirection.normalize();
                        Vec2DF inTangentPoint = point + inTangentDirection * cellSize * 2;
                        */
                        Vec2DF inTangentPoint = getInTangentPoint(i);

                        if (!inTangentPoint.isNaN())
                        {
                            m_curvePixelSheet.drawLine(
                                (S32)(point.x), (S32)(point.y),
                                (S32)(inTangentPoint.x), (S32)(inTangentPoint.y),
                                ColorU32(147, 147, 147));

                            m_curvePixelSheet.drawFilledCircle(
                                (Vec2DS)inTangentPoint,
                                2,
                                ColorU32(156, 156, 156));

                            bool tangentPointSelected =
                                (m_draggingElement == CurveElement::InTangent) ||
                                (curveElement == CurveElement::InTangent);


                            ColorU32 circleColor;
                            if (tangentPointSelected)
                            {
                                circleColor = ColorU32(220, 220, 220);
                            }
                            else
                            {
                                circleColor = ColorU32(30, 30, 30);
                            }
                            m_curvePixelSheet.drawCircle(
                                (Vec2DS)inTangentPoint,
                                2,
                                circleColor);

                            ColorU32 filledCircleColor;
                            if (tangentPointSelected)
                            {
                                filledCircleColor = ColorU32(200, 200, 200);
                            }
                            else
                            {
                                filledCircleColor = ColorU32(50, 50, 50);
                            }
                            m_curvePixelSheet.drawFilledCircle(
                                (Vec2DS)inTangentPoint,
                                1,
                                filledCircleColor);
                        }
                    }

                    if (i < (S32)curve.getKeys().size() - 1)
                    {
                        Vec2DF outTangentPoint = getOutTangentPoint(i);

                        if (!outTangentPoint.isNaN())
                        {
                            m_curvePixelSheet.drawLine(
                                (S32)(point.x), (S32)(point.y),
                                (S32)(outTangentPoint.x), (S32)(outTangentPoint.y),
                                ColorU32(147, 147, 147));

                            m_curvePixelSheet.drawFilledCircle(
                                (Vec2DS)outTangentPoint,
                                2,
                                ColorU32(156, 156, 156));

                            bool tangentPointSelected =
                                (m_draggingElement == CurveElement::OutTangent) ||
                                (curveElement == CurveElement::OutTangent);

                            ColorU32 circleColor;
                            if (tangentPointSelected)
                            {
                                circleColor = ColorU32(220, 220, 220);
                            }
                            else
                            {
                                circleColor = ColorU32(30, 30, 30);
                            }
                            m_curvePixelSheet.drawCircle(
                                (Vec2DS)outTangentPoint,
                                2,
                                circleColor);

                            ColorU32 filledCircleColor;
                            if (tangentPointSelected)
                            {
                                filledCircleColor = ColorU32(200, 200, 200);
                            }
                            else
                            {
                                filledCircleColor = ColorU32(50, 50, 50);
                            }
                            m_curvePixelSheet.drawFilledCircle(
                                (Vec2DS)outTangentPoint,
                                1,
                                filledCircleColor);
                        }
                    }
                }


                m_curvePixelSheet.drawFilledCircle(
                    (Vec2DS)point,
                    3,
                    ColorU32::c_green);

                ColorU32 circleColor;
                if (selected)
                {
                    circleColor = ColorU32::c_white;
                }
                else
                if (focused)
                {
                    circleColor = ColorU32::c_gray;
                }
                else
                {
                    circleColor = ColorU32::c_black;
                }

                m_curvePixelSheet.drawCircle(
                    (Vec2DS)point,
                    3,
                    circleColor);

                ColorU32 filledCircleColor;
                if (selected)
                {
                    filledCircleColor = ColorU32::c_white;
                }
                else
                if (focused)
                {
                    filledCircleColor = ColorU32::c_darkGray;
                }
                else
                {
                    filledCircleColor = ColorU32::c_black;
                }

                m_curvePixelSheet.drawFilledCircle(
                    (Vec2DS)point,
                    1,
                    filledCircleColor);
            }
        }

        if (!m_curveScalarEdit->getSelected())
            m_curveScalarEdit->setText(StringHelper::F32ToStringFormatted(curve.getScalar()));

        m_curveTexture->loadTexture(m_curvePixelSheet);
        m_curveSprite->set(m_curveTexture);
        
        m_modeDropdown->setValue((S32)curve.getMode());
        m_minMaxModeDropdown->setValue((S32)curve.getMinMaxMode() - 1);
    }

    //////////////////////////////////////////
    void SceneCurveEditor::notifyCurveClickButtonClick(Button2D* _button, CursorInputEvent const& _event)
    {
        if (_event.button == 0)
        {
            if ((_event.position - m_curveClickPosition).squaredLength() <= 2.0f * 2.0f)
            {
                S32 focusedKey;
                CurveElement curveElement = getFocusedElement(focusedKey);

                if (curveElement == CurveElement::None)
                {
                    F32 width = m_curveRenderer->getTransform()->getSize().x - c_leftOffset - c_rightOffset;

                    F32 heightScalar = getCurveHeightScalar();
                    Vec2DF origin = getCurveOrigin();

                    AnimationCurve curve = AnimationCurveManager::GetInstancePtr()->getCurve();

                    Vec2DF const& cursorPosition = InputManager::GetInstancePtr()->getCursorPosition(0);

                    Vec2DF cursorPositionCurveSpace;
                    m_curveRenderer->getTransform()->getWorldTransform().inversedAffineCopy().transformAffine(
                        cursorPosition,
                        cursorPositionCurveSpace);

                    F32 x = cursorPositionCurveSpace.x - origin.x;
                    F32 y = cursorPositionCurveSpace.y - origin.y;

                    x /= width;
                    y /= heightScalar;

                    if (   x >= 0.0f && x <= 1.0f
                        && y >= -1.0f && y <= 1.0f)
                    {
                        S32 insertionIndex = (S32)curve.getInsertionIndex(x);

                        F32 inTangent = 0.0f;
                        F32 outTangent = 0.0f;

                        if (insertionIndex > 0 && insertionIndex < (S32)curve.getKeys().size())
                        {
                            AnimationCurve::Keyframe const& prevKey = curve.getKeys()[insertionIndex - 1];
                            AnimationCurve::Keyframe const& nextKey = curve.getKeys()[insertionIndex];

                            F32 tangent = (nextKey.value - prevKey.value) / (nextKey.time - prevKey.time);

                            inTangent = tangent;
                            outTangent = tangent;
                        }            

                        curve.addKey(x, y, inTangent, outTangent);
                        m_keyIndex = insertionIndex;

                        AnimationCurveManager::GetInstancePtr()->setCurve(curve);
                    }
                }
            }
        }
    }

    //////////////////////////////////////////
    void SceneCurveEditor::notifyCurveClickButtonCursorPressIn(Vec2DF const& _positionOS, CursorInputEvent const& _event)
    {
        if (_event.button == 0)
        {
            S32 focusedKey;
            CurveElement curveElement = getFocusedElement(focusedKey);
            if (curveElement == CurveElement::Key ||
                curveElement == CurveElement::InTangent ||
                curveElement == CurveElement::OutTangent)
            {
                m_keyIndex = focusedKey;
                startDragging(curveElement);
            }

            m_curveClickPosition = _event.position;
        }
        else
        if (_event.button == 1)
        {
            S32 focusedKey;
            CurveElement curveElement = getFocusedElement(focusedKey);
            if (curveElement == CurveElement::Key)
            {
                AnimationCurve curve = AnimationCurveManager::GetInstancePtr()->getCurve();

                if (curve.getKeys().size() > 1)
                {
                    if (m_keyIndex == focusedKey)
                        m_keyIndex = -1;

                    curve.removeKey(focusedKey);

                    AnimationCurveManager::GetInstancePtr()->setCurve(curve);
                }
            }
            else
            {
                m_keyIndex = -1;
            }
        }
    }

    //////////////////////////////////////////
    Vec2DF SceneCurveEditor::getCurveOrigin()
    {
        AnimationCurve const& curve = AnimationCurveManager::GetInstancePtr()->getCurve();

        Vec2DF origin;

        switch (curve.getMinMaxMode())
        {
            case AnimationCurveMinMaxMode::NormalizedPositive:
            {
                origin.x = c_leftOffset;
                origin.y = c_bottomOffset;
                break;
            }
            case AnimationCurveMinMaxMode::Normalized:
            {
                F32 height = m_curveRenderer->getTransform()->getSize().y - c_bottomOffset - c_topOffset;

                origin.x = c_leftOffset;
                origin.y = c_bottomOffset + height * 0.5f;
                break;
            }
            default:
            {
                return Vec2DF::c_zero;
            }
        }

        return origin;
    }

    //////////////////////////////////////////
    F32 SceneCurveEditor::getCurveHeightScalar()
    {
        AnimationCurve const& curve = AnimationCurveManager::GetInstancePtr()->getCurve();

        F32 height = m_curveRenderer->getTransform()->getSize().y - c_bottomOffset - c_topOffset;

        F32 heightScalar;

        switch (curve.getMinMaxMode())
        {
            case AnimationCurveMinMaxMode::NormalizedPositive:
            {
                heightScalar = height;
                break;
            }
            case AnimationCurveMinMaxMode::Normalized:
            {
                heightScalar = height * 0.5f;
                break;
            }
            default:
            {
                return 0.0f;
            }
        }

        return heightScalar;
    }

    //////////////////////////////////////////
    SceneCurveEditor::CurveElement SceneCurveEditor::getFocusedElement(S32& _key)
    {
        _key = -1;

        if (!m_curveClickButton->getFocused())
        {
            return SceneCurveEditor::CurveElement::None;
        }

        F32 width = m_curveRenderer->getTransform()->getSize().x - c_leftOffset - c_rightOffset;

        F32 heightScalar = getCurveHeightScalar();
        Vec2DF origin = getCurveOrigin();

        AnimationCurve const& curve = AnimationCurveManager::GetInstancePtr()->getCurve();

        Vec2DF const& cursorPosition = InputManager::GetInstancePtr()->getCursorPosition(0);

        Vec2DF cursorPositionCurveSpace;
        m_curveRenderer->getTransform()->getWorldTransform().inversedAffineCopy().transformAffine(
            cursorPosition,
            cursorPositionCurveSpace);

        F32 squaredDistanceMin = F32_MAX;
        
        if (curve.getMode() == AnimationCurve::EvaluateMode::Smooth)
        {
            if (m_keyIndex != -1)
            {
                if (m_keyIndex > 0)
                {
                    Vec2DF inTangentPoint = getInTangentPoint(m_keyIndex);
                    F32 squaredDistance = inTangentPoint.squaredDistance(cursorPositionCurveSpace);

                    if (squaredDistance < 3.0f * 3.0f)
                    {
                        _key = m_keyIndex;
                        return SceneCurveEditor::CurveElement::InTangent;
                    }
                }

                if (m_keyIndex < (S32)curve.getKeys().size() - 1)
                {
                    Vec2DF outTangentPoint = getOutTangentPoint(m_keyIndex);
                    F32 squaredDistance = outTangentPoint.squaredDistance(cursorPositionCurveSpace);

                    if (squaredDistance < 3.0f * 3.0f)
                    {
                        _key = m_keyIndex;
                        return SceneCurveEditor::CurveElement::OutTangent;
                    }
                }
            }
        }

        
        for (S32 i = 0; i < (S32)curve.getKeys().size(); ++i)
        {
            AnimationCurve::Keyframe const& keyframe = curve.getKeys()[i];

            F32 x = keyframe.time;
            F32 y = keyframe.value;

            Vec2DF point(
                origin.x + x * width,
                origin.y + y * heightScalar);

            F32 squaredDistance = point.squaredDistance(cursorPositionCurveSpace);

            if (squaredDistance < 5.0f * 5.0f)
            {
                if (squaredDistance < squaredDistanceMin)
                {
                    squaredDistanceMin = squaredDistance;
                    _key = i;
                }
            }
        }

        if (_key != -1)
            return SceneCurveEditor::CurveElement::Key;

        return SceneCurveEditor::CurveElement::None;
    }

    //////////////////////////////////////////
    void SceneCurveEditor::changeCurrentKey(F32 _newTime, F32 _newValue)
    {
        AnimationCurve curve = AnimationCurveManager::GetInstancePtr()->getCurve();

        if (m_keyIndex < 0 && m_keyIndex >= (S32)curve.getKeys().size())
            return;

        if (curve.getKeys()[m_keyIndex].time == _newTime)
        {
            curve.setKey(m_keyIndex, _newValue);
            AnimationCurveManager::GetInstancePtr()->setCurve(curve);
            return;
        }

        // Fix key collision
        bool fixDirection = (_newTime >= 0.5f);
        while (curve.hasKeyTime(_newTime))
        {
            if (fixDirection)
                _newTime -= std::numeric_limits<F32>::epsilon();
            else
                _newTime += std::numeric_limits<F32>::epsilon();
        }

        F32 prevTime = curve.getKeys()[m_keyIndex].time;
        S32 insertionIndex = (S32)curve.getInsertionIndex(_newTime);

        curve.setKey(m_keyIndex, _newValue);
        curve.setKeyTime(m_keyIndex, _newTime);

        if (_newTime < prevTime)
            m_keyIndex = insertionIndex;
        else
            m_keyIndex = insertionIndex - 1;


        AnimationCurveManager::GetInstancePtr()->setCurve(curve);
    }

    //////////////////////////////////////////
    void SceneCurveEditor::notifyCanvasCursorRelease(CursorInputEvent const& _event)
    {
        m_draggingElement = CurveElement::None;
        m_draggingDistance = 0.0f;
    }

    //////////////////////////////////////////
    void SceneCurveEditor::processDragging()
    {
        if (m_draggingElement == CurveElement::None)
            return;

        if (m_keyIndex == -1)
            return;

        F32 width = m_curveRenderer->getTransform()->getSize().x - c_leftOffset - c_rightOffset;

        F32 heightScalar = getCurveHeightScalar();
        Vec2DF origin = getCurveOrigin();

        AnimationCurve curve = AnimationCurveManager::GetInstancePtr()->getCurve();

        Vec2DF const& cursorPosition = InputManager::GetInstancePtr()->getCursorPosition(0);
        

        Vec2DF cursorPositionCurveSpace;
        m_curveRenderer->getTransform()->getWorldTransform().inversedAffineCopy().transformAffine(
            cursorPosition,
            cursorPositionCurveSpace);

        m_draggingDistance += m_draggingPrevCursorPositionCurveSpace.distance(cursorPositionCurveSpace);
        m_draggingPrevCursorPositionCurveSpace = cursorPositionCurveSpace;
        
        if (m_draggingElement == CurveElement::Key)
        {
            if (m_draggingDistance > 1.0f)
            {
                F32 x = cursorPositionCurveSpace.x - origin.x;
                F32 y = cursorPositionCurveSpace.y - origin.y;

                x /= width;
                y /= heightScalar;

                x = Math::Clamp01(x);

                switch (curve.getMinMaxMode())
                {
                    case AnimationCurveMinMaxMode::NormalizedPositive:
                    {
                        y = Math::Clamp01(y);
                        break;
                    }
                    case AnimationCurveMinMaxMode::Normalized:
                    {
                        y = Math::Clamp(y, -1.0f, 1.0f);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
                
                
                changeCurrentKey(x, y);
            }
        }
        else
        if (m_draggingElement == CurveElement::InTangent)
        {
            Vec2DF point = getKeyPoint(m_keyIndex);
            Vec2DF toPoint = cursorPositionCurveSpace - point;
            
            if (toPoint.x > -c_tangentEpsilon)
                toPoint.x = -c_tangentEpsilon;
            
            F32 tn = toPoint.y / toPoint.x;
            tn *= width / heightScalar;

            curve.setKeyInTangent(m_keyIndex, tn);
            curve.setKeyOutTangent(m_keyIndex, tn);

            AnimationCurveManager::GetInstancePtr()->setCurve(curve);
            
        }
        else
        if (m_draggingElement == CurveElement::OutTangent)
        {
            Vec2DF point = getKeyPoint(m_keyIndex);
            Vec2DF toPoint = cursorPositionCurveSpace - point;

            if (toPoint.x < c_tangentEpsilon)
                toPoint.x = c_tangentEpsilon;

            F32 tn = toPoint.y / toPoint.x;
            tn *= width / heightScalar;

            curve.setKeyInTangent(m_keyIndex, tn);
            curve.setKeyOutTangent(m_keyIndex, tn);

            AnimationCurveManager::GetInstancePtr()->setCurve(curve);
        }
    }

    //////////////////////////////////////////
    void SceneCurveEditor::startDragging(CurveElement _element)
    {
        m_draggingElement = _element;
        m_draggingDistance = 0.0f;

        Vec2DF const& cursorPosition = InputManager::GetInstancePtr()->getCursorPosition(0);

        m_curveRenderer->getTransform()->getWorldTransform().inversedAffineCopy().transformAffine(
            cursorPosition,
            m_draggingPrevCursorPositionCurveSpace);
    }

    //////////////////////////////////////////
    Vec2DF SceneCurveEditor::getKeyPoint(Size _keyIndex)
    {
        AnimationCurve const& curve = AnimationCurveManager::GetInstancePtr()->getCurve();

        AnimationCurve::Keyframe const& keyframe = curve.getKeys()[_keyIndex];

        F32 x = keyframe.time;
        F32 y = keyframe.value;

        auto tangentIn = [y, x, keyframe](F32 _x) { return y + keyframe.inTangent * (_x - x); };

        F32 width = m_curveRenderer->getTransform()->getSize().x - c_leftOffset - c_rightOffset;
        F32 heightScalar = getCurveHeightScalar();
        Vec2DF origin = getCurveOrigin();

        Vec2DF point(
            origin.x + x * width,
            origin.y + y * heightScalar);

        return point;
    }

    //////////////////////////////////////////
    Vec2DF SceneCurveEditor::getInTangentPoint(Size _keyIndex)
    {
        AnimationCurve const& curve = AnimationCurveManager::GetInstancePtr()->getCurve();

        AnimationCurve::Keyframe const& keyframe = curve.getKeys()[_keyIndex];

        F32 x = keyframe.time;
        F32 y = keyframe.value;

        auto tangentIn = [y, x, keyframe](F32 _x) { return y + keyframe.inTangent * (_x - x); };        

        F32 width = m_curveRenderer->getTransform()->getSize().x - c_leftOffset - c_rightOffset;
        F32 heightScalar = getCurveHeightScalar();
        Vec2DF origin = getCurveOrigin();

        F32 cellSize = width / 20.0f;

        Vec2DF point(
            origin.x + x * width,
            origin.y + y * heightScalar);

        Vec2DF inTangentDirection(
            (-1.0f) * width,
            (tangentIn(x - 1.0f) - y) * heightScalar);
        inTangentDirection.normalize();
        Vec2DF inTangentPoint = point + inTangentDirection * cellSize * 2;

        return inTangentPoint;
    }

    //////////////////////////////////////////
    Vec2DF SceneCurveEditor::getOutTangentPoint(Size _keyIndex)
    {
        AnimationCurve const& curve = AnimationCurveManager::GetInstancePtr()->getCurve();

        AnimationCurve::Keyframe const& keyframe = curve.getKeys()[_keyIndex];

        F32 x = keyframe.time;
        F32 y = keyframe.value;

        auto tangentOut = [y, x, keyframe](F32 _x) { return y + keyframe.outTangent * (_x - x); };

        F32 width = m_curveRenderer->getTransform()->getSize().x - c_leftOffset - c_rightOffset;
        F32 heightScalar = getCurveHeightScalar();
        Vec2DF origin = getCurveOrigin();

        F32 cellSize = width / 20.0f;

        Vec2DF point(
            origin.x + x * width,
            origin.y + y * heightScalar);

        Vec2DF outTangentDirection(
            (+1.0f) * width,
            (tangentOut(x + 1.0f) - y) * heightScalar);
        outTangentDirection.normalize();
        Vec2DF outTangentPoint = point + outTangentDirection * cellSize * 2;

        return outTangentPoint;
    }

    //////////////////////////////////////////
    void SceneCurveEditor::notifyCurveScalarEditTextInput(EditBox2D* _edit)
    {
        if (StringHelper::IsF32Number(_edit->getText()))
            _edit->setText(StringHelper::F32ToStringFormatted(StringHelper::StringToF32(_edit->getText()), 4));
        else
            _edit->setText("0");

        F32 newScalar = StringHelper::StringToF32(_edit->getText());
        if (newScalar == 0.0f)
        {
            newScalar = 1.0f;
            _edit->setText("1");
        }

        AnimationCurve curve = AnimationCurveManager::GetInstancePtr()->getCurve();

        curve.setScalar(newScalar);

        AnimationCurveManager::GetInstancePtr()->setCurve(curve);
    }

    //////////////////////////////////////////
    void SceneCurveEditor::notifyValueEditTextInput(EditBox2D* _edit)
    {
        AnimationCurve const& curve = AnimationCurveManager::GetInstancePtr()->getCurve();

        if (m_keyIndex < 0 && m_keyIndex >= (S32)curve.getKeys().size())
            return;

        F32 newValue = StringHelper::StringToF32(_edit->getText());

        switch (curve.getMinMaxMode())
        {
            case AnimationCurveMinMaxMode::NormalizedPositive:
                newValue /= curve.getScalar();
                break;
            case AnimationCurveMinMaxMode::Normalized:
                newValue /= curve.getScalar();
                break;
            default:
                break;
        }

        changeCurrentKey(curve.getKeys()[m_keyIndex].time, newValue);
    }

    //////////////////////////////////////////
    void SceneCurveEditor::notifyLocationEditTextInput(EditBox2D* _edit)
    {
        AnimationCurve const& curve = AnimationCurveManager::GetInstancePtr()->getCurve();
        
        if (m_keyIndex < 0 && m_keyIndex >= (S32)curve.getKeys().size())
            return;

        if (StringHelper::IsF32Number(_edit->getText()))
            _edit->setText(StringHelper::F32ToStringFormatted(Math::Clamp(StringHelper::StringToF32(_edit->getText()), 0.0f, 100.0f), 1));
        else
            _edit->setText("0");

        F32 newTime = StringHelper::StringToF32(_edit->getText()) / 100.0f;

        changeCurrentKey(newTime, curve.getKeys()[m_keyIndex].value);
    }

    //////////////////////////////////////////
    void SceneCurveEditor::notifyInTangentEditTextInput(EditBox2D* _edit)
    {
        AnimationCurve curve = AnimationCurveManager::GetInstancePtr()->getCurve();

        if (m_keyIndex < 0 && m_keyIndex >= (S32)curve.getKeys().size())
            return;

        if (!StringHelper::IsF32Number(_edit->getText()))
            _edit->setText("0");

        F32 newTangent = StringHelper::StringToF32(_edit->getText());

        curve.setKeyInTangent(m_keyIndex, newTangent);
        
        AnimationCurveManager::GetInstancePtr()->setCurve(curve);
    }

    //////////////////////////////////////////
    void SceneCurveEditor::notifyOutTangentEditTextInput(EditBox2D* _edit)
    {
        AnimationCurve curve = AnimationCurveManager::GetInstancePtr()->getCurve();

        if (m_keyIndex < 0 && m_keyIndex >= (S32)curve.getKeys().size())
            return;

        if (!StringHelper::IsF32Number(_edit->getText()))
            _edit->setText("0");

        F32 newTangent = StringHelper::StringToF32(_edit->getText());

        curve.setKeyOutTangent(m_keyIndex, newTangent);

        AnimationCurveManager::GetInstancePtr()->setCurve(curve);
    }

} // namespace Maze
//////////////////////////////////////////
