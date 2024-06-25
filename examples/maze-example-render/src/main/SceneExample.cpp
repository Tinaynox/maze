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
        ExampleCommonSettings* exampleCommonSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ExampleCommonSettings>();

        exampleCommonSettings->setBloomEnabled(false);

        if (!BaseSceneExample::init(Vec2F(20.0f, 20.0f)))
            return false;

        InputManager::GetInstancePtr()->eventKeyboard.subscribe(this, &SceneExample::notifyKeyboard);

        getLightingSettings()->setSkyBoxMaterial("Skybox02.mzmaterial");
        m_simpleLevelConfig.floorMaterial = MaterialManager::GetCurrentInstance()->getMaterial("Chessboard00.mzmaterial");
        m_simpleLevelConfig.wallMaterial = MaterialManager::GetCurrentInstance()->getMaterial("Chessboard00.mzmaterial");
        ExampleHelper::BuildSimpleLevel(
            this,
            m_fpsController->getLevelSize(),
            m_simpleLevelConfig);

        createMeshForCamera(m_camera3D, "PlayerCamera00.mzmaterial");

        
        m_fpsController->setPosition(Vec3F(-8.031569f, 0.000000f, 8.586735f));
        m_fpsController->setYawAngle(Math::DegreesToRadians(120.0f));

        // Red Camera
        {
            EntityPtr cameraEntity = createEntity();
            Transform3DPtr cameraTransform = cameraEntity->ensureComponent<Transform3D>();

            m_cameraRed = cameraEntity->createComponent<Camera3D>();
            m_cameraRed->setClearColorFlag(false);
            m_cameraRed->setClearSkyBoxFlag(true);
            m_cameraRed->setRenderMask(m_camera3D->getRenderMask() & ~(S32)DefaultRenderMask::Gizmos);
            m_cameraRed->getEntityRaw()->ensureComponent<Name>("Red Camera");
            m_cameraRed->setNearZ(0.01f);
            m_cameraRed->setFarZ(666.0f);
            m_cameraRed->setRenderTarget(Example::GetInstancePtr()->getMainRenderWindow());

            m_cameraRed->setViewport(getRedCameraViewport());

            createMeshForCamera(m_cameraRed, "RedCamera00.mzmaterial");

            cameraTransform->setLocalPosition(-4.000000f, 3.672040f, 0.000000f);
            cameraTransform->setLocalRotation(0.702228f, 0.219007f, 0.646709f, -0.201692f);

            SinMovement3DPtr cameraSinMovement = cameraEntity->ensureComponent<SinMovement3D>();
            cameraSinMovement->setAxis(Vec3F::c_unitX);
            cameraSinMovement->setAmplitude(5.0f);
            cameraSinMovement->setFrequency(0.6f);
        }

        // Green Camera
        {
            EntityPtr cameraEntity = createEntity();
            Transform3DPtr cameraTransform = cameraEntity->ensureComponent<Transform3D>();

            m_cameraGreen = cameraEntity->createComponent<Camera3D>();
            m_cameraGreen->setClearColorFlag(false);
            m_cameraGreen->setClearSkyBoxFlag(true);
            m_cameraGreen->setRenderMask(m_camera3D->getRenderMask() & ~(S32)DefaultRenderMask::Gizmos);
            m_cameraGreen->getEntityRaw()->ensureComponent<Name>("Green Camera");
            m_cameraGreen->setNearZ(0.01f);
            m_cameraGreen->setFarZ(666.0f);
            m_cameraGreen->setRenderTarget(Example::GetInstancePtr()->getMainRenderWindow());

            m_cameraGreen->setViewport(getGreenCameraViewport());

            createMeshForCamera(m_cameraGreen, "GreenCamera00.mzmaterial");

            m_cameraGreen->setFOV(Math::DegreesToRadians(100.0f));
            cameraTransform->setLocalPosition(12.444103f, 5.068819f, -4.097405f);
            cameraTransform->setLocalRotation(0.761028f, 0.117517f, -0.627810f, 0.113489f);
        }


        return true;
    }

    //////////////////////////////////////////
    void SceneExample::update(F32 _dt)
    {
        BaseSceneExample::update(_dt);
    }

    //////////////////////////////////////////
    void SceneExample::updateHintText()
    {
        ExampleCommonSettings* exampleCommonSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ExampleCommonSettings>();        

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

        if (m_cameraRed)
            m_cameraRed->setViewport(getRedCameraViewport());

        if (m_cameraGreen)
            m_cameraGreen->setViewport(getGreenCameraViewport());
    }

    //////////////////////////////////////////
    Rect2DF SceneExample::getMainViewport()
    {
        Rect2DF viewport = Example::GetInstancePtr()->getMainRenderWindowViewport();
        viewport.size.x *= 0.65f;
        return viewport;
    }

    //////////////////////////////////////////
    Rect2DF SceneExample::getRedCameraViewport()
    {
        Rect2DF viewport = Example::GetInstancePtr()->getMainRenderWindowViewport();
        viewport.position.x += viewport.size.x * 0.65f;
        viewport.size.x *= 0.35f;
        viewport.size.y *= 0.5f;
        return viewport;
    }

    //////////////////////////////////////////
    Rect2DF SceneExample::getGreenCameraViewport()
    {
        Rect2DF viewport = Example::GetInstancePtr()->getMainRenderWindowViewport();
        viewport.position.x += viewport.size.x * 0.65f;
        viewport.position.y += viewport.size.y * 0.5f;
        viewport.size.x *= 0.35f;
        viewport.size.y *= 0.5f;
        return viewport;
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

} // namespace Maze
//////////////////////////////////////////
