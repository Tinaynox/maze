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
#include "maze-plugin-profiler-view/MazeProfilerViewPlugin.hpp"
#include "maze-plugin-loader-png/MazeLoaderPNGPlugin.hpp"
#include "maze-plugin-loader-dds/MazeLoaderDDSPlugin.hpp"
#include "maze-plugin-loader-tga/MazeLoaderTGAPlugin.hpp"
#include "maze-plugin-loader-jpg/MazeLoaderJPGPlugin.hpp"
#include "maze-plugin-loader-fbx/MazeLoaderFBXPlugin.hpp"
#include "Example.hpp"
#include "ExampleFPSCameraController.hpp"
#include "ExampleCommonSettings.hpp"
#include "LevelBloomController.hpp"


//////////////////////////////////////////
#define DISTORTION_BUFFER_DIV 1u


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    String GetExampleName()
    {
        return "Cameras, Viewports and Render Targets";
    }

    //////////////////////////////////////////
    void LoadFirstExampleScene(SceneManager* _sceneManager)
    {
        Example::GetInstancePtr()->loadCoreGameAssets();
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
        MAZE_LOAD_PLATFORM_PLUGIN(ProfilerView);
        MAZE_LOAD_PLATFORM_PLUGIN(LoaderPNG);
        // MAZE_LOAD_PLATFORM_PLUGIN(LoaderDDS);
        MAZE_LOAD_PLATFORM_PLUGIN(LoaderTGA);
        MAZE_LOAD_PLATFORM_PLUGIN(LoaderJPG);
        MAZE_LOAD_PLATFORM_PLUGIN(LoaderFBX);

        return true;
    }


    //////////////////////////////////////////
    // Class SceneExample
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneExample, BaseSceneExample);

    //////////////////////////////////////////
    SceneExample::SceneExample()
    {

    }

    //////////////////////////////////////////
    SceneExample::~SceneExample()
    {
        if (InputManager::GetInstancePtr())
            InputManager::GetInstancePtr()->eventKeyboard.unsubscribe(this);

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
        Example::GetInstancePtr()->getMainRenderWindow()->setVSync(true);

        ExampleCommonSettings* exampleCommonSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ExampleCommonSettings>();

        exampleCommonSettings->setBloomEnabled(false);

        if (!BaseSceneExample::init(Vec2F(20.0f, 20.0f)))
            return false;

        m_canvas->getEntityRaw()->getComponent<CanvasScaler>()->setScaleMode(CanvasScalerScaleMode::ConstantPixelSize);

        SpriteRenderer2DPtr mainBorder = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Frame01),
            m_canvas->getTransform()->getSize(),
            Vec2F::c_zero,
            SpriteManager::GetCurrentInstance()->getDefaultSpriteMaterial(),
            m_canvas->getTransform(),
            this);
        mainBorder->getEntityRaw()->ensureComponent<SizePolicy2D>();

        m_mainLight3D->getTransform()->setLocalRotation(0.401372f, 0.861992f, 0.103903f, 0.291678f);

        InputManager::GetInstancePtr()->eventKeyboard.subscribe(this, &SceneExample::notifyKeyboard);

        getLightingSettings()->setSkyBoxMaterial("Skybox02.mzmaterial");
        m_simpleLevelConfig.floorMaterial = MaterialManager::GetCurrentInstance()->getOrLoadMaterial("Chessboard00.mzmaterial");
        m_simpleLevelConfig.wallMaterial = MaterialManager::GetCurrentInstance()->getOrLoadMaterial("Chessboard00.mzmaterial");
        ExampleHelper::BuildSimpleLevel(
            this,
            m_fpsController->getLevelSize(),
            m_simpleLevelConfig);

        createMeshForCamera(m_camera3D, "PlayerCamera00.mzmaterial");

        
        m_fpsController->setPosition(Vec3F(-8.031569f, 0.000000f, 8.586735f));
        m_fpsController->setYawAngle(Math::DegreesToRadians(120.0f));

        
        // Red Camera
        {
            m_renderBufferRed = RenderBuffer::Create(
                {
                    getRedRenderBufferSize(),
                    PixelFormat::RGBA_U8,
                    PixelFormat::DEPTH_U24
                });
            m_renderBufferRed->setName("Red RenderBuffer");
            m_renderBufferRed->getColorTexture2D()->setMinFilter(TextureFilter::Linear);
            m_renderBufferRed->getColorTexture2D()->setMagFilter(TextureFilter::Linear);

            m_renderBufferRedTV = RenderBuffer::Create(
                {
                    getRedRenderBufferSize(),
                    PixelFormat::RGBA_U8,
                    //PixelFormat::DEPTH_U24
                });
            m_renderBufferRedTV->setName("Red RenderBuffer Copy");
            m_renderBufferRedTV->getColorTexture2D()->setMinFilter(TextureFilter::Linear);
            m_renderBufferRedTV->getColorTexture2D()->setMagFilter(TextureFilter::Linear);

            EntityPtr cameraEntity = createEntity();
            Transform3DPtr cameraTransform = cameraEntity->ensureComponent<Transform3D>();

            m_cameraRed = cameraEntity->createComponent<Camera3D>();
            m_cameraRed->setClearColorFlag(false);
            m_cameraRed->setClearSkyBoxFlag(true);
            m_cameraRed->setRenderMask(m_camera3D->getRenderMask() & ~(S32)DefaultRenderMask::Gizmos);
            m_cameraRed->getEntityRaw()->ensureComponent<Name>("Red Camera");
            m_cameraRed->setNearZ(0.01f);
            m_cameraRed->setFarZ(666.0f);
            m_cameraRed->setRenderTarget(m_renderBufferRed);

            //m_cameraRed->setViewport(getRedCameraViewport());

            createMeshForCamera(m_cameraRed, "RedCamera00.mzmaterial");

            cameraTransform->setLocalPosition(-4.000000f, 3.672040f, 0.000000f);
            cameraTransform->setLocalRotation(0.702228f, 0.219007f, 0.646709f, -0.201692f);

            SinMovement3DPtr cameraSinMovement = cameraEntity->ensureComponent<SinMovement3D>();
            cameraSinMovement->setAxis(Vec3F::c_unitX);
            cameraSinMovement->setAmplitude(5.0f);
            cameraSinMovement->setFrequency(0.6f);


            // UI
            EntityPtr canvasEntity = createEntity("Red Canvas");
            m_canvasRed = canvasEntity->createComponent<Canvas>();
            m_canvasRed->setViewport(getRedCameraViewport());
            m_canvasRed->setRenderTarget(Example::GetInstancePtr()->getMainRenderWindow());

            /*
            CanvasScalerPtr canvasScaler = canvasEntity->ensureComponent<CanvasScaler>();
            canvasScaler->setScaleMode(CanvasScalerScaleMode::ScaleWithViewportSize);
            canvasScaler->setScreenMatchMode(CanvasScalerScreenMatchMode::MatchWidthOrHeight);
            canvasScaler->setMatchWidthOrHeight(1.0f);
            canvasScaler->updateCanvasScale();
            */

            m_renderColorSpriteRed = SpriteHelper::CreateSprite(
                Sprite::Create(m_renderBufferRed->getColorTexture2D()),
                m_canvasRed->getTransform()->getSize(),
                Vec2F32::c_zero,
                SpriteManager::GetCurrentInstance()->getDefaultSpriteMaterial(),
                m_canvasRed->getTransform(),
                this);
            m_renderColorSpriteRed->getEntityRaw()->ensureComponent<SizePolicy2D>();

            SpriteRenderer2DPtr redBorder = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Frame01),
                m_canvasRed->getTransform()->getSize(),
                Vec2F::c_zero,
                SpriteManager::GetCurrentInstance()->getDefaultSpriteMaterial(),
                m_canvasRed->getTransform(),
                this);
            redBorder->getEntityRaw()->ensureComponent<SizePolicy2D>();
            redBorder->setColor(ColorU32::FromVec3F32(Vec3F(1.000000f, 0.329412f, 0.508235f)));

            SystemTextRenderer2DPtr hint = SystemUIHelper::CreateSystemText(
                "Red Camera",
                8,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                Vec2F32::c_zero,
                Vec2F32(10.0f, -10.0f),
                m_canvasRed->getTransform(),
                this,
                Vec2F32(0.0f, 1.0f),
                Vec2F32(0.0f, 1.0f));
            hint->setColor(redBorder->getColor());
            hint->setSystemFont(SystemFontManager::GetCurrentInstancePtr()->getBuiltinSystemFont(BuiltinSystemFontType::DefaultOutlined));
        }

        // Green Camera
        {
            m_renderBufferGreen = RenderBuffer::Create(
                {
                    getGreenRenderBufferSize(),
                    PixelFormat::RGBA_U8,
                    PixelFormat::DEPTH_U24
                });
            m_renderBufferGreen->setName("Green RenderBuffer");
            m_renderBufferGreen->getColorTexture2D()->setMinFilter(TextureFilter::Linear);
            m_renderBufferGreen->getColorTexture2D()->setMagFilter(TextureFilter::Linear);

            m_renderBufferGreenTV = RenderBuffer::Create(
                {
                    getGreenRenderBufferSize(),
                    PixelFormat::RGBA_U8,
                    //PixelFormat::DEPTH_U24
                });
            m_renderBufferGreenTV->setName("Green RenderBuffer");
            m_renderBufferGreenTV->getColorTexture2D()->setMinFilter(TextureFilter::Linear);
            m_renderBufferGreenTV->getColorTexture2D()->setMagFilter(TextureFilter::Linear);

            EntityPtr cameraEntity = createEntity();
            Transform3DPtr cameraTransform = cameraEntity->ensureComponent<Transform3D>();

            m_cameraGreen = cameraEntity->createComponent<Camera3D>();
            m_cameraGreen->setClearColorFlag(false);
            m_cameraGreen->setClearSkyBoxFlag(true);
            m_cameraGreen->setRenderMask(m_camera3D->getRenderMask() & ~(S32)DefaultRenderMask::Gizmos);
            m_cameraGreen->getEntityRaw()->ensureComponent<Name>("Green Camera");
            m_cameraGreen->setNearZ(0.01f);
            m_cameraGreen->setFarZ(666.0f);
            m_cameraGreen->setRenderTarget(m_renderBufferGreen);

            // m_cameraGreen->setViewport(getGreenCameraViewport());

            createMeshForCamera(m_cameraGreen, "GreenCamera00.mzmaterial");

            m_cameraGreen->setFOV(Math::DegreesToRadians(100.0f));
            cameraTransform->setLocalPosition(12.870625f, 5.244466f, -4.912718f);
            cameraTransform->setLocalRotation(0.860573f, 0.095453f, -0.482414f, 0.132584f);


            // UI
            EntityPtr canvasEntity = createEntity("Green Canvas");
            m_canvasGreen = canvasEntity->createComponent<Canvas>();
            m_canvasGreen->setViewport(getGreenCameraViewport());
            m_canvasGreen->setRenderTarget(Example::GetInstancePtr()->getMainRenderWindow());

            /*
            CanvasScalerPtr canvasScaler = canvasEntity->ensureComponent<CanvasScaler>();
            canvasScaler->setScaleMode(CanvasScalerScaleMode::ScaleWithViewportSize);
            canvasScaler->setScreenMatchMode(CanvasScalerScreenMatchMode::MatchWidthOrHeight);
            canvasScaler->setMatchWidthOrHeight(1.0f);
            canvasScaler->updateCanvasScale();
            */

            m_renderColorSpriteGreen = SpriteHelper::CreateSprite(
                Sprite::Create(m_renderBufferGreen->getColorTexture2D()),
                m_canvasGreen->getTransform()->getSize(),
                Vec2F32::c_zero,
                SpriteManager::GetCurrentInstance()->getDefaultSpriteMaterial(),
                m_canvasGreen->getTransform(),
                this);

            m_renderColorSpriteGreen->getEntityRaw()->ensureComponent<SizePolicy2D>();

            SpriteRenderer2DPtr greenBorder = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Frame01),
                m_canvasGreen->getTransform()->getSize(),
                Vec2F::c_zero,
                SpriteManager::GetCurrentInstance()->getDefaultSpriteMaterial(),
                m_canvasGreen->getTransform(),
                this);
            greenBorder->getEntityRaw()->ensureComponent<SizePolicy2D>();
            greenBorder->setColor(ColorU32::FromVec3F32(Vec3F(0.290196f, 0.933333f, 0.504575f)));

            SystemTextRenderer2DPtr hint = SystemUIHelper::CreateSystemText(
                "Green Camera",
                8,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                Vec2F32::c_zero,
                Vec2F32(10.0f, -10.0f),
                m_canvasGreen->getTransform(),
                this,
                Vec2F32(0.0f, 1.0f),
                Vec2F32(0.0f, 1.0f));
            hint->setColor(greenBorder->getColor());
            hint->setSystemFont(SystemFontManager::GetCurrentInstancePtr()->getBuiltinSystemFont(BuiltinSystemFontType::DefaultOutlined));
        }

        // Blue Camera
        {
            m_renderBufferBlue = RenderBuffer::Create(
                {
                    getBlueRenderBufferSize(),
                    PixelFormat::RGBA_U8,
                    PixelFormat::DEPTH_U24
                });
            m_renderBufferBlue->setName("Blue RenderBuffer");
            m_renderBufferBlue->getColorTexture2D()->setMinFilter(TextureFilter::Linear);
            m_renderBufferBlue->getColorTexture2D()->setMagFilter(TextureFilter::Linear);

            m_renderBufferBlueTV = RenderBuffer::Create(
                {
                    getGreenRenderBufferSize(),
                    PixelFormat::RGBA_U8,
                    //PixelFormat::DEPTH_U24
                });
            m_renderBufferBlueTV->setName("Blue RenderBuffer");
            m_renderBufferBlueTV->getColorTexture2D()->setMinFilter(TextureFilter::Linear);
            m_renderBufferBlueTV->getColorTexture2D()->setMagFilter(TextureFilter::Linear);

            EntityPtr cameraEntity = createEntity();
            Transform3DPtr cameraTransform = cameraEntity->ensureComponent<Transform3D>();

            m_cameraBlue = cameraEntity->createComponent<Camera3D>();
            m_cameraBlue->setClearColorFlag(false);
            m_cameraBlue->setClearSkyBoxFlag(true);
            m_cameraBlue->setRenderMask(m_camera3D->getRenderMask() & ~(S32)DefaultRenderMask::Gizmos);
            m_cameraBlue->getEntityRaw()->ensureComponent<Name>("Blue Camera");
            m_cameraBlue->setNearZ(0.01f);
            m_cameraBlue->setFarZ(666.0f);
            m_cameraBlue->setRenderTarget(m_renderBufferBlue);

            // m_cameraBlue->setViewport(getBlueCameraViewport());

            createMeshForCamera(m_cameraBlue, "BlueCamera00.mzmaterial");

            m_cameraBlue->setFOV(Math::DegreesToRadians(60.0f));
            cameraTransform->setLocalPosition(2.904454f, 1.586349f, 2.530885f);
            cameraTransform->setLocalRotation(0.659504f, -0.059111f, 0.746614f, 0.064239f);
        }


        EntityPtr tvRed = createTV(
            Vec3F(11.086767f, 2.399250f, -2.787267f),
            Vec4F(0.000000f, 0.707107f, 0.000000f, 0.707107f),
            Vec3F(2.000000f, 2.000000f, 1.000000f),
            m_renderBufferRedTV->getColorTexture2D(),
            "RedTV00.mzmaterial");
        SinMovement3DPtr tvRedSinMovement = tvRed->ensureComponent<SinMovement3D>();
        tvRedSinMovement->setAmplitude(0.75f);
        tvRedSinMovement->setFrequency(0.25f);

        EntityPtr tvGreen = createTV(
            Vec3F(11.086767f, 2.399250f, 6.787267f),
            Vec4F(0.000000f, 0.707107f, 0.000000f, 0.707107f),
            Vec3F(3.000000f, 3.000000f, 1.000000f),
            m_renderBufferGreenTV->getColorTexture2D(),
            "GreenTV00.mzmaterial");
        SinMovement3DPtr tvGreenSinMovement = tvGreen->ensureComponent<SinMovement3D>();
        tvGreenSinMovement->setAmplitude(0.5f);
        tvGreenSinMovement->setFrequency(0.325f);

        EntityPtr tvBlue = createTV(
            Vec3F(12.478155f, 3.611719f, 2.024486f),
            Vec4F(0.000000f, 0.707107f, 0.000000f, 0.707107f),
            Vec3F(6.000000f, 5.00000f, 1.000000f),
            m_renderBufferBlueTV->getColorTexture2D(),
            "BlueTV00.mzmaterial");
        SinMovement3DPtr tvBlueSinMovement = tvBlue->ensureComponent<SinMovement3D>();
        tvBlueSinMovement->setAmplitude(0.25f);
        tvBlueSinMovement->setFrequency(0.225f);

        return true;
    }

    //////////////////////////////////////////
    void SceneExample::update(F32 _dt)
    {
        BaseSceneExample::update(_dt);

        if (Example::GetInstancePtr()->isMainWindowReadyToRender())
        {
            m_renderBufferRedTV->blit(m_renderBufferRed);
            m_renderBufferGreenTV->blit(m_renderBufferGreen);
            m_renderBufferBlueTV->blit(m_renderBufferBlue);
        }
    }

    //////////////////////////////////////////
    void SceneExample::updateHintText()
    {
        ExampleCommonSettings* exampleCommonSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ExampleCommonSettings>();        

        /*
        m_hintText->setTextFormatted(
            "[CONTROLS]\n"
            "Movement - WASD, Jump - Space, Camera - RMB (Hold)\n"
            //"%s - R\n"
            "%s - P\n"
            "\n"
            "[INFO]\n"
            //"Mesh Movement: %s\n"
            "Post Processing: %s",
            //m_meshMovementEnabled ? "Disable Mesh Movement" : "Enable Mesh Movement",
            exampleCommonSettings->getBloomEnabled() ? "Disable Post Processing" : "Enable Post Processing",
            //m_meshMovementEnabled ? "ON" : "OFF",
            exampleCommonSettings->getBloomEnabled() ? "ON" : "OFF"
        );
        */

        m_hintText->setTextFormatted(
            "[CONTROLS]\n"
            "Movement - WASD, Jump - Space, Camera - RMB (Hold)");
    }


    //////////////////////////////////////////
    void SceneExample::notifyKeyboard(InputEventKeyboardData const& _data)
    {
        switch (_data.type)
        {
            case InputEventKeyboardType::KeyDown:
            {
                switch (_data.keyCode)
                {
                    case KeyCode::P:
                    {
                        /*
                        ExampleCommonSettings* exampleCommonSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ExampleCommonSettings>();

                        exampleCommonSettings->setBloomEnabled(
                            !exampleCommonSettings->getBloomEnabled());
                        */
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            default:
                break;
        }
    }

    //////////////////////////////////////////
    void SceneExample::updateRenderTargetViewport()
    {
        BaseSceneExample::updateRenderTargetViewport();

        //if (m_cameraRed)
        //    m_cameraRed->setViewport(getRedCameraViewport());

        //if (m_cameraGreen)
        //    m_cameraGreen->setViewport(getGreenCameraViewport());

        if (m_canvasRed)
            m_canvasRed->setViewport(getRedCameraViewport());

        if (m_canvasGreen)
            m_canvasGreen->setViewport(getGreenCameraViewport());
    }

    //////////////////////////////////////////
    Rect2F SceneExample::getMainViewport()
    {
        Rect2F viewport = Example::GetInstancePtr()->getMainRenderWindowViewport();
        viewport.size.x *= 0.65f;
        return viewport;
    }

    //////////////////////////////////////////
    Rect2F SceneExample::getRedCameraViewport()
    {
        Rect2F viewport = Example::GetInstancePtr()->getMainRenderWindowViewport();
        viewport.position.x += viewport.size.x * 0.65f;
        viewport.size.x *= 0.35f;
        viewport.size.y *= 0.5f;
        return viewport;
    }

    //////////////////////////////////////////
    Vec2U32 SceneExample::getRedRenderBufferSize()
    {
        return Vec2U32(Vec2F32(
            Example::GetInstancePtr()->getMainRenderWindow()->getRenderTargetSize()) * getRedCameraViewport().size * 2.0f);
    }

    //////////////////////////////////////////
    Rect2F SceneExample::getGreenCameraViewport()
    {
        Rect2F viewport = Example::GetInstancePtr()->getMainRenderWindowViewport();
        viewport.position.x += viewport.size.x * 0.65f;
        viewport.position.y += viewport.size.y * 0.5f;
        viewport.size.x *= 0.35f;
        viewport.size.y *= 0.5f;
        return viewport;
    }

    //////////////////////////////////////////
    Vec2U32 SceneExample::getGreenRenderBufferSize()
    {
        return Vec2U32(Vec2F32(
            Example::GetInstancePtr()->getMainRenderWindow()->getRenderTargetSize()) * getGreenCameraViewport().size * 2.0f);
    }

    //////////////////////////////////////////
    Vec2U32 SceneExample::getBlueRenderBufferSize()
    {
        F32 width = getMainViewport().size.x * Vec2F32(Example::GetInstancePtr()->getMainRenderWindow()->getRenderTargetSize()).x;
        F32 height = getMainViewport().size.y * Vec2F32(Example::GetInstancePtr()->getMainRenderWindow()->getRenderTargetSize()).y;
        F32 value = Math::Max(width, height);

        return Vec2U32(Vec2F(1.2f * value, value) * 2.0f);
    }

    //////////////////////////////////////////
    void SceneExample::updateRenderBuffersSize()
    {
        BaseSceneExample::updateRenderBuffersSize();

        if (m_renderBufferRed)
            m_renderBufferRed->setSize(getRedRenderBufferSize());

        if (m_renderBufferRedTV)
            m_renderBufferRedTV->setSize(getRedRenderBufferSize());

        if (m_renderBufferGreen)
            m_renderBufferGreen->setSize(getGreenRenderBufferSize());

        if (m_renderBufferGreenTV)
            m_renderBufferGreenTV->setSize(getGreenRenderBufferSize());

        if (m_renderBufferBlue)
            m_renderBufferBlue->setSize(getBlueRenderBufferSize());

        if (m_renderBufferBlueTV)
            m_renderBufferBlueTV->setSize(getBlueRenderBufferSize());
    }

    //////////////////////////////////////////
    void SceneExample::createMeshForCamera(
        Camera3DPtr const& _camera,
        String const& _material)
    {
        EntityPtr meshEntity = createEntity();

        meshEntity->ensureComponent<Name>("Mesh");

        Transform3DPtr meshTransform = meshEntity->createComponent<Transform3D>();
        meshTransform->setParent(_camera->getTransform());

        meshTransform->setLocalY(-0.3f);
        meshTransform->setLocalZ(-0.8f);
        meshTransform->setLocalRotationDegrees(0.0f, 90.0f, 0.0f);
        meshTransform->setLocalScale(10.0f);

        MeshRendererPtr meshRenderer = meshEntity->createComponent<MeshRenderer>();

        meshRenderer->setRenderMesh("Camera01.fbx");
        meshRenderer->setMaterial(_material);
    }

    //////////////////////////////////////////
    EntityPtr SceneExample::createTV(
        Vec3F const& _pos,
        Vec4F const& _rotation,
        Vec3F const& _scale,
        Texture2DPtr const& _texture,
        String const& _materialName)
    {
        EntityPtr quadEntity = createEntity();

        quadEntity->ensureComponent<Name>("TV");

        Transform3DPtr meshTransform = quadEntity->createComponent<Transform3D>();

        meshTransform->setLocalPosition(_pos);
        meshTransform->setLocalRotation(Quaternion(_rotation));
        meshTransform->setLocalScale(_scale);

        MeshRendererPtr meshRenderer = quadEntity->createComponent<MeshRenderer>();

        meshRenderer->setRenderMesh(
            RenderMeshManager::GetCurrentInstancePtr()->getDefaultQuadMesh());

        MaterialPtr material = MaterialManager::GetCurrentInstance()->getOrLoadMaterial(MAZE_HCS("TV00.mzmaterial"))->createCopy();
        RenderPassPtr const& renderPass = material->getFirstRenderPass();
        renderPass->setDepthWriteEnabled(true);
        renderPass->setDepthTestCompareFunction(CompareFunction::LessEqual);
        material->setUniform(MAZE_HCS("u_baseMap"), _texture);
        meshRenderer->setMaterial(material);


        {
            EntityPtr boxEntity = createEntity();
            boxEntity->ensureComponent<Name>("Cube");
            Transform3DPtr boxTransform = boxEntity->createComponent<Transform3D>();
            boxTransform->setParent(meshTransform);

            MeshRendererPtr boxMeshRenderer = boxEntity->createComponent<MeshRenderer>();

            boxMeshRenderer->setRenderMesh(
                RenderMeshManager::GetCurrentInstancePtr()->getDefaultCubeMesh());
            boxMeshRenderer->setMaterial(_materialName);

            boxTransform->setLocalPosition(-0.000010f, 0.000000f, 0.154284f);
            boxTransform->setLocalScale(1.200000f, 1.200000f, 0.200000f);
        }

        return quadEntity;
    }

} // namespace Maze
//////////////////////////////////////////
