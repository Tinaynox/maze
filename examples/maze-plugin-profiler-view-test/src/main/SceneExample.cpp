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
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/components/MazeRotor3D.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
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
#include "maze-core/events/MazeEvent.hpp"
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
#include "maze-plugin-console/MazeConsolePlugin.hpp"
#include "maze-plugin-console/MazeConsoleService.hpp"
#include "Example.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    String GetExampleName()
    {
        return "Test00";
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
        MAZE_LOAD_PLATFORM_PLUGIN(Console);

        if (SettingsManager::GetInstancePtr()->getSettingsRaw<ProfilerViewSettings>())
            SettingsManager::GetInstancePtr()->getSettingsRaw<ProfilerViewSettings>()->setActive(true);

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


        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.subscribe(this, &SceneExample::notifyMainRenderWindowViewportChanged);
        Example::GetInstancePtr()->getMainRenderWindow()->eventRenderTargetResized.subscribe(this, &SceneExample::notifyRenderTargetResized);


        // Light
        EntityPtr lightEntity = createEntity("Light");
        Light3DPtr mainLight3D = lightEntity->createComponent<Light3D>();
        mainLight3D->setColor(ColorU32(255, 244, 214));
        mainLight3D->getTransform()->setLocalDirection(-0.477f, -0.677f, -0.577f);
        mainLight3D->getTransform()->setLocalPosition(5.0f, 5.0f, 5.0f);


        // Camera
        EntityPtr cameraEntity = createEntity("Camera");
        m_camera3D = cameraEntity->createComponent<Camera3D>();
        m_camera3D->getTransform()->setLocalPosition(Vec3F32(8.0f, 4.0f, 8.0f));
        // m_camera3D->setFOV(Math::DegreesToRadians(30));
        m_camera3D->setClearColorFlag(true);
        // m_camera3D->setClearSkyBoxFlag(true);
        m_camera3D->setRenderMask(m_camera3D->getRenderMask() | (S32)DefaultRenderMask::Gizmos);
        m_camera3D->setNearZ(0.01f);
        m_camera3D->setFarZ(100.0f);
        m_camera3D->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
        m_camera3D->setRenderTarget(Example::GetInstancePtr()->getMainRenderWindow());


        // m_world->addSystem("SomeUpdateES", SomeUpdateES, 0);

        EntityPtr objectEntity = createEntity();
        Transform3DPtr transform = objectEntity->createComponent<Transform3D>();
        MeshRendererPtr meshRenderer = objectEntity->createComponent<MeshRenderer>();
        meshRenderer->setRenderMesh(RenderMeshManager::GetCurrentInstancePtr()->getDefaultCubeMesh());
        // meshRenderer->setRenderMesh("DroneLP.obj");
        meshRenderer->setMaterial(MaterialManager::GetCurrentInstance()->getBuiltinMaterial(BuiltinMaterialType::SpecularDS));
        objectEntity->ensureComponent<Name>("Obj");
        Rotor3DPtr rotor = objectEntity->createComponent<Rotor3D>();
        rotor->setAxis({0.0f, -0.7071f, -0.7071f });
        rotor->setSpeed(0.2f);


        ConsoleService::GetInstancePtr()->registerCommand(
            "profile",
            [](String* _argv, S32 _argc)
            {
                if (_argc == 0)
                    SettingsManager::GetInstancePtr()->getSettingsRaw<ProfilerViewSettings>()->switchActive();
                else
                {
                    bool active = StringHelper::StringToBool(_argv[0]);
                    SettingsManager::GetInstancePtr()->getSettingsRaw<ProfilerViewSettings>()->setActive(active);
                }

                return true;
            });

        ConsoleService::GetInstancePtr()->registerCommand(
            "memory.dump",
            [](String* _argv, S32 _argc)
            {
                Debug::log << MemoryTrackerService::DumpCurrentAllocations() << endl;

                return true;
            });

        ConsoleService::GetInstancePtr()->registerCommand(
            "memory.log",
            [](String* _argv, S32 _argc)
            {
                MemoryTrackerService::LogCurrentAllocationsByType();

                return true;
            });

        return true;
    }

    //////////////////////////////////////////
    void SceneExample::notifyMainRenderWindowViewportChanged(Rect2DF const& _mainRenderWindowViewport)
    {
        if (!Example::GetInstancePtr()->isMainWindowReadyToRender())
            return;

        m_canvas->setViewport(_mainRenderWindowViewport);
        m_camera3D->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
    }

    //////////////////////////////////////////
    void SceneExample::notifyRenderTargetResized(RenderTarget* _renderTarget)
    {
        if (!Example::GetInstancePtr()->isMainWindowReadyToRender())
            return;

    }

    //////////////////////////////////////////
    void SceneExample::update(F32 _dt)
    {
        ECSRenderScene::update(_dt);

        Quaternion q = Quaternion::Slerp(
            24.0f * _dt,
            m_camera3D->getTransform()->getLocalRotation(),
            Quaternion(m_pitchAngle, m_yawAngle, 0.0f));
        m_camera3D->getTransform()->setLocalRotation(q);
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
