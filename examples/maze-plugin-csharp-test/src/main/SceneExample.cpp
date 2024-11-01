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
#include "maze-plugin-csharp/MazeCSharpPlugin.hpp"
#include "maze-plugin-csharp/MazeCSharpService.hpp"
#include "maze-plugin-csharp-editor-tools/MazeCSharpEditorToolsPlugin.hpp"
#include "maze-plugin-console/MazeConsolePlugin.hpp"
#include "maze-plugin-console/MazeConsoleService.hpp"
#include "maze-plugin-csharp/ecs/components/MazeMonoBehaviour.hpp"
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
        MAZE_LOAD_PLATFORM_PLUGIN(Console);

        return true;
    }


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

        MAZE_LOAD_PLATFORM_PLUGIN(CSharp);
        MAZE_LOAD_PLATFORM_PLUGIN(CSharpEditorTools);
        testMono();

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
        canvasScaler->setScaleMode(CanvasScalerScaleMode::ScaleWithViewportSize);
        canvasScaler->setScreenMatchMode(CanvasScalerScreenMatchMode::MatchWidthOrHeight);
        canvasScaler->setMatchWidthOrHeight(1.0f);
        canvasScaler->updateCanvasScale();


        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.subscribe(this, &SceneExample::notifyMainRenderWindowViewportChanged);
        Example::GetInstancePtr()->getMainRenderWindow()->eventRenderTargetResized.subscribe(this, &SceneExample::notifyRenderTargetResized);


        // Light
        EntityPtr lightEntity = createEntity("Light");
        Light3DPtr mainLight3D = lightEntity->createComponent<Light3D>();
        mainLight3D->setColor(ColorU32(255, 244, 214));
        mainLight3D->getTransform()->setLocalRotation(0.851863f, 0.116778f, -0.330708f, -0.389006f);
        mainLight3D->getTransform()->setLocalPosition(5.0f, 5.0f, 5.0f);


        // Camera
        EntityPtr cameraEntity = createEntity("Camera");
        m_camera3D = cameraEntity->createComponent<Camera3D>();
        m_camera3D->getTransform()->setLocalPosition(Vec3F32(0.0f, 0.0f, -5.0f));
        m_camera3D->setClearColorFlag(true);
        m_camera3D->setRenderMask(m_camera3D->getRenderMask() | (S32)DefaultRenderMask::Gizmos);
        m_camera3D->setNearZ(0.01f);
        m_camera3D->setFarZ(100.0f);
        m_camera3D->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
        m_camera3D->setRenderTarget(Example::GetInstancePtr()->getMainRenderWindow());


        EntityPtr objectEntity = createEntity();
        Transform3DPtr transform = objectEntity->createComponent<Transform3D>();
        MeshRendererPtr meshRenderer = objectEntity->createComponent<MeshRenderer>();
        meshRenderer->setRenderMesh(RenderMeshManager::GetCurrentInstancePtr()->getDefaultCubeMesh());
        
        meshRenderer->setMaterial(MaterialManager::GetCurrentInstance()->getBuiltinMaterial(BuiltinMaterialType::SpecularDS));

        objectEntity->ensureComponent<Name>("Obj");


        objectEntity->createComponent<MonoBehaviour>(MAZE_HCS("Sandbox.Player"));
        objectEntity->createComponent<MonoBehaviour>(MAZE_HCS("Sandbox.TestRotor"));

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
        EcsRenderScene::update(_dt);

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

    //////////////////////////////////////////
    void CppFunction()
    {
        Debug::Log("CppFunction!!!");
    }

    //////////////////////////////////////////
    void NativeLog(MonoString* _string, S32 _arg0)
    {
        Char* cstr = mono_string_to_utf8(_string);
        String str(cstr);
        mono_free(cstr);

        Debug::log << "NativeLog: " << str << " => " << _arg0 << endl;
    }

    //////////////////////////////////////////
    void NativeLogVector3(Vec3F const& _arg0)
    {
        Debug::log << "NativeLog: " << _arg0.x << ", " << _arg0.y << ", " << _arg0.z << endl;
    }

    //////////////////////////////////////////
    void CrossVector3(
        Vec3F const& _arg0,
        Vec3F const& _arg1,
        Vec3F& _result)
    {
        _result = _arg0.crossProduct(_arg1);
    }

    //////////////////////////////////////////
    void SceneExample::testMono()
    {
        MonoDomain* monoDomain = CSharpService::GetInstancePtr()->getMonoDomain();

        mono_add_internal_call("Maze.Test::CppFunction", CppFunction);
        mono_add_internal_call("Maze.Test::NativeLog", NativeLog);
        mono_add_internal_call("Maze.Test::NativeLogVector3", NativeLogVector3);
        mono_add_internal_call("Maze.Test::CrossVector3", CrossVector3);

        MonoAssembly* testAssembly = CSharpService::GetInstancePtr()->loadMonoAssembly(MAZE_HCS("maze-csharp-test-lib.dll"));

        // Tests
        MonoImage* assemblyImage = mono_assembly_get_image(testAssembly);
        MonoClass* monoClass = mono_class_from_name(assemblyImage, "Maze", "Test");

        // 1. Create C# object (and call constructor)
        MonoObject* instance = mono_object_new(monoDomain, monoClass);
        mono_runtime_object_init(instance);

        // 2. Call C# method
        MonoMethod* printMessageFunc = mono_class_get_method_from_name(monoClass, "PrintMessage", 0);
        mono_runtime_invoke(printMessageFunc, instance, nullptr, nullptr);

        // 3. Call C# method with param
        {
            MonoMethod* printIntFunc = mono_class_get_method_from_name(monoClass, "PrintInt", 1);
            S32 value = 5;
            void* param = &value;
            mono_runtime_invoke(printIntFunc, instance, &param, nullptr);
        }
        {
            MonoMethod* printIntsFunc = mono_class_get_method_from_name(monoClass, "PrintInts", 2);
            S32 value0 = 42;
            S32 value1 = 12;
            void* params[2] =
            {
                &value0,
                &value1,
            };
            mono_runtime_invoke(printIntsFunc, instance, params, nullptr);
        }
        {
            MonoMethod* printCustomMessageFunc = mono_class_get_method_from_name(monoClass, "PrintCustomMessage", 1);
            MonoString* monoString = mono_string_new(monoDomain, "Hello World from C++!");
            void* stringParam = monoString;
            mono_runtime_invoke(printCustomMessageFunc, instance, &stringParam, nullptr);
        }

        // 4. Call C++ function from C#
        MonoMethod* callCppFunctionFunc = mono_class_get_method_from_name(monoClass, "CallCppFunction", 0);
        mono_runtime_invoke(callCppFunctionFunc, instance, nullptr, nullptr);

        // 5. Call C++ functions with params from C#
        MonoMethod* callCppFunctionWithParamsFunc = mono_class_get_method_from_name(monoClass, "CallCppFunctionWithParams", 0);
        mono_runtime_invoke(callCppFunctionWithParamsFunc, instance, nullptr, nullptr);
    }


} // namespace Maze
//////////////////////////////////////////
