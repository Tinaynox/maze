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
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
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
#include "maze-graphics/managers/MazeSystemFontManager.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeWaterRenderer3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-particles/ecs/components/MazeParticleSystem3D.hpp"
#include "maze-particles/MazeParticleSystemParameterF32.hpp"
#include "maze-particles/MazeParticleSystemParameterColor.hpp"
#include "maze-particles/managers/MazeParticlesManager.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/ecs/components/MazeToggleButton2D.hpp"
#include "maze-ui/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"
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
#include "maze-plugin-water/ecs/components/MazeRenderWaterController.hpp"
#include "LevelBloomController.hpp"
#include "maze-plugin-console/MazeConsoleService.hpp"
#include "Demo00Settings.hpp"
#include "ExampleCommonSettings.hpp"
#include "Example.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneExample
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneExample, EcsRenderScene);

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

        if (SettingsManager::GetInstancePtr())
        {
            ExampleCommonSettings* exampleCommonSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ExampleCommonSettings>();
            exampleCommonSettings->getBloomEnabledChangedEvent().unsubscribe(this);

            Demo00Settings* exampleSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<Demo00Settings>();
            exampleSettings->getDebugMenuChangedEvent().unsubscribe(this);
            exampleSettings->getParticlesEnabledChangedEvent().unsubscribe(this);
            exampleSettings->getWaterEnabledChangedEvent().unsubscribe(this);
            exampleSettings->getExampleWaterRenderModeChangedEvent().unsubscribe(this);
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
        if (!EcsRenderScene::init(Example::GetInstancePtr()->getMainRenderWindow()))
            return false;

        ExampleCommonSettings* exampleCommonSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ExampleCommonSettings>();
        exampleCommonSettings->getBloomEnabledChangedEvent().subscribe(this, &SceneExample::notifyDemo00SettingsChanged);

        Demo00Settings* exampleSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<Demo00Settings>();
        exampleSettings->getDebugMenuChangedEvent().subscribe(this, &SceneExample::notifyDemo00SettingsChanged);
        exampleSettings->getParticlesEnabledChangedEvent().subscribe(this, &SceneExample::notifyDemo00SettingsChanged);
        exampleSettings->getWaterEnabledChangedEvent().subscribe(this, &SceneExample::notifyDemo00SettingsChanged);
        exampleSettings->getExampleWaterRenderModeChangedEvent().subscribe(this, &SceneExample::notifyDemo00SettingsChanged);


        InputManager* inputManager = InputManager::GetInstancePtr();
        inputManager->eventMouse.subscribe(this, &SceneExample::notifyMouse);
        inputManager->eventTouch.subscribe(this, &SceneExample::notifyTouch);

        Vec2U32 renderBufferSize = Example::GetInstancePtr()->getMainRenderWindowAbsoluteSize();
        m_renderBuffer = RenderBuffer::Create(
            {
                renderBufferSize,
                PixelFormat::RGBA_F16,
                PixelFormat::DEPTH_U24
            });
        m_renderBuffer->setName("RenderBuffer");
        m_renderBuffer->getColorTexture2D()->setMinFilter(TextureFilter::Linear);
        m_renderBuffer->getColorTexture2D()->setMagFilter(TextureFilter::Linear);


        m_renderWaterController = createAndAddEntityWithComponent<RenderWaterController>("RenderWaterController");
        m_renderWaterController->resizeBuffers(renderBufferSize);


        EntityPtr canvasEntity = createEntity("Canvas");
        m_canvas = canvasEntity->createComponent<Canvas>();
        m_canvas->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
        m_canvas->setRenderTarget(Example::GetInstancePtr()->getMainRenderWindow());
        m_canvas->setClearColorFlag(false);
        m_canvas->setClearColor(ColorU32::c_blackSoft);

        CanvasScalerPtr canvasScaler = canvasEntity->ensureComponent<CanvasScaler>();
        canvasScaler->setScaleMode(CanvasScalerScaleMode::ScaleWithViewportSize);
        canvasScaler->setScreenMatchMode(CanvasScalerScreenMatchMode::MatchWidthOrHeight);
        canvasScaler->setMatchWidthOrHeight(1.0f);
        canvasScaler->updateCanvasScale();


        MaterialPtr const& postFXMaterial = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getMaterialManager()->getMaterial("PostFX00.mzmaterial");
        m_renderColorSprite = SpriteHelper::CreateSprite(
            Sprite::Create(m_renderBuffer->getColorTexture2D()),
            m_canvas->getTransform()->getSize(),
            Vec2F32::c_zero,
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
            Vec2F32::c_zero,
            GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getMaterialManager()->getSpriteMaterial(),
            m_canvas->getTransform(),
            this,
            { 0.0f, 1.0f },
            { 0.0f, 1.0f });
        m_testSprite1->getTransform()->setZ(10000);

        m_testSprite2 = SpriteHelper::CreateSprite(
            SpritePtr(),
            { 128, 128 },
            { 128, 0 },
            GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getMaterialManager()->getSpriteMaterial(),
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
        m_camera3D->getTransform()->setLocalPosition(Vec3F32(8.0f, 4.0f, 8.0f));
        m_camera3D->setFOV(Math::DegreesToRadians(30));
        m_camera3D->setClearColorFlag(false);
        m_camera3D->setClearSkyBoxFlag(true);
        m_camera3D->setRenderTarget(m_renderBuffer);
        m_camera3D->setRenderMask(m_camera3D->getRenderMask() & ~(S32)DefaultRenderMask::Gizmos);
        m_camera3D->setNearZ(0.01f);
        m_camera3D->setFarZ(100.0f);

        m_bloomController = LevelBloomController::Create(m_renderBuffer);
        updateBloom();

        getLightingSettings()->setSkyBoxMaterial("Skybox00.mzmaterial");


        // Terrain
        EntityPtr terrainEntity = createEntity("Terrain");
        m_terrainMesh = terrainEntity->ensureComponent<TerrainMesh3D>();
        m_terrainMesh->setCellsCount({ 50, 50 });
        m_terrainMesh->setSize({ 20, 20 });
        m_terrainMesh->setHeight(2.25f);
        m_terrainMesh->setHeightMap(renderSystem->getTextureManager()->getTexture2D("Heightmap02.png"));
        MeshRendererPtr terrainMeshRenderer = terrainEntity->ensureComponent<MeshRenderer>();
        terrainMeshRenderer->setMaterial("Terrain00.mzmaterial");


        // Water
        EntityPtr waterEntity = createEntity("Water");
        m_waterRenderer = waterEntity->ensureComponent<WaterRenderer3D>();
        Transform3DPtr waterTransform = waterEntity->ensureComponent<Transform3D>();
        waterTransform->setLocalScale({ 20.0f, 20.0f, 1.0f });
        waterTransform->setLocalY(0.75f);
        waterTransform->setLocalRotation(Quaternion(Math::DegreesToRadians(90), Vec3F32::c_unitX));
        MeshRendererPtr waterMeshRenderer = waterEntity->ensureComponent<MeshRenderer>();
        waterMeshRenderer->setRenderMesh(renderSystem->getRenderMeshManager()->getDefaultQuadMesh());
        m_waterRenderer->setMaterial("Water00.mzmaterial");
        // m_waterRenderer->getMeshRenderer()->getMaterial()->getFirstRenderPass()->setRenderQueueIndex(2500);
        
        ConsoleService::GetInstancePtr()->registerCommand(
            MAZE_HS("water"),
            [this](String* _argv, S32 _argc)
            {
                if (_argc > 1)
                    return false;

                if (_argc == 0)
                    SettingsManager::GetInstancePtr()->getSettingsRaw<Demo00Settings>()->setWaterEnabled(
                        !SettingsManager::GetInstancePtr()->getSettingsRaw<Demo00Settings>()->getWaterEnabled());
                else
                    SettingsManager::GetInstancePtr()->getSettingsRaw<Demo00Settings>()->setWaterEnabled(StringHelper::StringToBool(_argv[0]));

                return true;
            }, 1);
        updateWater();


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


        // Debug menu
        {
            Transform2DPtr debugMenuTransform = SpriteHelper::CreateTransform2D(
                Vec2F(200.0f, 358.0f),
                Vec2F(-20.0f, -20.0f),
                m_canvas->getTransform(),
                this,
                { 1.0f, 1.0f },
                { 1.0f, 1.0f });            
            debugMenuTransform->setZ(2000);
            m_debugMenuCanvasGroup = debugMenuTransform->getEntityRaw()->ensureComponent<CanvasGroup>();

            m_debugMenuButton = UIHelper::CreateToggleButton(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
                Vec2F(24.0f, 24.0f),
                Vec2F(-5.0f, -5.0f),
                debugMenuTransform,
                this,
                Vec2F(1.0f, 1.0f),
                Vec2F(1.0f, 1.0f),
                ColorU32{ 200, 200, 200, 160 },
                ColorU32{ 187, 187, 187, 160 },
                ColorU32{ 161, 161, 161, 160 },
                ColorU32{ 171, 171, 171, 160 },
                ColorU32{ 151, 151, 151, 160 });
            m_debugMenuButton->setCheckByClick(false);
            m_debugMenuButton->eventClick.subscribe(
                [](Button2D* _button, CursorInputEvent const& _event)
                {
                    if (_event.button != 0)
                        return;

                    SettingsManager::GetInstancePtr()->getSettingsRaw<Demo00Settings>()->setDebugMenu(
                        !SettingsManager::GetInstancePtr()->getSettingsRaw<Demo00Settings>()->getDebugMenu());
                    SettingsManager::GetInstancePtr()->saveSettings();
                });
            SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Hamburger),
                Vec2F32(24.0f, 24.0f),
                Vec2F32::c_zero,
                MaterialManager::GetCurrentInstance()->getSpriteMaterial(),
                m_debugMenuButton->getTransform(),
                this)->setColor(85, 85, 85);

            SystemTextRenderer2DPtr textRenderer = SystemUIHelper::CreateSystemText(
                "Menu",
                8u,
                HorizontalAlignment2D::Center,
                VerticalAlignment2D::Middle,
                Vec2F::c_zero,
                Vec2F(95.0f, -5.0f - 12.0f),
                debugMenuTransform,
                this,
                Vec2F(0.0f, 1.0f),
                Vec2F(0.5f, 0.5f));
            textRenderer->setColor(0, 0, 0, 160);


            m_debugMenuBackground = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
                debugMenuTransform->getSize(),
                Vec2F::c_zero,
                GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getMaterialManager()->getSpriteMaterial(),
                debugMenuTransform,
                this,
                { 1.0f, 1.0f },
                { 1.0f, 1.0f });
            m_debugMenuBackground->setColor(255, 255, 255, 140);
            m_debugMenuBackground->getTransform()->setZ(-100);

            m_debugMenuBackgroundCollapsed = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
                Vec2F(debugMenuTransform->getWidth(), 34.0f),
                Vec2F::c_zero,
                GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getMaterialManager()->getSpriteMaterial(),
                debugMenuTransform,
                this,
                { 1.0f, 1.0f },
                { 1.0f, 1.0f });
            m_debugMenuBackgroundCollapsed->setColor(255, 255, 255, 140);
            m_debugMenuBackgroundCollapsed->getTransform()->setZ(-100);


            SpriteRenderer2DPtr debugMenuInnerBackground = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
                m_debugMenuBackground->getTransform()->getSize() - Vec2F(10.0f, 39.0f),
                Vec2F(0.5f, 5.0f),
                GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getMaterialManager()->getSpriteMaterial(),
                m_debugMenuBackground->getTransform(),
                this,
                { 0.5f, 0.0f },
                { 0.5f, 0.0f });
            debugMenuInnerBackground->setColor(55, 55, 55, 100);

            VerticalLayout2DPtr listLayout = UIHelper::CreateVerticalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                debugMenuInnerBackground->getTransform()->getSize(),
                Vec2F::c_zero,
                debugMenuInnerBackground->getTransform(),
                this);
            listLayout->setPadding(5.0f, 5.0f, 5.0f, 5.0f);
            listLayout->setSpacing(2.0f);
            listLayout->setAutoHeight(false);

            // Bloom
            m_debugMenuBloomButton = createDebugMenuToggleButton("Bloom", listLayout->getTransform());
            m_debugMenuBloomButton->eventClick.subscribe(
                [](Button2D* _button, CursorInputEvent const& _event)
            {
                if (_event.button != 0)
                    return;

                SettingsManager::GetInstancePtr()->getSettingsRaw<ExampleCommonSettings>()->setBloomEnabled(
                    !SettingsManager::GetInstancePtr()->getSettingsRaw<ExampleCommonSettings>()->getBloomEnabled());
                SettingsManager::GetInstancePtr()->saveSettings();
            });

            // Particles
            m_debugMenuParticlesButton = createDebugMenuToggleButton("Particles", listLayout->getTransform());
            m_debugMenuParticlesButton->eventClick.subscribe(
                [](Button2D* _button, CursorInputEvent const& _event)
                {
                    if (_event.button != 0)
                        return;

                    SettingsManager::GetInstancePtr()->getSettingsRaw<Demo00Settings>()->setParticlesEnabled(
                        !SettingsManager::GetInstancePtr()->getSettingsRaw<Demo00Settings>()->getParticlesEnabled());
                    SettingsManager::GetInstancePtr()->saveSettings();
                });

            // Water
            m_debugMenuWaterButton = createDebugMenuToggleButton("Water", listLayout->getTransform());
            m_debugMenuWaterButton->eventClick.subscribe(
                [](Button2D* _button, CursorInputEvent const& _event)
                {
                    if (_event.button != 0)
                        return;

                    SettingsManager::GetInstancePtr()->getSettingsRaw<Demo00Settings>()->setWaterEnabled(
                        !SettingsManager::GetInstancePtr()->getSettingsRaw<Demo00Settings>()->getWaterEnabled());
                    SettingsManager::GetInstancePtr()->saveSettings();
                });

            // Water render mode
            {
                SystemTextRenderer2DPtr text = SystemUIHelper::CreateSystemText(
                    "Water Render Mode",
                    8u,
                    HorizontalAlignment2D::Center,
                    VerticalAlignment2D::Middle,
                    Vec2F(180.0f, 24.0f),
                    Vec2F::c_zero,
                    listLayout->getTransform(),
                    this,
                    Vec2F(0.5f, 0.5f),
                    Vec2F(0.5f, 0.5f));
                text->setColor(30, 30, 30, 255);
            }

            for (ExampleWaterRenderMode mode = ExampleWaterRenderMode::Default; mode < ExampleWaterRenderMode::MAX; ++mode)
            {
                m_debugMenuExampleWaterRenderModeButtons[(Size)mode] = createDebugMenuToggleButton(mode.toCString(), listLayout->getTransform());
                m_debugMenuExampleWaterRenderModeButtons[(Size)mode]->eventClick.subscribe(
                    [mode](Button2D* _button, CursorInputEvent const& _event)
                    {
                        if (_event.button != 0)
                            return;

                        SettingsManager::GetInstancePtr()->getSettingsRaw<Demo00Settings>()->setExampleWaterRenderMode(mode);
                        SettingsManager::GetInstancePtr()->saveSettings();
                    });
            }

            updateDebugMenu();
        }


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
        Vec2U32 size = Example::GetInstancePtr()->getMainRenderWindowAbsoluteSize();
        Debug::Log("Render target resized: %ux%u", size.x, size.y);

        if (!Example::GetInstancePtr()->isMainWindowReadyToRender())
            return;

        m_renderBuffer->setSize(size);
    }

    //////////////////////////////////////////
    void SceneExample::update(F32 _dt)
    {
        EcsRenderScene::update(_dt);

        ExampleCommonSettings* exampleCommonSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ExampleCommonSettings>();
        Demo00Settings* exampleSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<Demo00Settings>();

        if (exampleCommonSettings->getBloomEnabled())
        {
            m_bloomController->update(_dt);
            m_renderColorSprite->getMaterial()->ensureUniform(
                MAZE_HS("u_bloomMap"),
                ShaderUniformType::UniformTexture2D)->set(
                    m_bloomController->getBloomRenderBuffer()->getColorTexture2D());
        }


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
            if (m_renderWaterController && m_renderWaterController->getReflectionBuffer() && m_renderWaterController->getRefractionBuffer())
            {
                Texture2DPtr reflectionTexture = m_renderWaterController->getReflectionBuffer()->getColorTexture2D();
                Texture2DPtr refractionTexture = m_renderWaterController->getRefractionBuffer()->getColorTexture2D();

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
        ps->getTransform()->rotate(Vec3F32::c_unitX, -Math::c_halfPi);
        ps->getTransform()->setLocalY(2.0f);

        ps->getMainModule().setPrewarm(true);
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
        ps->getRendererModule().getTextureSheetAnimation().tiles = Vec2S32(7, 7);
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
            gradient.addKeyRGB(0.0f, Vec3F32::c_one);
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


        ConsoleService::GetInstancePtr()->registerCommand(
            MAZE_HS("ps"),
            [this](String* _argv, S32 _argc)
            {
                if (_argc > 1)
                    return false;

                m_particleSystem->restart();
                if (_argc == 0)
                    SettingsManager::GetInstancePtr()->getSettingsRaw<Demo00Settings>()->setParticlesEnabled(
                        !SettingsManager::GetInstancePtr()->getSettingsRaw<Demo00Settings>()->getParticlesEnabled());
                else
                    SettingsManager::GetInstancePtr()->getSettingsRaw<Demo00Settings>()->setParticlesEnabled(StringHelper::StringToBool(_argv[0]));

                return true;
            }, 1);

        updateParticleSystem();
    }

    //////////////////////////////////////////
    void SceneExample::notifyMouse(InputEventMouseData const& _data)
    {
        switch (_data.type)
        {
            case InputEventMouseType::Move:
            {
                Vec2F32 cursorPosition = Vec2F32((F32)_data.x, (F32)_data.y);

                if (m_cursorDrag)
                {
                    Vec2F32 deltaPosition = cursorPosition - m_cursorPositionLastFrame;

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
                    Vec2F32 cursorPosition = Vec2F32((F32)_data.x, (F32)_data.y);
                    Rect2DF viewportRect(
                        m_canvas->getViewport().position.x * m_renderTarget->getRenderTargetSize().x,
                        m_canvas->getViewport().position.y * m_renderTarget->getRenderTargetSize().y,
                        m_canvas->getViewport().size.x * m_renderTarget->getRenderTargetSize().x,
                        m_canvas->getViewport().size.y * m_renderTarget->getRenderTargetSize().y);

                    m_cursorPositionLastFrame = cursorPosition;

                    if (viewportRect.contains(cursorPosition) &&
                        (!m_debugMenuBackground->getEntityRaw()->getActiveSelf() || !m_debugMenuBackground->getTransform()->calculateWorldAABB().toRect().contains(
                            m_canvas->convertRenderTargetCoordsToViewportCoords(cursorPosition))))
                    {
                        m_cursorDrag = true;
                        m_cursorPositionLastFrame = cursorPosition;
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
                Vec2F32 cursorPosition = Vec2F32((F32)_data.x, (F32)_data.y);

                if (m_cursorDrag)
                {
                    Vec2F32 deltaPosition = cursorPosition - m_cursorPositionLastFrame;

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
                    Vec2F32 cursorPosition = Vec2F32((F32)_data.x, (F32)_data.y);
                    Rect2DF viewportRect(
                        m_canvas->getViewport().position.x * m_renderTarget->getRenderTargetSize().x,
                        m_canvas->getViewport().position.y * m_renderTarget->getRenderTargetSize().y,
                        m_canvas->getViewport().size.x * m_renderTarget->getRenderTargetSize().x,
                        m_canvas->getViewport().size.y * m_renderTarget->getRenderTargetSize().y);

                    m_cursorPositionLastFrame = cursorPosition;

                    if (viewportRect.contains(cursorPosition))
                    {
                        m_cursorDrag = true;
                        m_cursorPositionLastFrame = cursorPosition;
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

    //////////////////////////////////////////
    void SceneExample::notifyDemo00SettingsChanged(bool const& _value)
    {
        updateDebugMenu();
        updateParticleSystem();
        updateWater();
        updateBloom();
    }

    //////////////////////////////////////////
    void SceneExample::notifyDemo00SettingsChanged(ExampleWaterRenderMode const& _value)
    {
        updateDebugMenu();
        updateWater();
    }

    //////////////////////////////////////////
    void SceneExample::updateDebugMenu()
    {
        ExampleCommonSettings* exampleCommonSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ExampleCommonSettings>();
        Demo00Settings* exampleSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<Demo00Settings>();

        bool debugMenuActive = exampleSettings->getDebugMenu();
        
        m_debugMenuCanvasGroup->setLocalAlpha(debugMenuActive ? 1.0f : 0.75f);
        m_debugMenuButton->setChecked(debugMenuActive);
        m_debugMenuBackground->getEntityRaw()->setActiveSelf(debugMenuActive);
        m_debugMenuBackgroundCollapsed->getEntityRaw()->setActiveSelf(!debugMenuActive);

        m_debugMenuBloomButton->setChecked(exampleCommonSettings->getBloomEnabled());
        m_debugMenuParticlesButton->setChecked(exampleSettings->getParticlesEnabled());
        m_debugMenuWaterButton->setChecked(exampleSettings->getWaterEnabled());


        for (ExampleWaterRenderMode mode = ExampleWaterRenderMode::Default; mode < ExampleWaterRenderMode::MAX; ++mode)
            m_debugMenuExampleWaterRenderModeButtons[(Size)mode]->setChecked(exampleSettings->getExampleWaterRenderMode() == mode);
    }

    //////////////////////////////////////////
    ToggleButton2DPtr SceneExample::createDebugMenuToggleButton(
        CString _title,
        Transform2DPtr const& _parent)
    {
        ToggleButton2DPtr button = UIHelper::CreateToggleButton(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
            Vec2F(180.0f, 24.0f),
            Vec2F::c_zero,
            _parent,
            this,
            Vec2F(1.0f, 1.0f),
            Vec2F(1.0f, 1.0f),
            ColorU32{ 200, 200, 200, 255 },
            ColorU32{ 255, 200, 50, 255 },
            ColorU32{ 255, 160, 20, 255 },
            ColorU32{ 255, 160, 20, 255 },
            ColorU32{ 255, 200, 50, 255 });
        button->setCheckByClick(false);

        SystemTextRenderer2DPtr textRenderer = SystemUIHelper::CreateSystemText(
            _title,
            8u,
            HorizontalAlignment2D::Center,
            VerticalAlignment2D::Middle,
            Vec2F::c_zero,
            Vec2F::c_zero,
            button->getTransform(),
            this,
            Vec2F(0.5f, 0.5f),
            Vec2F(0.5f, 0.5f));
        textRenderer->setColor(30, 30, 30, 255);

        return button;
    }

    //////////////////////////////////////////
    void SceneExample::updateParticleSystem()
    {
        Demo00Settings* exampleSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<Demo00Settings>();
        if (exampleSettings->getParticlesEnabled() != m_particleSystem->getEntityRaw()->getActiveSelf())
        {
            m_particleSystem->restart();
            m_particleSystem->getEntityRaw()->setActiveSelf(exampleSettings->getParticlesEnabled());
        }
    }

    //////////////////////////////////////////
    void SceneExample::updateWater()
    {
        Demo00Settings* exampleSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<Demo00Settings>();

        m_renderWaterController->setEnabled(exampleSettings->getWaterEnabled());

        m_waterRenderer->getEntityRaw()->setActiveSelf(exampleSettings->getWaterEnabled());

        MaterialPtr const& waterMaterial = MaterialManager::GetCurrentInstance()->getMaterial("Water00.mzmaterial");
        if (exampleSettings->getExampleWaterRenderMode() == ExampleWaterRenderMode::Default)
        {
            m_waterRenderer->setMaterial(waterMaterial);
        }
        else
        {
            MaterialPtr newMaterial = waterMaterial->createCopy();

            ShaderPtr shader = newMaterial->getFirstRenderPass()->getShader()->createCopy();
            shader->addLocalFeature("DEBUG_MATERIAL", "(1)");
            shader->recompile();

            S32 debugMaterial = (S32)exampleSettings->getExampleWaterRenderMode() - 2;
            Debug::log << "u_debugMaterial=" << debugMaterial << endl;
            newMaterial->getFirstRenderPass()->setShader(shader);
            newMaterial->setUniform(MAZE_HASHED_CSTRING("u_debugMaterial"), debugMaterial);
            
            m_waterRenderer->setMaterial(newMaterial);
        }
    }

    //////////////////////////////////////////
    void SceneExample::updateBloom()
    {
        ExampleCommonSettings* exampleCommonSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ExampleCommonSettings>();

        if (exampleCommonSettings->getBloomEnabled())
        {
            m_renderColorSprite->getEntityRaw()->setActiveSelf(true);
            m_camera3D->setRenderTarget(m_renderBuffer);

            m_renderColorSprite->getMaterial()->ensureUniform(
                MAZE_HS("u_bloomMap"),
                ShaderUniformType::UniformTexture2D)->set(
                    m_renderBuffer->getColorTexture2D());
        }
        else
        {
            m_renderColorSprite->getEntityRaw()->setActiveSelf(false);
            m_camera3D->setRenderTarget(Example::GetInstancePtr()->getMainRenderWindow());
        }
    }

} // namespace Maze
//////////////////////////////////////////
