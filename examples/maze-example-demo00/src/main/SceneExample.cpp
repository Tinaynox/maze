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
#include "SceneExample.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/ecs/components/MazeTerrainMesh3D.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/MazeGPUTextureBuffer.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeWaterRenderer3D.hpp"
#include "maze-particles/ecs/components/MazeParticleSystem3D.hpp"
#include "maze-particles/MazeParticleSystemParameterF32.hpp"
#include "maze-particles/MazeParticleSystemParameterColor.hpp"
#include "maze-particles/managers/MazeParticlesManager.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-plugin-profiler-view/MazeProfilerViewPlugin.hpp"
#include "maze-plugin-profiler-view/settings/MazeProfilerViewSettings.hpp"
#include "maze-plugin-loader-png/MazeLoaderPNGPlugin.hpp"
#include "maze-plugin-water/MazeWaterPlugin.hpp"
#include "maze-plugin-water/ecs/systems/MazeRenderWaterSystem.hpp"
#include "main/LevelBloomController.hpp"
#include "Example.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    String GetExampleName()
    {
        return "Sky, Terrain and Water";
    }

    //////////////////////////////////////////
    void LoadFirstExampleScene(SceneManager* _sceneManager)
    {
        _sceneManager->loadScene<SceneExample>();
    }

    //////////////////////////////////////////
    bool IsLoadAllAssets()
    {
        return false;
    }

    //////////////////////////////////////////
    bool LoadPlugins()
    {
        MAZE_LOAD_PLATFORM_PLUGIN(ProfilerView, "maze-plugin-profiler-view");
        MAZE_LOAD_PLATFORM_PLUGIN(LoaderPNG, "maze-plugin-loader-png");
        MAZE_LOAD_PLATFORM_PLUGIN(Water, "maze-plugin-water");

#if (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)
        if (SettingsManager::GetInstancePtr()->getSettingsRaw<ProfilerViewSettings>())
            SettingsManager::GetInstancePtr()->getSettingsRaw<ProfilerViewSettings>()->setActive(true);
#endif

        return true;
    }


    //////////////////////////////////////////
    // Class SceneExample
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneExample, ECSRenderScene);

    //////////////////////////////////////////
    SceneExample::SceneExample()
    {
    }

    //////////////////////////////////////////
    SceneExample::~SceneExample()
    {
        InputManager* inputManager = InputManager::GetInstancePtr();
        if (inputManager)
        {
            inputManager->eventMouse.unsubscribe(this);
            inputManager->eventTouch.unsubscribe(this);
        }

        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.unsubscribe(this);
        Example::GetInstancePtr()->getMainRenderWindow()->eventRenderTargetResized.unsubscribe(this);
    }

    //////////////////////////////////////////
    SceneExamplePtr SceneExample::Create()
    {
        SceneExamplePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneExample, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SceneExample::init()
    {
        if (!ECSRenderScene::init(Example::GetInstancePtr()->getMainRenderWindow()))
            return false;

        InputManager* inputManager = InputManager::GetInstancePtr();
        inputManager->eventMouse.subscribe(this, &SceneExample::notifyMouse);
        inputManager->eventTouch.subscribe(this, &SceneExample::notifyTouch);

        Vec2DU renderBufferSize = Example::GetInstancePtr()->getMainRenderWindowAbsoluteSize();
        m_renderBuffer = RenderBuffer::Create(
            {
                renderBufferSize,
                PixelFormat::RGBA_F16,
                PixelFormat::DEPTH_U24
            });
        m_renderBuffer->getColorTexture2D()->setMinFilter(TextureFilter::Linear);
        m_renderBuffer->getColorTexture2D()->setMagFilter(TextureFilter::Linear);


        RenderWaterSystemPtr waterSystem = m_world->getSystem<RenderWaterSystem>();
        waterSystem->resizeBuffers(renderBufferSize);


        EntityPtr canvasEntity = createEntity("Canvas");
        m_canvas = canvasEntity->createComponent<Canvas>();
        m_canvas->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
        m_canvas->setRenderTarget(Example::GetInstancePtr()->getMainRenderWindow());
        m_canvas->setClearColorFlag(false);
        m_canvas->setClearColor(ColorU32::c_zero);

        CanvasScalerPtr canvasScaler = canvasEntity->ensureComponent<CanvasScaler>();
        canvasScaler->setScaleMode(CanvasScaler::ScaleMode::ScaleWithViewportSize);
        canvasScaler->setScreenMatchMode(CanvasScaler::ScreenMatchMode::MatchWidthOrHeight);
        canvasScaler->setMatchWidthOrHeight(1.0f);
        canvasScaler->updateCanvasScale();


        MaterialPtr const& postFXMaterial = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getMaterialManager()->getMaterial("PostFX00.mzmaterial");
        m_renderColorSprite = SpriteHelper::CreateSprite(
            Sprite::Create(m_renderBuffer->getColorTexture2D()),
            m_canvas->getTransform()->getSize(),
            Vec2DF::c_zero,
            postFXMaterial,
            m_canvas->getTransform(),
            this);
        m_renderColorSprite->getTransform()->setZ(1000);
        m_renderColorSprite->getEntityRaw()->ensureComponent<Name>("RenderColorSprite");
        m_renderColorSprite->getEntityRaw()->ensureComponent<SizePolicy2D>();

#if (0)
        
        m_testSprite1 = SpriteHelper::CreateSprite(
            SpritePtr(),
            {128, 128 },
            Vec2DF::c_zero,
            GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getMaterialManager()->getColorTextureMaterial(),
            m_canvas->getTransform(),
            this,
            { 0.0f, 1.0f },
            { 0.0f, 1.0f });
        m_testSprite1->getTransform()->setZ(10000);

        m_testSprite2 = SpriteHelper::CreateSprite(
            SpritePtr(),
            { 128, 128 },
            { 128, 0 },
            GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getMaterialManager()->getColorTextureMaterial(),
            m_canvas->getTransform(),
            this,
            { 0.0f, 1.0f },
            { 0.0f, 1.0f });
        m_testSprite2->getTransform()->setZ(10000);
#endif

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.subscribe(this, &SceneExample::notifyMainRenderWindowViewportChanged);
        Example::GetInstancePtr()->getMainRenderWindow()->eventRenderTargetResized.subscribe(this, &SceneExample::notifyRenderTargetResized);


        // Light
        EntityPtr lightEntity = createEntity("Light");
        Light3DPtr mainLight3D = lightEntity->createComponent<Light3D>();
        mainLight3D->setColor(ColorU32(255, 244, 214));
        mainLight3D->getTransform()->setLocalDirection(0.577f, -0.577f, 0.577f);
        mainLight3D->getTransform()->setLocalPosition(0.0f, 5.0f, -5.0f);


        // Camera
        EntityPtr cameraEntity = createEntity("Camera");
        m_camera3D = cameraEntity->createComponent<Camera3D>();
        m_camera3D->getTransform()->setLocalPosition(Vec3DF(8.0f, 4.0f, 8.0f));
        m_camera3D->setFOV(Math::DegreesToRadians(30));
        m_camera3D->setClearColorFlag(false);
        m_camera3D->setClearSkyBoxFlag(true);
        m_camera3D->setRenderTarget(m_renderBuffer);
        m_camera3D->setRenderMask(m_camera3D->getRenderMask() & ~(S32)DefaultRenderMask::Gizmos);
        m_camera3D->setNearZ(0.01f);
        m_camera3D->setFarZ(100.0f);

        m_bloomController = LevelBloomController::Create(this);

        getLightingSettings()->setSkyBoxMaterial("Skybox00.mzmaterial");


        // Terrain
        EntityPtr terrainEntity = createEntity("Terrain");
        TerrainMesh3DPtr terrainMesh = terrainEntity->ensureComponent<TerrainMesh3D>();
        terrainMesh->setCellsCount({ 50, 50 });
        terrainMesh->setSize({ 20, 20 });
        terrainMesh->setHeight(2.25f);
        terrainMesh->setHeightMap(renderSystem->getTextureManager()->getTexture2D("Heightmap02.mztexture"));
        MeshRendererPtr terrainMeshRenderer = terrainEntity->ensureComponent<MeshRenderer>();
        terrainMeshRenderer->setMaterial("Terrain00.mzmaterial");


        // Water
        EntityPtr waterEntity = createEntity("Water");
        WaterRenderer3DPtr waterRenderer = waterEntity->ensureComponent<WaterRenderer3D>();
        Transform3DPtr waterTransform = waterEntity->ensureComponent<Transform3D>();
        waterTransform->setLocalScale({ 20.0f, 20.0f, 1.0f });
        waterTransform->setLocalY(0.75f);
        waterTransform->setLocalRotation(Quaternion(Math::DegreesToRadians(90), Vec3DF::c_unitX));
        MeshRendererPtr waterMeshRenderer = waterEntity->ensureComponent<MeshRenderer>();
        waterMeshRenderer->setRenderMesh(renderSystem->getRenderMeshManager()->getDefaultQuadMesh());
        waterRenderer->setMaterial("Water00.mzmaterial");
        waterRenderer->getMeshRenderer()->getMaterial()->getFirstRenderPass()->setRenderQueueIndex(2500);
        

        // Barrel
        {
            EntityPtr barrelEntity = createEntity("Barrel");
            Transform3DPtr barrelTransform = barrelEntity->createComponent<Transform3D>();
            MeshRendererPtr barrelMeshRenderer = barrelEntity->createComponent<MeshRenderer>();
            barrelMeshRenderer->setRenderMesh("Barrel00.obj");
            barrelMeshRenderer->setMaterial("Barrel00.mzmaterial");
            barrelTransform->setLocalY(0.05f);
            barrelTransform->setLocalZ(-1.0f);
            barrelTransform->setLocalRotation(Quaternion(Math::c_halfPi, 0.0f, 0.0f));
            barrelTransform->setLocalScale({ 0.025f, 0.025f, 0.025f });
        }

        // Barrel
        {
            EntityPtr barrelEntity = createEntity("Barrel");
            Transform3DPtr barrelTransform = barrelEntity->createComponent<Transform3D>();
            MeshRendererPtr barrelMeshRenderer = barrelEntity->createComponent<MeshRenderer>();
            barrelMeshRenderer->setRenderMesh("Barrel00.obj");
            barrelMeshRenderer->setMaterial("Barrel00.mzmaterial");
            barrelTransform->setLocalPosition({2.5f, 0.3f, -5.0f});
            barrelTransform->setLocalRotation(Quaternion(0.0f, Math::c_halfPi, Math::c_halfPi * 0.1f));
            barrelTransform->setLocalScale({ 0.025f, 0.025f, 0.025f });
        }

        // Barrel
        {
            EntityPtr barrelEntity = createEntity("Barrel");
            Transform3DPtr barrelTransform = barrelEntity->createComponent<Transform3D>();
            MeshRendererPtr barrelMeshRenderer = barrelEntity->createComponent<MeshRenderer>();
            barrelMeshRenderer->setRenderMesh("Barrel00.obj");
            barrelMeshRenderer->setMaterial("Barrel00.mzmaterial");
            barrelTransform->setLocalPosition({ -1.0f, 1.8f, -7.0f });
            // barrelTransform->setLocalRotation(Quaternion(0.0f, Math::c_halfPi + 0.1f, Math::c_halfPi));
            barrelTransform->setLocalRotationDegrees(0.0f, 180.0f, 90.0f);
            barrelTransform->setLocalScale({ 0.025f, 0.025f, 0.025f });
        }

        createParticleSystem();

        return true;
    }

    //////////////////////////////////////////
    void SceneExample::notifyMainRenderWindowViewportChanged(Rect2DF const& _mainRenderWindowViewport)
    {
        if (!Example::GetInstancePtr()->isMainWindowReadyToRender())
            return;

        m_canvas->setViewport(_mainRenderWindowViewport);

        if(!Example::GetInstancePtr()->isDebugEditorProgress())
            m_renderBuffer->setSize(Example::GetInstancePtr()->getMainRenderWindowAbsoluteSize());
    }

    //////////////////////////////////////////
    void SceneExample::notifyRenderTargetResized(RenderTarget* _renderTarget)
    {
        Vec2DU size = Example::GetInstancePtr()->getMainRenderWindowAbsoluteSize();
        Debug::Log("Render target resized: %ux%u", size.x, size.y);

        if (!Example::GetInstancePtr()->isMainWindowReadyToRender())
            return;

        m_renderBuffer->setSize(size);
    }

    //////////////////////////////////////////
    void SceneExample::update(F32 _dt)
    {
        ECSRenderScene::update(_dt);

        m_bloomController->update(_dt);
        m_renderColorSprite->getMaterial()->ensureUniform(
            "u_bloomMap",
            ShaderUniformType::UniformTexture2D)->set(
                m_bloomController->getBloomRenderBuffer()->getColorTexture2D());


        Quaternion q = Quaternion::Slerp(
            24.0f * _dt,
            m_camera3D->getTransform()->getLocalRotation(),
            Quaternion(m_pitchAngle, m_yawAngle, 0.0f));
        m_camera3D->getTransform()->setLocalRotation(q);


        if (m_particleSystem)
        {
            static F32 timer = 0.0f;
            timer += _dt;
            F32 x = 2 * Math::Sin(1.4f * timer) + 2.0f;
            F32 z = 2 * Math::Cos(1.4f * timer) - 2.0f;
            m_particleSystem->getTransform()->setLocalPosition(
                {x, 2.5f + Math::Sin(2.0f * timer), z});
        }
        
        if (m_testSprite1 && m_testSprite2)
        {
            RenderWaterSystemPtr waterSystem = m_world->getSystem<RenderWaterSystem>();
            if (waterSystem && waterSystem->getReflectionBuffer() && waterSystem->getRefractionBuffer())
            {
                Texture2DPtr reflectionTexture = waterSystem->getReflectionBuffer()->getColorTexture2D();
                Texture2DPtr refractionTexture = waterSystem->getRefractionBuffer()->getColorTexture2D();

                if (!m_testSprite1->getSprite() || m_testSprite1->getSprite()->getTexture() != reflectionTexture)
                    m_testSprite1->setSprite(Sprite::Create(reflectionTexture));

                if (!m_testSprite2->getSprite() || m_testSprite2->getSprite()->getTexture() != refractionTexture)
                    m_testSprite2->setSprite(Sprite::Create(refractionTexture));
            }
        }
    }

    //////////////////////////////////////////
    void SceneExample::createParticleSystem()
    {
        // Particle System
        EntityPtr psEntity = createEntity();
        ParticleSystem3DPtr ps = psEntity->ensureComponent<ParticleSystem3D>();
        m_particleSystem = ps;
        ps->getTransform()->rotate(Vec3DF::c_unitX, -Math::c_halfPi);
        ps->getTransform()->setLocalY(2.0f);

        ps->getMainModule().setTransformPolicy(ParticleSystemSimulationSpace::World);
        ps->getMainModule().getLifetime().setConstant(1.0f);
        ps->getMainModule().getSpeed().setConstant(1.0f);
        ps->getMainModule().getRotation().setRandomBetweenConstants(0.0f, Math::c_twoPi);
        ps->getMainModule().getGravity().setConstant(0.0f);
        ps->getMainModule().getSize().setRandomBetweenConstants(0.5f, 1.0f);
        ps->getMainModule().getColor().setColor({ 1.2f * 63.0f / 255.0f, 1.2f * 255.0f / 255.0f, 1.2f * 79.0f / 255.0f, 0.4f });
        //ps->getMainModule().getSize().setConstant(0.5f);
        ps->getMainModule().getRotationOverLifetime().enabled = true;

        MaterialPtr material = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getMaterialManager()->getMaterial("Fireball00.mzmaterial");

        ps->getRendererModule().getTextureSheetAnimation().enabled = true;
        ps->getRendererModule().getTextureSheetAnimation().tiles = Vec2DS(7, 7);
        ps->getRendererModule().getTextureSheetAnimation().startFrame.setConstant(0);

        {
            AnimationCurve curve;
            curve.setMode(AnimationCurve::EvaluateMode::Linear);
            curve.addKey(0.0f, 0.0f);
            curve.addKey(1.0f, 49.0f);
            ps->getRendererModule().getTextureSheetAnimation().frameOverTime.setCurve(curve);
        }

        {
            ColorGradient gradient;
            gradient.addKeyRGB(0.0f, Vec3DF::c_one);
            gradient.addKeyAlpha(0.23f, 0.0f);
            gradient.addKeyAlpha(0.42f, 1.0f);
            gradient.addKeyAlpha(0.71f, 1.0f);
            gradient.addKeyAlpha(0.88f, 0.0f);
            ps->getMainModule().getColorOverLifetime().setEnabled(true);
            ps->getMainModule().getColorOverLifetime().setParameter({ gradient });
        }

        ps->getRendererModule().setParticlesMaxCount(10000);
        ps->getMainModule().getEmission().emissionPerSecond.setConstant(150.0f);

        ps->getShapeModule().setType(ParticleSystem3DZoneType::Cone);
        ps->getShapeModule().getZoneData().cone.angle = 0.436f;
        ps->getShapeModule().getZoneData().cone.length = 0.001f;
        ps->getShapeModule().getZoneData().cone.radius = 0.119f;
        ps->getShapeModule().setShapeVolume(true);

        ps->setMaterial(material);
        ps->play();
        psEntity->ensureComponent<Name>("ParticleSystem");
    }

    //////////////////////////////////////////
    void SceneExample::notifyMouse(InputEventMouseData const& _data)
    {
        switch (_data.type)
        {
            case InputEventMouseType::Move:
            {
                Vec2DF cursorPosition = Vec2DF((F32)_data.x, (F32)_data.y);

                if (m_cursorDrag)
                {
                    Vec2DF deltaPosition = cursorPosition - m_cursorPositionLastFrame;

                    m_yawAngle += deltaPosition.x * 0.0075f * 0.25f;
                    m_pitchAngle -= deltaPosition.y * 0.0075f * 0.25f;
                }

                m_cursorPositionLastFrame = cursorPosition;
                break;
            }
            case InputEventMouseType::ButtonDown:
            {
                if (_data.buttonId == 1)
                {
                    Vec2DF cursorPosition = Vec2DF((F32)_data.x, (F32)_data.y);
                    Rect2DF viewportRect(
                        m_canvas->getViewport().position.x * m_renderTarget->getRenderTargetSize().x,
                        m_canvas->getViewport().position.y * m_renderTarget->getRenderTargetSize().y,
                        m_canvas->getViewport().size.x * m_renderTarget->getRenderTargetSize().x,
                        m_canvas->getViewport().size.y * m_renderTarget->getRenderTargetSize().y);

                    m_cursorPositionLastFrame = cursorPosition;

                    if (viewportRect.contains(cursorPosition))
                    {
                        m_cursorDrag = true;
                    }
                }
                break;
            }
            case InputEventMouseType::ButtonUp:
            {
                if (_data.buttonId == 1)
                {
                    m_cursorDrag = false;
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }

    //////////////////////////////////////////
    void SceneExample::notifyTouch(InputEventTouchData const& _data)
    {
        switch (_data.type)
        {
            case InputEventTouchType::Move:
            {
                Vec2DF cursorPosition = Vec2DF((F32)_data.x, (F32)_data.y);

                if (m_cursorDrag)
                {
                    Vec2DF deltaPosition = cursorPosition - m_cursorPositionLastFrame;

                    m_yawAngle += deltaPosition.x * 0.0075f * 0.25f;
                    m_pitchAngle -= deltaPosition.y * 0.0075f * 0.25f;
                }

                m_cursorPositionLastFrame = cursorPosition;
                break;
            }
            case InputEventTouchType::Press:
            {
                if (_data.index == 0)
                {
                    Vec2DF cursorPosition = Vec2DF((F32)_data.x, (F32)_data.y);
                    Rect2DF viewportRect(
                        m_canvas->getViewport().position.x * m_renderTarget->getRenderTargetSize().x,
                        m_canvas->getViewport().position.y * m_renderTarget->getRenderTargetSize().y,
                        m_canvas->getViewport().size.x * m_renderTarget->getRenderTargetSize().x,
                        m_canvas->getViewport().size.y * m_renderTarget->getRenderTargetSize().y);

                    m_cursorPositionLastFrame = cursorPosition;

                    if (viewportRect.contains(cursorPosition))
                    {
                        m_cursorDrag = true;
                    }
                }
                break;
            }
            case InputEventTouchType::Release:
            {
                if (_data.index == 0)
                {
                    m_cursorDrag = false;
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }

} // namespace Maze
//////////////////////////////////////////
