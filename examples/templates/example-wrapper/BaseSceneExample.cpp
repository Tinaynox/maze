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
#include "BaseSceneExample.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/components/MazeRotor3D.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeSinMovement3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-core/math/MazeTMat.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeMeshManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/MazeGPUTextureBuffer.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeSystemFontManager.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCamera3DShadowBuffer.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer3D.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderMZMESH.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
// #include "maze-plugin-profiler-view/MazeProfilerViewPlugin.hpp"
// #include "maze-plugin-loader-png/MazeLoaderPNGPlugin.hpp"
#include "Example.hpp"
#include "ExampleFPSCameraController.hpp"
#include "ExampleCommonSettings.hpp"
#include "LevelBloomController.hpp"


//////////////////////////////////////////
#define DISTORTION_BUFFER_DIV 1u
// #define SHADOW_BUFFER_DIV 1u


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class BaseSceneExample
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(BaseSceneExample, EcsRenderScene);

    //////////////////////////////////////////
    BaseSceneExample::BaseSceneExample()
    {

    }

    //////////////////////////////////////////
    BaseSceneExample::~BaseSceneExample()
    {

        if (m_canvas)
        {
            UIElement2D* canvasUIElement = m_canvas->getEntityRaw()->getComponentRaw<UIElement2D>();
            if (canvasUIElement)
            {
                canvasUIElement->eventCursorPressIn.unsubscribe(this);
                canvasUIElement->eventCursorDrag.unsubscribe(this);
            }
        }

        if (SettingsManager::GetInstancePtr())
        {
            ExampleCommonSettings* exampleCommonSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ExampleCommonSettings>();
            exampleCommonSettings->getBloomEnabledChangedEvent().unsubscribe(this);
        }

        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.unsubscribe(this);
        Example::GetInstancePtr()->getMainRenderWindow()->eventRenderTargetResized.unsubscribe(this);
    }


    //////////////////////////////////////////
    bool BaseSceneExample::init(Vec2F const& _levelSize)
    {
        if (!EcsRenderScene::init(Example::GetInstancePtr()->getMainRenderWindow()))
            return false;

        ExampleCommonSettings* exampleCommonSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ExampleCommonSettings>();
        exampleCommonSettings->getBloomEnabledChangedEvent().subscribe(this, &BaseSceneExample::notifyExampleCommonSettingsChanged);

        Vec2U32 renderBufferSize = getMainRenderBufferSize();

        S32 samples = RenderSystem::GetCurrentInstancePtr()->getTextureMaxAntialiasingLevelSupport();

        if (samples > 0)
        {
            Debug::Log("RenderBuffer MSAA(x%d) creating...", samples);
            m_renderBufferMSAA = RenderBuffer::Create(
                {
                    renderBufferSize,
                    { PixelFormat::RGBA_F16, samples },
                    { PixelFormat::DEPTH_U24, samples }
                });
            if (m_renderBufferMSAA)
            {
                m_renderBufferMSAA->eventRenderBufferEndDraw.subscribe(
                    [&](RenderBuffer* _renderBuffer)
                {
                    MAZE_PROFILE_EVENT("RenderBufferMSAA end draw");

                    m_renderBuffer->blit(m_renderBufferMSAA);
                    m_renderBuffer->eventRenderBufferEndDraw(m_renderBuffer.get());
                });
                Debug::Log("RenderBuffer MSAA(x%d) created.", samples);
            }
            else
            {
                Debug::Log("RenderBuffer MSAA(x%d) creating failed.", samples);
            }
        }

        m_renderBuffer = RenderBuffer::Create(
            {
                renderBufferSize,
                PixelFormat::RGBA_F16,
                PixelFormat::DEPTH_U24
            });
        m_renderBuffer->setName("RenderBuffer");
        m_renderBuffer->getColorTexture2D()->setMinFilter(TextureFilter::Linear);
        m_renderBuffer->getColorTexture2D()->setMagFilter(TextureFilter::Linear);


        /*m_shadowBuffer = RenderBuffer::Create(
            {
                renderBufferSize / SHADOW_BUFFER_DIV,
                PixelFormat::None,
                PixelFormat::DEPTH_U24
            });
        m_shadowBuffer->setName("ShadowBuffer");
        m_shadowBuffer->getDepthTexture()->castRaw<Texture2D>()->setMinFilter(TextureFilter::Linear);
        m_shadowBuffer->getDepthTexture()->castRaw<Texture2D>()->setMagFilter(TextureFilter::Linear);
        m_shadowBuffer->getDepthTexture()->castRaw<Texture2D>()->setWrapS(TextureWrap::ClampToBorder);
        m_shadowBuffer->getDepthTexture()->castRaw<Texture2D>()->setWrapT(TextureWrap::ClampToBorder);
        m_shadowBuffer->getDepthTexture()->castRaw<Texture2D>()->setBorderColor(ColorU32::c_white);*/

        EntityPtr canvasEntity = createEntity("Canvas");
        m_canvas = canvasEntity->createComponent<Canvas>();
        m_canvas->setViewport(getMainViewport());
        m_canvas->setRenderTarget(Example::GetInstancePtr()->getMainRenderWindow());

        CanvasScalerPtr canvasScaler = canvasEntity->ensureComponent<CanvasScaler>();
        canvasScaler->setScaleMode(CanvasScalerScaleMode::ScaleWithViewportSize);
        canvasScaler->setScreenMatchMode(CanvasScalerScreenMatchMode::MatchWidthOrHeight);
        canvasScaler->setMatchWidthOrHeight(1.0f);
        canvasScaler->updateCanvasScale();


        UIElement2DPtr canvasUIElement = canvasEntity->ensureComponent<UIElement2D>();
        canvasUIElement->eventCursorPressIn.subscribe(this, &BaseSceneExample::processCursorPress);
        canvasUIElement->eventCursorDrag.subscribe(this, &BaseSceneExample::processCursorDrag);


        EntityPtr canvasUIEntity = createEntity("CanvasUI");
        m_canvasUI = canvasUIEntity->createComponent<Canvas>();
        m_canvasUI->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
        m_canvasUI->setRenderTarget(Example::GetInstancePtr()->getMainRenderWindow());
        m_canvasUI->setSortOrder(100);


        MaterialPtr postFXMaterial = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getMaterialManager()->getOrLoadMaterial("PostFX00.mzmaterial");
        m_renderColorSprite = SpriteHelper::CreateSprite(
            Sprite::Create(m_renderBuffer->getColorTexture2D()),
            m_canvas->getTransform()->getSize(),
            Vec2F32::c_zero,
            postFXMaterial,
            m_canvas->getTransform(),
            this);
        m_postFXMaterial = m_renderColorSprite->getMaterial();
        m_renderColorSprite->getTransform()->setZ(1000);
        m_renderColorSprite->getEntityRaw()->ensureComponent<Name>("RenderColorSprite");
        m_renderColorSprite->getEntityRaw()->ensureComponent<SizePolicy2D>();
        m_renderColorSprite->setPixelPerfect(true);


        m_hintText = SystemUIHelper::CreateSystemText(
            "",
            8,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Top,
            Vec2F32::c_zero,
            Vec2F32(10.0f, -10.0f),
            m_canvasUI->getTransform(),
            this,
            Vec2F32(0.0f, 1.0f),
            Vec2F32(0.0f, 1.0f));
        m_hintText->getTransform()->setZ(2000);
        m_hintText->setColor(ColorU32(255, 255, 255, 220));
        m_hintText->setSystemFont(SystemFontManager::GetCurrentInstancePtr()->getBuiltinSystemFont(BuiltinSystemFontType::DefaultOutlined));


        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.subscribe(this, &BaseSceneExample::notifyMainRenderWindowViewportChanged);
        Example::GetInstancePtr()->getMainRenderWindow()->eventRenderTargetResized.subscribe(this, &BaseSceneExample::notifyRenderTargetResized);


        // Light
        EntityPtr lightEntity = createEntity();
        m_mainLight3D = lightEntity->createComponent<Light3D>();
        m_mainLight3D->setColor(ColorU32(255, 244, 214));

        m_mainLight3D->getTransform()->setLocalPosition(0.0f, 5.0f, -5.0f);
        m_mainLight3D->getTransform()->setLocalRotation(0.408979f, 0.906161f, -0.068055f, -0.083529f);

        lightEntity->ensureComponent<Name>("Light");



        // FPS Controller
        EntityPtr fpsControllerEntity = createEntity();
        m_fpsController = fpsControllerEntity->ensureComponent<ExampleFPSCameraController>(m_canvas);
        m_fpsController->setLevelSize(_levelSize);
        m_fpsController->setYawAngle(Math::DegreesToRadians(180.0f));


        // Camera
        m_camera3D = m_fpsController->getCamera3D();
        m_camera3D->getTransform()->setLocalRotationDegrees(0.0f, 180.0f, 0.0f);
        m_camera3D->setRenderMask(m_camera3D->getRenderMask() & ~(S32)DefaultRenderMask::UserMask0);
        m_camera3D->getEntityRaw()->ensureComponent<Camera3DShadowBuffer>();
        // m_camera3D->setShadowBuffer(m_shadowBuffer);


        // Distortion render buffer
        {
            Debug::Log("Distortion Render Buffer creating...");
            m_distortionRenderBuffer = RenderBuffer::Create(
                {
                    renderBufferSize / DISTORTION_BUFFER_DIV,
                    PixelFormat::RGBA_F16
                });
            Debug::Log("Distortion Render Buffer created.");

            if (m_distortionRenderBuffer->getColorTexture()->getType() == TextureType::TwoDimensional)
            {
                m_distortionRenderBuffer->getColorTexture()->castRaw<Texture2D>()->setMinFilter(TextureFilter::Linear);
                m_distortionRenderBuffer->getColorTexture()->castRaw<Texture2D>()->setMagFilter(TextureFilter::Linear);
            }
        }

        EntityPtr distortionCameraEntity = createEntity("DistortionCamera");
        m_distortionCamera3D = distortionCameraEntity->createComponent<Camera3D>();
        m_distortionCamera3D->getTransform()->setParent(m_camera3D->getTransform());
        m_distortionCamera3D->setFOV(m_camera3D->getFOV());
        m_distortionCamera3D->setClearColor(ColorU32::c_black);
        m_distortionCamera3D->setClearColorFlag(true);
        m_distortionCamera3D->setRenderMask((S32)DefaultRenderMask::UserMask0);
        m_distortionCamera3D->setNearZ(m_camera3D->getNearZ());
        m_distortionCamera3D->setFarZ(m_camera3D->getFarZ());
        m_distortionCamera3D->setSortOrder(m_camera3D->getSortOrder() + 100);
        m_distortionCamera3D->setRenderTarget(m_distortionRenderBuffer);


        m_bloomController = LevelBloomController::Create(m_renderBuffer);

        updateRenderTarget();
        updateHintText();

        return true;
    }

    //////////////////////////////////////////
    void BaseSceneExample::notifyMainRenderWindowViewportChanged(Rect2F const& _mainRenderWindowViewport)
    {
        if (!Example::GetInstancePtr()->isMainWindowReadyToRender())
            return;

        m_canvas->setViewport(getMainViewport());
        m_canvasUI->setViewport(_mainRenderWindowViewport);

        if (!Example::GetInstancePtr()->isDebugEditorProgress())
        {
            updateRenderBuffersSize();

        }

        updateRenderTargetViewport();
    }

    //////////////////////////////////////////
    void BaseSceneExample::notifyRenderTargetResized(RenderTarget* _renderTarget)
    {
        Vec2U32 size = getMainRenderBufferSize();
        Debug::Log("Render target resized: %ux%u", size.x, size.y);

        if (!Example::GetInstancePtr()->isMainWindowReadyToRender())
            return;

        updateRenderBuffersSize();
    }

    //////////////////////////////////////////
    void BaseSceneExample::update(F32 _dt)
    {
        EcsRenderScene::update(_dt);

        ExampleCommonSettings* exampleCommonSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ExampleCommonSettings>();

        if (exampleCommonSettings->getBloomEnabled())
        {
            m_bloomController->update(_dt);
            m_renderColorSprite->getMaterial()->ensureUniform(
                MAZE_HS("u_bloomMap"),
                ShaderUniformType::UniformTexture2D)->set(
                    m_bloomController->getBloomRenderBuffer()->getColorTexture2D());
        }

        m_fpsController->setForward(InputManager::GetInstancePtr()->getKeyState(KeyCode::W));
        m_fpsController->setBackward(InputManager::GetInstancePtr()->getKeyState(KeyCode::S));
        m_fpsController->setRight(InputManager::GetInstancePtr()->getKeyState(KeyCode::A));
        m_fpsController->setLeft(InputManager::GetInstancePtr()->getKeyState(KeyCode::D));
        m_fpsController->setJump(InputManager::GetInstancePtr()->getKeyState(KeyCode::Space));
    }

    //////////////////////////////////////////
    void BaseSceneExample::processCursorPress(Vec2F32 const& _positionOS, CursorInputEvent const& _event)
    {
#if (MAZE_PLATFORM_MOBILE)
        if (_event.button == 0)
#else
        if (_event.button == 1)
#endif
        {
            m_cursorPositionLastFrame = _positionOS;
        }
    }

    //////////////////////////////////////////
    void BaseSceneExample::processCursorDrag(Vec2F32 const& _positionOS, CursorInputEvent const& _event)
    {
        Vec2F32 deltaPosition = _positionOS - m_cursorPositionLastFrame;

#if (MAZE_PLATFORM_MOBILE)
        if (_event.button == 0)
#else
        if (_event.button == 1)
#endif
        {
            F32 yawAngle = m_fpsController->getYawAngle();
            F32 pitchAngle = m_fpsController->getPitchAngle();
            yawAngle += deltaPosition.x * m_cursorDragDeltaKoef;
            pitchAngle -= deltaPosition.y * m_cursorDragDeltaKoef;
            pitchAngle = Math::Clamp(pitchAngle, -Math::c_halfPi, Math::c_halfPi);
            m_fpsController->setYawAngle(yawAngle);
            m_fpsController->setPitchAngle(pitchAngle);

            m_cursorPositionLastFrame = _positionOS;
        }
    }

    //////////////////////////////////////////
    void BaseSceneExample::updateHintText()
    {

    }

    //////////////////////////////////////////
    void BaseSceneExample::notifyExampleCommonSettingsChanged(bool const& _value)
    {
        updateRenderTarget();
        updateHintText();
    }

    //////////////////////////////////////////
    void BaseSceneExample::updateRenderBuffersSize()
    {
        Vec2F renderWindowSize = getMainRenderBufferSize();

        m_renderBuffer->setSize(renderWindowSize);

        if (m_renderBufferMSAA)
            m_renderBufferMSAA->setSize(renderWindowSize);

        // m_shadowBuffer->setSize(renderWindowSize / SHADOW_BUFFER_DIV);

        m_distortionRenderBuffer->setSize(renderWindowSize / DISTORTION_BUFFER_DIV);
    }

    //////////////////////////////////////////
    void BaseSceneExample::updateRenderTarget()
    {
        ExampleCommonSettings* exampleCommonSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ExampleCommonSettings>();

        if (exampleCommonSettings->getBloomEnabled())
        {
            m_renderColorSprite->getEntityRaw()->setActiveSelf(true);
            if (m_renderBufferMSAA)
                m_camera3D->setRenderTarget(m_renderBufferMSAA);
            else
                m_camera3D->setRenderTarget(m_renderBuffer);

            m_renderColorSprite->getMaterial()->ensureUniform(
                MAZE_HS("u_bloomMap"),
                ShaderUniformType::UniformTexture2D)->set(
                    m_renderBuffer->getColorTexture2D());


            m_postFXMaterial->setUniform(
                MAZE_HASHED_CSTRING("u_distortionMap"), m_distortionRenderBuffer->getColorTexture2D());
            m_postFXMaterial->setUniform(
                MAZE_HASHED_CSTRING("u_invAspectRatio"),
                (F32)m_renderBuffer->getHeight() / (F32)m_renderBuffer->getWidth());
            m_postFXMaterial->setUniform(
                MAZE_HASHED_CSTRING("u_baseMapTexelSize"),
                1.0f / (Vec2F)m_renderBuffer->getColorTexture()->cast<Texture2D>()->getSize());
        }
        else
        {
            m_renderColorSprite->getEntityRaw()->setActiveSelf(false);
            m_camera3D->setRenderTarget(
                Example::GetInstancePtr()->getMainRenderWindow());
        }

        updateRenderTargetViewport();
    }

    //////////////////////////////////////////
    void BaseSceneExample::updateRenderTargetViewport()
    {
        ExampleCommonSettings* exampleCommonSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ExampleCommonSettings>();

        if (exampleCommonSettings->getBloomEnabled())
            m_camera3D->setViewport(Rect2F(0.0f, 0.0f, 1.0f, 1.0f));
        else
            m_camera3D->setViewport(getMainViewport());
    }

    //////////////////////////////////////////
    Rect2F BaseSceneExample::getMainViewport()
    {
        return Example::GetInstancePtr()->getMainRenderWindowViewport();
    }

    //////////////////////////////////////////
    Vec2U32 BaseSceneExample::getMainRenderBufferSize()
    {
        return Vec2U32(Vec2F32(
            Example::GetInstancePtr()->getMainRenderWindow()->getRenderTargetSize()) * getMainViewport().size);
    }


} // namespace Maze
//////////////////////////////////////////
