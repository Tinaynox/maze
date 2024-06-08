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
#include "maze-editor-tools/color-picker/MazeSceneColorPicker.hpp"
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
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
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
#include "maze-editor-tools/managers/MazeColorPickerManager.hpp"
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
    F32 const c_intensityMax = 32.0f;

    //////////////////////////////////////////
    // Class SceneColorPicker
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneColorPicker, EcsRenderScene);

    //////////////////////////////////////////
    SceneColorPicker::SceneColorPicker()
        : m_hdr(false)
        , m_prevColor(ColorU32::c_black)
        , m_prevColorIntensity(0.0f)
        , m_hsv(Vec3F::c_zero)
        , m_alpha(255)
        , m_intensity(0.0f)
    {
    }

    //////////////////////////////////////////
    SceneColorPicker::~SceneColorPicker()
    {
        if (ColorPickerManager::GetInstancePtr())
        {
            ColorPickerManager::GetInstancePtr()->eventColorChanged.unsubscribe(this);
            ColorPickerManager::GetInstancePtr()->eventColorHDRChanged.unsubscribe(this);
        }

        if (m_hsvRectButton)
        {
            m_hsvRectButton->eventCursorPressIn.unsubscribe(this);
            m_hsvRectButton->eventCursorDrag.unsubscribe(this);
            m_hsvRectButton->eventCursorMoveOut.unsubscribe(this);
        }

        if (m_hsvBandButton)
        {
            m_hsvBandButton->eventCursorPressIn.unsubscribe(this);
            m_hsvBandButton->eventCursorDrag.unsubscribe(this);
            m_hsvBandButton->eventCursorMoveOut.unsubscribe(this);
        }

        for (Size i = 0; i < 4; ++i)
        {
            m_rgbaButtons[i]->eventCursorPressIn.unsubscribe(this);
            m_rgbaButtons[i]->eventCursorDrag.unsubscribe(this);
            m_rgbaButtons[i]->eventCursorMoveOut.unsubscribe(this);

            m_rgbaTextEdits[i]->eventTextInput.unsubscribe(this);
            m_rgbaTextEdits[i]->eventSelectedChanged.unsubscribe(this);
        }

        m_hexadecimalTextEdit->eventTextInput.unsubscribe(this);
        m_hexadecimalTextEdit->eventSelectedChanged.unsubscribe(this);

        m_canvas.reset();
    }

    //////////////////////////////////////////
    SceneColorPickerPtr SceneColorPicker::Create(RenderTargetPtr const& _renderTarget)
    {
        SceneColorPickerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneColorPicker, object, init(_renderTarget));
        return object;
    }

    //////////////////////////////////////////
    bool SceneColorPicker::init(RenderTargetPtr const& _renderTarget)
    {
        if (!EcsRenderScene::init(_renderTarget))
            return false;

        create2D();
        createSystems();

        ColorPickerManager::GetInstancePtr()->eventColorChanged.subscribe(this, &SceneColorPicker::notifyColorChanged);
        ColorPickerManager::GetInstancePtr()->eventColorHDRChanged.subscribe(this, &SceneColorPicker::notifyColorHDRChanged);

        return true;
    }

    //////////////////////////////////////////
    void SceneColorPicker::setup(bool _hdr)
    {
        m_setup = true;

        m_hdr = _hdr;

        if (m_hdr)
        {
            ColorF128 const& colorHDR = ColorPickerManager::GetInstancePtr()->getColorHDR();
            colorHDR.toColorU32AndIntensity(m_prevColor, m_prevColorIntensity);
        }
        else
        {
            m_prevColor = ColorPickerManager::GetInstancePtr()->getColor();
            m_prevColorIntensity = 0.0f;
        }

        m_intensityEntity->setActiveSelf(m_hdr);

        processRGBtoHSV();

        m_setup = false;
    }

    //////////////////////////////////////////
    void SceneColorPicker::update(F32 _dt)
    {
        
    }


    //////////////////////////////////////////
    void SceneColorPicker::create2D()
    {
        ColorU32 bandColor(130, 130, 130);

        EntityPtr canvasEntity = createEntity();
        m_canvas = canvasEntity->createComponent<Canvas>();
        m_canvas->setClearColorFlag(true);
        m_canvas->setClearDepthFlag(true);
        m_canvas->setClearColor(ColorU32(203, 203, 203, 255));
        m_canvas->setRenderTarget(m_renderTarget);


        m_copyButton = UIHelper::CreateDefaultClickButton(
            "C",
            { 18.0f, 18.0f },
            { 12.0f, -15.0f },
            m_canvas->getTransform(),
            this,
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        m_copyButton->eventClick.subscribe(
            [this](Button2D* _button, CursorInputEvent const& _event)
            {
                if (m_hdr)
                {
                    ColorF128 const& color = ColorPickerManager::GetInstancePtr()->getColorHDR();
                    SystemManager::GetInstancePtr()->setClipboardString(color.toString());
                }
                else
                {
                    ColorU32 const& color = ColorPickerManager::GetInstancePtr()->getColor();
                    SystemManager::GetInstancePtr()->setClipboardString(color.toString());
                }
            });

        m_pasteButton = UIHelper::CreateDefaultClickButton(
            "P",
            { 18.0f, 18.0f },
            { 32.0f, -15.0f },
            m_canvas->getTransform(),
            this,
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        m_pasteButton->eventClick.subscribe(
            [this](Button2D* _button, CursorInputEvent const& _event)
            {
                String text = SystemManager::GetInstancePtr()->getClipboardAsString();
                if (m_hdr)
                {
                    ColorF128 color = ColorF128::FromString(text);
                    ColorPickerManager::GetInstancePtr()->setColorHDR(color);
                }
                else
                {
                    ColorU32 color = ColorU32::FromString(text);
                    ColorPickerManager::GetInstancePtr()->setColor(color);
                }
            });


        SpriteRenderer2DPtr transparentChessPrevNextRenderer = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::TransparentChess),
            Vec2F(76.0f, 24.0f),
            Vec2F(-10.0f, -15.0f),
            GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getColorTextureMaterial(),
            m_canvas->getTransform(),
            this,
            Vec2F(1.0f, 1.0f),
            Vec2F(1.0f, 1.0f));
        transparentChessPrevNextRenderer->getMaterial()->setUniform(
            MAZE_HCS("u_baseMapST"),
            Vec4F(
                transparentChessPrevNextRenderer->getTransform()->getWidth() / 8.0f,
                transparentChessPrevNextRenderer->getTransform()->getHeight() / 8.0f,
                0.0f,
                0.0f));

        m_finalColorRenderer = SpriteHelper::CreateSprite(
            ColorU32::c_white,
            Vec2F(76.0f / 2.0f, 24.0f),
            Vec2F(0.0f, 0.0f),
            GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getBuiltinMaterial(BuiltinMaterialType::ColorHDR),
            transparentChessPrevNextRenderer->getTransform(),
            this,
            Vec2F(1.0f, 1.0f),
            Vec2F(1.0f, 1.0f));

        m_prevColorRenderer = SpriteHelper::CreateSprite(
            ColorU32::c_white,
            Vec2F(76.0f / 2.0f, 24.0f),
            Vec2F(0.0f, 0.0f),
            GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getBuiltinMaterial(BuiltinMaterialType::ColorHDR),
            m_finalColorRenderer->getTransform(),
            this,
            Vec2F(0.0f, 0.0f),
            Vec2F(1.0f, 0.0f));

        SpriteRenderer2DPtr hsvRectBorder = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Frame01),
            Vec2F(185.0f, 185.0f),
            Vec2F(10.0f, 212.0f),
            GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getColorTextureMaterial(),
            m_canvas->getTransform(),
            this,
            Vec2F(0.0f, 0.0f),
            Vec2F(0.0f, 0.0f));
        hsvRectBorder->setColor(bandColor);
        hsvRectBorder->setRenderMode(SpriteRenderMode::Sliced);
        hsvRectBorder->getTransform()->setZ(100);

        m_hsvRectRenderer = SpriteHelper::CreateSprite(
            ColorU32::c_white,
            hsvRectBorder->getTransform()->getSize() - 4.0f,
            Vec2F(2.0f, 2.0f),
            GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getBuiltinMaterial(BuiltinMaterialType::HSVRect),
            hsvRectBorder->getTransform(),
            this,
            Vec2F(0.0f, 0.0f),
            Vec2F(0.0f, 0.0f));

        m_hsvRectButton = m_hsvRectRenderer->getEntityRaw()->ensureComponent<UIElement2D>();
        m_hsvRectButton->setUnpressOnUnfocus(false);
        m_hsvRectButton->eventCursorPressIn.subscribe(this, &SceneColorPicker::notifyHSVRectCursorPressIn);
        m_hsvRectButton->eventCursorDrag.subscribe(this, &SceneColorPicker::notifyHSVRectCursorDrag);
        m_hsvRectButton->eventCursorMoveOut.subscribe(this, &SceneColorPicker::notifyHSVRectCursorMoveOut);

        m_hsvRectCircleRenderer = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::ColorPickerCircle),
            Vec2F(13.01f, 13.01f),
            Vec2F(0.0f, 0.0f),
            GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getColorTextureMaterial(),
            m_hsvRectRenderer->getTransform(),
            this,
            Vec2F(0.0f, 0.0f),
            Vec2F(0.5f, 0.5f));

        

        SpriteRenderer2DPtr hsvBandBorder = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Frame01),
            Vec2F(23.0f, 185.0f),
            Vec2F(-10.0f, 212.0f),
            GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getColorTextureMaterial(),
            m_canvas->getTransform(),
            this,
            Vec2F(1.0f, 0.0f),
            Vec2F(1.0f, 0.0f));
        hsvBandBorder->setColor(bandColor);
        hsvBandBorder->setRenderMode(SpriteRenderMode::Sliced);

        m_hsvBandRenderer = SpriteHelper::CreateSprite(
            ColorU32::c_white,
            hsvBandBorder->getTransform()->getSize() - 4.0f,
            Vec2F(2.0f, 2.0f),
            GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getBuiltinMaterial(BuiltinMaterialType::HSVBand),
            hsvBandBorder->getTransform(),
            this,
            Vec2F(0.0f, 0.0f),
            Vec2F(0.0f, 0.0f));

        m_hsvBandButton = m_hsvBandRenderer->getEntityRaw()->ensureComponent<UIElement2D>();
        m_hsvBandButton->setUnpressOnUnfocus(false);
        m_hsvBandButton->eventCursorPressIn.subscribe(this, &SceneColorPicker::notifyHSVBandCursorPressIn);
        m_hsvBandButton->eventCursorDrag.subscribe(this, &SceneColorPicker::notifyHSVBandCursorDrag);
        m_hsvBandButton->eventCursorMoveOut.subscribe(this, &SceneColorPicker::notifyHSVBandCursorMoveOut);
        
        m_hsvBandScale = SpriteHelper::CreateTransform2D(
            Vec2F(m_hsvBandRenderer->getTransform()->getWidth(), 1.0f),
            Vec2F::c_zero,
            m_hsvBandRenderer->getTransform(),
            this,
            Vec2F(0.0f, 0.0f),
            Vec2F(0.0f, 0.0f));

        SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::ScaleMark),
            Vec2F(8, 9),
            Vec2F(-2.0f, 0.0f),
            GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getColorTextureMaterial(),
            m_hsvBandScale,
            this,
            Vec2F::c_zero,
            Vec2F(1.0f, 0.5f));

        SpriteRenderer2DPtr rightScaleMarkRenderer = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::ScaleMark),
            Vec2F(8, 9),
            Vec2F(m_hsvBandScale->getWidth() + 2.0f, 0.0f),
            GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getColorTextureMaterial(),
            m_hsvBandScale,
            this,
            Vec2F::c_zero,
            Vec2F(1.0f, 0.5f));
        rightScaleMarkRenderer->getTransform()->setLocalScale(-1.0f, 1.0f);


        m_floatLabel = UIHelper::CreateText(
            "",
            12,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            { 8.0f, 18.0f },
            { 10.0f, 200.0f },
            m_canvas->getTransform(),
            this,
            Vec2F::c_zero,
            Vec2F(0.0f, 1.0f));
        m_floatLabel->setColor(ColorU32::c_black);

        
        VerticalLayout2DPtr layout = UIHelper::CreateVerticalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Top,
            Vec2F(300.0f, 300.0f),
            Vec2F(10.0f, 176.0f),
            m_canvas->getTransform(),
            this,
            Vec2F::c_zero,
            Vec2F(0.0f, 1.0f));
        layout->setSpacing(8.0f);
        
        // R
        {
            HorizontalLayout2DPtr rowLayout = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                Vec2F(211.0f, 18.0f),
                Vec2F(0.0f, 0.0f),
                layout->getTransform(),
                this,
                Vec2F(0.0f, 1.0f),
                Vec2F(0.0f, 1.0f));
            rowLayout->setExpand(true);
            rowLayout->setAutoWidth(false);


            AbstractTextRenderer2DPtr label = UIHelper::CreateText(
                "R",
                12,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                { 8.0f, 18.0f },
                Vec2F::c_zero,
                rowLayout->getTransform(),
                this);
            label->setColor(ColorU32::c_black);

            SpriteRenderer2DPtr border = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Frame01),
                Vec2F(148.0f, 18.0f),
                Vec2F(0.0f, 0.0f),
                GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getColorTextureMaterial(),
                rowLayout->getTransform(),
                this,
                Vec2F(0.0f, 0.0f),
                Vec2F(0.0f, 0.0f));
            border->setColor(bandColor);
            border->setRenderMode(SpriteRenderMode::Sliced);

            m_rgbaRenderers[0] = SpriteHelper::CreateSprite(
                ColorU32::c_white,
                border->getTransform()->getSize() - 4.0f,
                Vec2F(2.0f, 2.0f),
                GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getBuiltinMaterial(BuiltinMaterialType::ColorPickerChannel),
                border->getTransform(),
                this,
                Vec2F(0.0f, 0.0f),
                Vec2F(0.0f, 0.0f));
            m_rgbaRenderers[0]->getMaterial()->setUniform(MAZE_HCS("u_channel"), 0);

            m_rgbaTextEdits[0] = UIHelper::CreateDefaultEditBox(
                "255",
                FontMaterialPtr(),
                12,
                Vec2F(45.0f, 18.0f),
                Vec2F::c_zero,
                rowLayout->getTransform(),
                this);
        }

        // G
        {
            HorizontalLayout2DPtr rowLayout = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                Vec2F(211.0f, 18.0f),
                Vec2F(0.0f, 0.0f),
                layout->getTransform(),
                this,
                Vec2F(0.0f, 1.0f),
                Vec2F(0.0f, 1.0f));
            rowLayout->setExpand(true);
            rowLayout->setAutoWidth(false);

            AbstractTextRenderer2DPtr label = UIHelper::CreateText(
                "G",
                12,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                { 8.0f, 18.0f },
                Vec2F::c_zero,
                rowLayout->getTransform(),
                this);
            label->setColor(ColorU32::c_black);

            SpriteRenderer2DPtr border = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Frame01),
                Vec2F(148.0f, 18.0f),
                Vec2F(0.0f, 0.0f),
                GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getColorTextureMaterial(),
                rowLayout->getTransform(),
                this,
                Vec2F(0.0f, 0.0f),
                Vec2F(0.0f, 0.0f));
            border->setColor(bandColor);
            border->setRenderMode(SpriteRenderMode::Sliced);

            m_rgbaRenderers[1] = SpriteHelper::CreateSprite(
                ColorU32::c_white,
                border->getTransform()->getSize() - 4.0f,
                Vec2F(2.0f, 2.0f),
                GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getBuiltinMaterial(BuiltinMaterialType::ColorPickerChannel),
                border->getTransform(),
                this,
                Vec2F(0.0f, 0.0f),
                Vec2F(0.0f, 0.0f));
            m_rgbaRenderers[1]->getMaterial()->setUniform(MAZE_HCS("u_channel"), 1);

            m_rgbaTextEdits[1] = UIHelper::CreateDefaultEditBox(
                "0",
                FontMaterialPtr(),
                12,
                Vec2F(45.0f, 18.0f),
                Vec2F::c_zero,
                rowLayout->getTransform(),
                this);
        }

        // B
        {
            HorizontalLayout2DPtr rowLayout = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                Vec2F(211.0f, 18.0f),
                Vec2F(0.0f, 0.0f),
                layout->getTransform(),
                this,
                Vec2F(0.0f, 1.0f),
                Vec2F(0.0f, 1.0f));
            rowLayout->setExpand(true);
            rowLayout->setAutoWidth(false);

            AbstractTextRenderer2DPtr label = UIHelper::CreateText(
                "B",
                12,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                { 8.0f, 18.0f },
                Vec2F::c_zero,
                rowLayout->getTransform(),
                this);
            label->setColor(ColorU32::c_black);

            SpriteRenderer2DPtr border = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Frame01),
                Vec2F(148.0f, 18.0f),
                Vec2F(0.0f, 0.0f),
                GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getColorTextureMaterial(),
                rowLayout->getTransform(),
                this,
                Vec2F(0.0f, 0.0f),
                Vec2F(0.0f, 0.0f));
            border->setColor(bandColor);
            border->setRenderMode(SpriteRenderMode::Sliced);

            m_rgbaRenderers[2] = SpriteHelper::CreateSprite(
                ColorU32::c_white,
                border->getTransform()->getSize() - 4.0f,
                Vec2F(2.0f, 2.0f),
                GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getBuiltinMaterial(BuiltinMaterialType::ColorPickerChannel),
                border->getTransform(),
                this,
                Vec2F(0.0f, 0.0f),
                Vec2F(0.0f, 0.0f));
            m_rgbaRenderers[2]->getMaterial()->setUniform(MAZE_HCS("u_channel"), 2);

            m_rgbaTextEdits[2] = UIHelper::CreateDefaultEditBox(
                "0",
                FontMaterialPtr(),
                12,
                Vec2F(45.0f, 18.0f),
                Vec2F::c_zero,
                rowLayout->getTransform(),
                this);
        }

        // A
        {
            HorizontalLayout2DPtr rowLayout = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                Vec2F(211.0f, 18.0f),
                Vec2F(0.0f, 0.0f),
                layout->getTransform(),
                this,
                Vec2F(0.0f, 1.0f),
                Vec2F(0.0f, 1.0f));
            rowLayout->setExpand(true);
            rowLayout->setAutoWidth(false);

            AbstractTextRenderer2DPtr label = UIHelper::CreateText(
                "A",
                12,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                { 8.0f, 18.0f },
                Vec2F::c_zero,
                rowLayout->getTransform(),
                this);
            label->setColor(ColorU32::c_black);

            SpriteRenderer2DPtr border = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Frame01),
                Vec2F(148.0f, 18.0f),
                Vec2F(0.0f, 0.0f),
                GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getColorTextureMaterial(),
                rowLayout->getTransform(),
                this,
                Vec2F(0.0f, 0.0f),
                Vec2F(0.0f, 0.0f));
            border->setColor(bandColor);
            border->setRenderMode(SpriteRenderMode::Sliced);

            SpriteRenderer2DPtr transparentChessRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::TransparentChess),
                border->getTransform()->getSize() - 4.0f,
                Vec2F(2.0f, 2.0f),
                GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getColorTextureMaterial(),
                border->getTransform(),
                this,
                Vec2F(0.0f, 0.0f),
                Vec2F(0.0f, 0.0f));
            transparentChessRenderer->getMaterial()->setUniform(
                MAZE_HCS("u_baseMapST"),
                Vec4F(
                    transparentChessRenderer->getTransform()->getWidth() / 8.0f,
                    transparentChessRenderer->getTransform()->getHeight() / 8.0f,
                    0.0f,
                    0.0f));

            m_rgbaRenderers[3] = SpriteHelper::CreateSprite(
                ColorU32::c_white,
                border->getTransform()->getSize() - 3.0f,
                Vec2F(2.0f, 2.0f),
                GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getBuiltinMaterial(BuiltinMaterialType::ColorPickerChannel),
                border->getTransform(),
                this,
                Vec2F(0.0f, 0.0f),
                Vec2F(0.0f, 0.0f));
            m_rgbaRenderers[3]->getMaterial()->setUniform(MAZE_HCS("u_channel"), 3);

            m_rgbaTextEdits[3] = UIHelper::CreateDefaultEditBox(
                "255",
                FontMaterialPtr(),
                12,
                Vec2F(45.0f, 18.0f),
                Vec2F::c_zero,
                rowLayout->getTransform(),
                this);
        }

        // Hexadecimal
        {
            HorizontalLayout2DPtr rowLayout = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                Vec2F(211.0f, 18.0f),
                Vec2F(0.0f, 0.0f),
                layout->getTransform(),
                this,
                Vec2F(0.0f, 1.0f),
                Vec2F(0.0f, 1.0f));
            rowLayout->setExpand(true);
            rowLayout->setAutoWidth(false);

            AbstractTextRenderer2DPtr label = UIHelper::CreateText(
                "Hexadecimal",
                12,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                { 100.0f, 18.0f },
                Vec2F::c_zero,
                rowLayout->getTransform(),
                this);
            label->setColor(ColorU32::c_black);

            m_hexadecimalTextEdit = UIHelper::CreateDefaultEditBox(
                "#FFFFFF",
                FontMaterialPtr(),
                12,
                Vec2F(80.0f, 18.0f),
                Vec2F::c_zero,
                rowLayout->getTransform(),
                this);
            m_hexadecimalTextEdit->eventTextInput.subscribe(this, &SceneColorPicker::notifyRGBATextInput);
            m_hexadecimalTextEdit->eventSelectedChanged.subscribe(this, &SceneColorPicker::notifyRGBATextSelectedChanged);
        }

        for (Size i = 0; i < 4; ++i)
        {
            SpriteRenderer2DPtr spriteRenderer = m_rgbaRenderers[i];

            m_rgbaButtons[i] = spriteRenderer->getEntityRaw()->ensureComponent<UIElement2D>();
            m_rgbaButtons[i]->setUnpressOnUnfocus(false);
            m_rgbaButtons[i]->eventCursorPressIn.subscribe(this, &SceneColorPicker::notifyRGBBandCursorPressIn);
            m_rgbaButtons[i]->eventCursorDrag.subscribe(this, &SceneColorPicker::notifyRGBBandCursorDrag);
            m_rgbaButtons[i]->eventCursorMoveOut.subscribe(this, &SceneColorPicker::notifyRGBBandCursorMoveOut);

            m_rgbaCarriages[i] = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
                Vec2F(4.0f, spriteRenderer->getTransform()->getHeight()),
                Vec2F(0.0f, 0.0f),
                GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getColorTextureMaterial(),
                spriteRenderer->getTransform(),
                this,
                Vec2F(0.0f, 0.0f),
                Vec2F(0.5f, 0.0f));
            m_rgbaCarriages[i]->setRenderMode(SpriteRenderMode::Sliced);
            m_rgbaCarriages[i]->setColor(ColorU32(241, 241, 241));

            m_rgbaTextEdits[i]->eventTextInput.subscribe(this, &SceneColorPicker::notifyRGBATextInput);
            m_rgbaTextEdits[i]->eventSelectedChanged.subscribe(this, &SceneColorPicker::notifyRGBATextSelectedChanged);
        }

        {
            HorizontalLayout2DPtr rowLayout = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                Vec2F(211.0f, 18.0f),
                Vec2F(0.0f, 0.0f),
                layout->getTransform(),
                this,
                Vec2F(0.0f, 1.0f),
                Vec2F(0.0f, 1.0f));
            rowLayout->setExpand(true);
            rowLayout->setAutoWidth(false);

            m_intensityEntity = rowLayout->getEntity();

            AbstractTextRenderer2DPtr label = UIHelper::CreateText(
                "I",
                12,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                { 8.0f, 18.0f },
                Vec2F::c_zero,
                rowLayout->getTransform(),
                this);
            label->setColor(ColorU32::c_black);

            m_intensitySlider = UIHelper::CreateDefaultSlider(
                0.5f,
                Vec2F(138.0f, 18.0f),
                Vec2F::c_zero,
                rowLayout->getTransform(),
                this);
            m_intensitySlider->eventValueChanged.subscribe(this, &SceneColorPicker::notifyIntensitySliderValueChanged);

            m_intensityTextEdit = UIHelper::CreateDefaultEditBox(
                "0",
                FontMaterialPtr(),
                12,
                Vec2F(45.0f, 18.0f),
                Vec2F::c_zero,
                rowLayout->getTransform(),
                this);
            m_intensityTextEdit->eventTextInput.subscribe(this, &SceneColorPicker::notifyIntensityTextInput);
            m_intensityTextEdit->eventSelectedChanged.subscribe(this, &SceneColorPicker::notifyIntensityTextSelectedChanged);
        }

        processRGBtoHSV();
        updateUI();

    }

    //////////////////////////////////////////
    void SceneColorPicker::createSystems()
    {

    }

    //////////////////////////////////////////
    void SceneColorPicker::notifyColorChanged(ColorU32 const& _color)
    {
        if (m_setup)
            return;

        if (!m_hdr)
            setup(false);
    }

    //////////////////////////////////////////
    void SceneColorPicker::notifyColorHDRChanged(ColorF128 const& _color)
    {
        if (m_setup)
            return;

        if (m_hdr)
            setup(true);
    }

    //////////////////////////////////////////
    void SceneColorPicker::updateUI()
    {
        if (getState() == EcsSceneState::Destroy)
            return;

        ColorU32 color = ColorHelper::ConvertHSVToRGB(m_hsv);
        color.a = m_alpha;

        m_prevColorRenderer->setColor(m_prevColor);
        m_prevColorRenderer->getMaterial()->setUniform(MAZE_HCS("u_intensity"), m_prevColorIntensity);
        m_finalColorRenderer->setColor(color);
        m_finalColorRenderer->getMaterial()->setUniform(MAZE_HCS("u_intensity"), m_intensity);

        m_hsvRectRenderer->getMaterial()->setUniform(MAZE_HCS("u_hue"), m_hsv.x);
        m_hsvRectRenderer->getMaterial()->setUniform(MAZE_HCS("u_intensity"), m_intensity);

        m_hsvRectCircleRenderer->getTransform()->setLocalPosition(
            m_hsvRectRenderer->getTransform()->getSize() * Vec2F(m_hsv.y, m_hsv.z));

        m_hsvBandScale->setLocalY(m_hsvBandRenderer->getTransform()->getHeight() * Math::Clamp01(m_hsv.x / 360.0f));

        Vec3F const luminosityK(0.2125f, 0.7154f, 0.0721f);
        F32 luminosity = (color.toVec3F32() * (m_intensity + 1.0f)).dotProduct(luminosityK);
        if (luminosity > 0.5f)
            m_hsvRectCircleRenderer->setColor(ColorU32::c_black);
        else
            m_hsvRectCircleRenderer->setColor(ColorU32::c_white);

        ColorU32 colorPickerColor;
        F32 intensity = 0.0f;

        if (m_hdr)        
            ColorPickerManager::GetInstancePtr()->getColorHDR().toColorU32AndIntensity(colorPickerColor, intensity);
        else
            colorPickerColor = ColorPickerManager::GetInstancePtr()->getColor();

        F32 floatR = (F32)colorPickerColor.r / 255.0f;
        F32 floatG = (F32)colorPickerColor.g / 255.0f;
        F32 floatB = (F32)colorPickerColor.b / 255.0f;
        F32 floatA = (F32)colorPickerColor.a / 255.0f;

        ColorF128 colorF128 = ColorF128::FromColorU32AndIntensity(colorPickerColor, intensity);
        m_floatLabel->setText(
            "4DF: " +
            StringHelper::F32ToStringFormatted(colorF128.r, 2) + " " +
            StringHelper::F32ToStringFormatted(colorF128.g, 2) + " " +
            StringHelper::F32ToStringFormatted(colorF128.b, 2) + " " +
            StringHelper::F32ToStringFormatted(colorF128.a, 2));

        for (Size i = 0; i < 4; ++i)
        {
            m_rgbaTextEdits[i]->setText(StringHelper::ToString(colorPickerColor[i]));
            m_rgbaRenderers[i]->getMaterial()->setUniform(MAZE_HCS("u_color"), colorPickerColor.toVec4F32());
        }

        m_hexadecimalTextEdit->setText(StringHelper::ToUpper(colorPickerColor.toStringHex()));

        m_rgbaCarriages[0]->getTransform()->setLocalX(
            m_rgbaCarriages[0]->getTransform()->getParent()->getWidth() * floatR);
        m_rgbaCarriages[1]->getTransform()->setLocalX(
            m_rgbaCarriages[1]->getTransform()->getParent()->getWidth() * floatG);
        m_rgbaCarriages[2]->getTransform()->setLocalX(
            m_rgbaCarriages[2]->getTransform()->getParent()->getWidth() * floatB);
        m_rgbaCarriages[3]->getTransform()->setLocalX(
            m_rgbaCarriages[3]->getTransform()->getParent()->getWidth() * floatA);

        m_intensityTextEdit->setText(StringHelper::F32ToString(intensity, 2));
        m_intensitySlider->setValue(intensity / c_intensityMax);
    }

    //////////////////////////////////////////
    void SceneColorPicker::notifyHSVRectCursorPressIn(Vec2F const& _positionOS, CursorInputEvent const& _inputEvent)
    {
        processHSVRectPick(_positionOS);
    }

    //////////////////////////////////////////
    void SceneColorPicker::notifyHSVRectCursorDrag(Vec2F const& _positionOS, CursorInputEvent const& _inputEvent)
    {
        if (m_hsvRectButton->getPressed())
            processHSVRectPick(_positionOS);
    }

    //////////////////////////////////////////
    void SceneColorPicker::notifyHSVRectCursorMoveOut(CursorInputEvent const& _inputEvent)
    {
        if (m_hsvRectButton->getPressed())
        {
            Vec2F positionOS = m_hsvRectButton->getTransform()->getWorldTransform().inversedAffine().transformAffine(_inputEvent.position);
            processHSVRectPick(positionOS);
        }
    }

    //////////////////////////////////////////
    void SceneColorPicker::processHSVRectPick(Vec2F const& _positionOS)
    {
        F32 s = Math::Clamp01(_positionOS.x / m_hsvRectRenderer->getTransform()->getWidth());
        F32 v = Math::Clamp01(_positionOS.y / m_hsvRectRenderer->getTransform()->getHeight());

        Vec3F newHSV(
            m_hsv.x,
            s,
            v);
        setHSV(newHSV, m_alpha, m_intensity);
    }

    //////////////////////////////////////////
    void SceneColorPicker::notifyHSVBandCursorPressIn(Vec2F const& _positionOS, CursorInputEvent const& _inputEvent)
    {
        processHSVBandPick(_positionOS);
    }

    //////////////////////////////////////////
    void SceneColorPicker::notifyHSVBandCursorDrag(Vec2F const& _positionOS, CursorInputEvent const& _inputEvent)
    {
        if (m_hsvBandButton->getPressed())
            processHSVBandPick(_positionOS);
    }

    //////////////////////////////////////////
    void SceneColorPicker::notifyHSVBandCursorMoveOut(CursorInputEvent const& _inputEvent)
    {
        if (m_hsvBandButton->getPressed())
        {
            Vec2F positionOS = m_hsvBandButton->getTransform()->getWorldTransform().inversedAffine().transformAffine(_inputEvent.position);
            processHSVBandPick(positionOS);
        }
    }

    //////////////////////////////////////////
    void SceneColorPicker::processHSVBandPick(Vec2F const& _positionOS)
    {
        F32 hue = Math::Clamp(360.0f * _positionOS.y / m_hsvBandRenderer->getTransform()->getHeight(), 0.0f, 360.0f);

        Vec3F newHSV(
            hue,
            m_hsv.y,
            m_hsv.z);
        setHSV(newHSV, m_alpha, m_intensity);
    }

    //////////////////////////////////////////
    void SceneColorPicker::notifyRGBBandCursorPressIn(Vec2F const& _positionOS, CursorInputEvent const& _inputEvent)
    {
        S32 channel = -1;
        for (Size i = 0; i < 4; ++i)
            if (m_rgbaButtons[i]->getFocused())
            {
                channel = (S32)i;
                break;
            }

        if (channel != -1)
            processRGBBandPick(channel, _positionOS);
    }

    //////////////////////////////////////////
    void SceneColorPicker::notifyRGBBandCursorDrag(Vec2F const& _positionOS, CursorInputEvent const& _inputEvent)
    {
        S32 channel = -1;
        for (Size i = 0; i < 4; ++i)
            if (m_rgbaButtons[i]->getPressed())
            {
                channel = (S32)i;
                break;
            }

        if (channel != -1)
            processRGBBandPick(channel, _positionOS);
    }

    //////////////////////////////////////////
    void SceneColorPicker::notifyRGBBandCursorMoveOut(CursorInputEvent const& _inputEvent)
    {
        S32 channel = -1;
        for (Size i = 0; i < 4; ++i)
            if (m_rgbaButtons[i]->getPressed())
            {
                channel = (S32)i;
                break;
            }

        if (channel != -1)
        {
            SpriteRenderer2DPtr rgbBandRenderer = m_rgbaRenderers[channel];
            Vec2F positionOS = rgbBandRenderer->getTransform()->getWorldTransform().inversedAffine().transformAffine(_inputEvent.position);
            processRGBBandPick(channel, positionOS);
        }
    }

    //////////////////////////////////////////
    void SceneColorPicker::processRGBBandPick(S32 _channel, Vec2F const& _positionOS)
    {
        SpriteRenderer2DPtr rgbBandRenderer = m_rgbaRenderers[_channel];

        F32 value = Math::Clamp01(_positionOS.x / rgbBandRenderer->getTransform()->getWidth());

        ColorU32 color;
        F32 intensity = 0.0f;

        if (m_hdr)
            ColorPickerManager::GetInstancePtr()->getColorHDR().toColorU32AndIntensity(color, intensity);
        else
            color = ColorPickerManager::GetInstancePtr()->getColor();

        switch (_channel)
        {
            case 0: color.r = U8(value * 255); break;
            case 1: color.g = U8(value * 255); break;
            case 2: color.b = U8(value * 255); break;
            case 3: color.a = U8(value * 255); break;
        }

        if (m_hdr)
        {
            ColorPickerManager::GetInstancePtr()->setColorHDR(
                ColorF128::FromColorU32AndIntensity(
                    color,
                    intensity));
        }
        else
        {
            ColorPickerManager::GetInstancePtr()->setColor(color);
        }

        m_hsv = ColorHelper::ConvertRGBToHSV(color);
        m_alpha = color.a;
        updateUI();
    }

    //////////////////////////////////////////
    void SceneColorPicker::notifyRGBATextInput(EditBox2D* _editBox)
    {
        processRGBAFromEditBox(_editBox);
    }

    //////////////////////////////////////////
    void SceneColorPicker::notifyRGBATextSelectedChanged(EditBox2D* _editBox, bool _selected)
    {
        if (!_selected)
        {
            processRGBAFromEditBox(_editBox);
        }
    }

    //////////////////////////////////////////
    void SceneColorPicker::notifyIntensitySliderValueChanged(Slider2D* _slider, F32 _value)
    {
        processIntensityFromSlider();
    }

    //////////////////////////////////////////
    void SceneColorPicker::notifyIntensityTextInput(EditBox2D* _editBox)
    {
        processIntensityFromEditBox();
    }

    //////////////////////////////////////////
    void SceneColorPicker::notifyIntensityTextSelectedChanged(EditBox2D* _editBox, bool _selected)
    {
        if (!_selected)
        {
            processIntensityFromEditBox();
        }
    }

    //////////////////////////////////////////
    void SceneColorPicker::processIntensityFromEditBox()
    {
        if (!m_hdr)
            return;

        ColorU32 color;
        F32 intensity = 0.0f;
        ColorPickerManager::GetInstancePtr()->getColorHDR().toColorU32AndIntensity(color, intensity);

        m_intensity = Math::Clamp(StringHelper::StringToF32(m_intensityTextEdit->getText()), 0.0f, c_intensityMax);

        ColorPickerManager::GetInstancePtr()->setColorHDR(
            ColorF128::FromColorU32AndIntensity(
                color,
                m_intensity));
        updateUI();
    }

    //////////////////////////////////////////
    void SceneColorPicker::processIntensityFromSlider()
    {
        if (!m_hdr)
            return;

        ColorU32 color;
        F32 intensity = 0.0f;
        ColorPickerManager::GetInstancePtr()->getColorHDR().toColorU32AndIntensity(color, intensity);

        m_intensity = Math::Clamp(m_intensitySlider->getValue() * c_intensityMax, 0.0f, c_intensityMax);

        ColorPickerManager::GetInstancePtr()->setColorHDR(
            ColorF128::FromColorU32AndIntensity(
                color,
                m_intensity));

        updateUI();
    }

    //////////////////////////////////////////
    void SceneColorPicker::processRGBAFromEditBox(EditBox2D* _editBox)
    {
        ColorU32 color;
        F32 intensity = 0.0f;

        if (m_hdr)
            ColorPickerManager::GetInstancePtr()->getColorHDR().toColorU32AndIntensity(color, intensity);
        else
            color = ColorPickerManager::GetInstancePtr()->getColor();

        for (Size i = 0; i < 4; ++i)
        {
            if (_editBox == m_rgbaTextEdits[i].get())
            {
                color[i] = StringHelper::StringToU8(m_rgbaTextEdits[i]->getText());
                break;
            }
        }

        if (_editBox == m_hexadecimalTextEdit.get())
        {
            color = ColorU32::FromStringHex(m_hexadecimalTextEdit->getText());
        }


        if (m_hdr)
        {
            ColorPickerManager::GetInstancePtr()->setColorHDR(
                ColorF128::FromColorU32AndIntensity(
                    color,
                    intensity));
        }
        else
        {
            ColorPickerManager::GetInstancePtr()->setColor(color);
        }


        m_hsv = ColorHelper::ConvertRGBToHSV(color);
        m_alpha = color.a;
        updateUI();
    }

    //////////////////////////////////////////
    void SceneColorPicker::setHSV(
        Vec3F const& _hsv,
        U8 _alpha,
        F32 _intensity)
    {
        m_hsv = _hsv;
        m_alpha = _alpha;
        m_intensity = _intensity;

        processHSVtoRGB();
        updateUI();
    }

    //////////////////////////////////////////
    void SceneColorPicker::processRGBtoHSV()
    {
        ColorU32 color;
        F32 intensity = 0.0f;
        
        if (m_hdr)
            ColorPickerManager::GetInstancePtr()->getColorHDR().toColorU32AndIntensity(color, intensity);
        else
            color = ColorPickerManager::GetInstancePtr()->getColor();

        Vec3F hsv = ColorHelper::ConvertRGBToHSV(color);
        setHSV(hsv, color.a, intensity);
    }

    //////////////////////////////////////////
    void SceneColorPicker::processHSVtoRGB()
    {        
        if (m_hdr)
        {
            auto rgb = ColorHelper::ConvertHSVToRGB(m_hsv);

            ColorF128 color = ColorF128::FromVec4F32AndIntensity(
                Vec4F(rgb, (F32)m_alpha / 255.0f),
                m_intensity);

            if (color != ColorPickerManager::GetInstancePtr()->getColorHDR())
                ColorPickerManager::GetInstancePtr()->setColorHDR(color);
        }
        else
        {
            ColorU32 color = ColorHelper::ConvertHSVToRGB(m_hsv);
            color.a = m_alpha;

            if (color != ColorPickerManager::GetInstancePtr()->getColor())
                ColorPickerManager::GetInstancePtr()->setColor(color);
        }
    }

} // namespace Maze
//////////////////////////////////////////
