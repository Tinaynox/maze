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
#include "MazeEngineHeader.hpp"
#include "scenes/MazeScenePlayerBase.hpp"
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
#include "ecs/components/MazeEnginePlayer.hpp"
#include "MazeEngine.hpp"
#include "settings/MazePlayerSettings.hpp"
#include "utils/MazeBloomController.hpp"


//////////////////////////////////////////
#define DISTORTION_BUFFER_DIV 1u
// #define SHADOW_BUFFER_DIV 1u


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ScenePlayerBase
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ScenePlayerBase, EcsRenderScene);

    //////////////////////////////////////////
    ScenePlayerBase::ScenePlayerBase()
    {

    }

    //////////////////////////////////////////
    ScenePlayerBase::~ScenePlayerBase()
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
            PlayerSettings* playerSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<PlayerSettings>();
            playerSettings->getBloomEnabledChangedEvent().unsubscribe(this);
        }

        if (m_enginePlayer)
            m_enginePlayer->eventEnginePlayerCurrentCameraChanged.unsubscribe(this);

        if (m_renderTarget)
            m_renderTarget->eventRenderTargetResized.unsubscribe(this);
    }


    //////////////////////////////////////////
    bool ScenePlayerBase::init(ScenePlayerBaseConfig const& _config)
    {
        m_config = _config;
        if (!EcsRenderScene::init(m_config.renderTarget))
            return false;

        EntityPtr enginePlayerEntity = createEntity(MAZE_HCS("EnginePlayer"));
        m_enginePlayer = enginePlayerEntity->createComponent<EnginePlayer>();
        m_enginePlayer->eventEnginePlayerCurrentCameraChanged.subscribe(this, &ScenePlayerBase::notifyEnginePlayerCurrentCameraChanged);

        PlayerSettings* playerSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<PlayerSettings>();
        playerSettings->getBloomEnabledChangedEvent().subscribe(this, &ScenePlayerBase::notifyPlayerSettingsChanged);

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
        m_canvas->setRenderTarget(m_renderTarget);

        CanvasScalerPtr canvasScaler = canvasEntity->ensureComponent<CanvasScaler>();
        canvasScaler->setScaleMode(CanvasScalerScaleMode::ScaleWithViewportSize);
        canvasScaler->setScreenMatchMode(CanvasScalerScreenMatchMode::MatchWidthOrHeight);
        canvasScaler->setMatchWidthOrHeight(1.0f);
        canvasScaler->updateCanvasScale();


        UIElement2DPtr canvasUIElement = canvasEntity->ensureComponent<UIElement2D>();


        EntityPtr canvasUIEntity = createEntity("CanvasUI");
        m_canvasUI = canvasUIEntity->createComponent<Canvas>();
        m_canvasUI->setViewport(getMainViewport());
        m_canvasUI->setRenderTarget(m_renderTarget);
        m_canvasUI->setSortOrder(100);


        MaterialPtr postFXMaterial = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getMaterialManager()->getOrLoadMaterial(
            _config.postFXMaterialName);
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


        m_renderTarget->eventRenderTargetResized.subscribe(this, &ScenePlayerBase::notifyRenderTargetResized);


        // Light
        /*
        EntityPtr lightEntity = createEntity();
        m_mainLight3D = lightEntity->createComponent<Light3D>();
        m_mainLight3D->setColor(ColorU32(255, 244, 214));
        
        m_mainLight3D->getTransform()->setLocalPosition(0.0f, 5.0f, -5.0f);
        m_mainLight3D->getTransform()->setLocalRotation(0.408979f, 0.906161f, -0.068055f, -0.083529f);
        
        lightEntity->ensureComponent<Name>("Light");
        */

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
        m_distortionCamera3D->setRenderMask((S32)DefaultRenderMask::UserMask0);

        m_bloomController = BloomController::Create(
            m_renderBuffer,
            HashedCString(m_config.bloomMaterialName.c_str()));

        updateRenderTarget();

        return true;
    }

    //////////////////////////////////////////
    void ScenePlayerBase::updateViewport(bool _updateRenderBufferSize)
    {
        if (!m_renderTarget->isReadyToRender())
            return;

        m_canvas->setViewport(getMainViewport());
        m_canvasUI->setViewport(getMainViewport());

        if (_updateRenderBufferSize)
        {
            updateRenderBuffersSize();

        }

        updateRenderTargetViewport();
    }

    //////////////////////////////////////////
    void ScenePlayerBase::notifyRenderTargetResized(RenderTarget* _renderTarget)
    {
        Vec2U32 size = getMainRenderBufferSize();
        Debug::Log("Render target resized: %ux%u", size.x, size.y);

        if (!m_renderTarget->isReadyToRender())
            return;

        updateRenderBuffersSize();
    }

    //////////////////////////////////////////
    void ScenePlayerBase::update(F32 _dt)
    {
        EcsRenderScene::update(_dt);

        PlayerSettings* playerSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<PlayerSettings>();

        if (playerSettings->getBloomEnabled())
        {
            m_bloomController->update(_dt);
            m_renderColorSprite->getMaterial()->ensureUniform(
                MAZE_HS("u_bloomMap"),
                ShaderUniformType::UniformTexture2D)->set(
                    m_bloomController->getBloomRenderBuffer()->getColorTexture2D());
        }

    }

    //////////////////////////////////////////
    void ScenePlayerBase::notifyPlayerSettingsChanged(bool const& _value)
    {
        updateRenderTarget();
    }

    //////////////////////////////////////////
    void ScenePlayerBase::updateRenderBuffersSize()
    {
        Vec2F renderWindowSize = getMainRenderBufferSize();

        m_renderBuffer->setSize(renderWindowSize);

        if (m_renderBufferMSAA)
            m_renderBufferMSAA->setSize(renderWindowSize);

        m_distortionRenderBuffer->setSize(renderWindowSize / DISTORTION_BUFFER_DIV);

        /*if (m_shadowBuffer)
            m_shadowBuffer->setSize(renderWindowSize / SHADOW_BUFFER_DIV);*/
    }

    //////////////////////////////////////////
    void ScenePlayerBase::updateRenderTarget()
    {
        if (!m_enginePlayer->getCurrentCamera3D())
            return;

        //m_enginePlayer->getCurrentCamera3D()->setShadowBuffer(m_shadowBuffer);

        PlayerSettings* playerSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<PlayerSettings>();

        if (playerSettings->getBloomEnabled())
        {
            m_renderColorSprite->getEntityRaw()->setActiveSelf(true);
            if (m_renderBufferMSAA)
                m_enginePlayer->getCurrentCamera3D()->setRenderTarget(m_renderBufferMSAA);
            else
                m_enginePlayer->getCurrentCamera3D()->setRenderTarget(m_renderBuffer);

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
            m_enginePlayer->getCurrentCamera3D()->setRenderTarget(
                m_renderTarget);
        }

        updateRenderTargetViewport();
    }

    //////////////////////////////////////////
    void ScenePlayerBase::updateDistortionCamera()
    {
        Camera3DPtr const& camera = m_enginePlayer->getCurrentCamera3D();
        if (!camera || !m_distortionCamera3D)
            return;

        m_distortionCamera3D->getTransform()->setParent(camera->getTransform());
        m_distortionCamera3D->setFOV(camera->getFOV());
        m_distortionCamera3D->setClearColor(ColorU32::c_black);
        m_distortionCamera3D->setClearColorFlag(true);
        m_distortionCamera3D->setNearZ(camera->getNearZ());
        m_distortionCamera3D->setFarZ(camera->getFarZ());
        m_distortionCamera3D->setSortOrder(camera->getSortOrder() + 100);
        m_distortionCamera3D->setRenderTarget(m_distortionRenderBuffer);
    }

    //////////////////////////////////////////
    void ScenePlayerBase::updateRenderTargetViewport()
    {
        Camera3DPtr const& camera = m_enginePlayer->getCurrentCamera3D();
        if (!camera)
            return;

        PlayerSettings* playerSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<PlayerSettings>();

        if (playerSettings->getBloomEnabled())
            camera->setViewport(Rect2F(0.0f, 0.0f, 1.0f, 1.0f));
        else
            camera->setViewport(getMainViewport());
    }

    //////////////////////////////////////////
    Rect2F ScenePlayerBase::getMainViewport()
    {
        return Rect2F(0.0f, 0.0f, 1.0f, 1.0f);
    }

    //////////////////////////////////////////
    Vec2U32 ScenePlayerBase::getMainRenderBufferSize()
    {
        return Vec2U32(Vec2F32(
            m_renderTarget->getRenderTargetSize()) * getMainViewport().size);
    }

    //////////////////////////////////////////
    void ScenePlayerBase::notifyEnginePlayerCurrentCameraChanged(Camera3DPtr const& _camera)
    {
        updateRenderTarget();
        updateDistortionCamera();
    }

} // namespace Maze
//////////////////////////////////////////
