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
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
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
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-physics2d/ecs/components/MazeBoxCollider2D.hpp"
#include "maze-physics2d/ecs/components/MazeCircleCollider2D.hpp"
#include "maze-physics2d/ecs/components/MazeRigidbody2D.hpp"
#include "maze-plugin-profiler-view/MazeProfilerViewPlugin.hpp"
#include "maze-plugin-loader-png/MazeLoaderPNGPlugin.hpp"
#include "Example.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    String GetExampleName()
    {
        return "Physics2D";
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
        MAZE_LOAD_PLATFORM_PLUGIN(ProfilerView, "maze-plugin-profiler-view");
        MAZE_LOAD_PLATFORM_PLUGIN(LoaderPNG, "maze-plugin-loader-png");

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
        }

        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.unsubscribe(this);
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


        S32 p0 = EntityManager::GetInstancePtr()->getComponentPriority<Transform3D>();
        S32 p1 = EntityManager::GetInstancePtr()->getComponentPriority<Camera3D>();


        InputManager* inputManager = InputManager::GetInstancePtr();

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        ShaderSystemPtr const& shaderSystem = renderSystem->getShaderSystem();
        SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();
        RenderMeshManagerPtr const& renderMeshManager = renderSystem->getRenderMeshManager();

        EntityManager* entityManager = EntityManager::GetInstancePtr();
        ECSWorldPtr const& world = entityManager->getDefaultWorld();

        RenderWindowPtr const& renderTarget = Example::GetInstancePtr()->getMainRenderWindow();
        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.subscribe(this, &SceneExample::notifyMainRenderWindowViewportChanged);


        // Light
        EntityPtr lightEntity = createEntity();
        Light3DPtr mainLight3D = lightEntity->createComponent<Light3D>();
        mainLight3D->setColor(ColorU32(255, 244, 214));
        mainLight3D->getTransform()->setLocalDirection(0.577f, -0.577f, 0.577f);
        mainLight3D->getTransform()->setLocalPosition(0.0f, 5.0f, -5.0f);
        lightEntity->ensureComponent<Name>("Light");


        // Camera
        EntityPtr cameraEntity = createEntity();
        m_camera3D = cameraEntity->createComponent<Camera3D>();
        m_camera3D->getTransform()->setLocalPosition(Vec3DF(0.0f, 0.5f, -18.0f));
        m_camera3D->setFOV(Math::DegreesToRadians(30));
        
        m_camera3D->setRenderTarget(renderTarget);
        m_camera3D->setRenderMask(m_camera3D->getRenderMask() & ~(S32)DefaultRenderMask::Gizmos);
        m_camera3D->getEntityRaw()->ensureComponent<Name>("Camera");
        m_camera3D->setNearZ(0.01f);
        m_camera3D->setFarZ(30.0f);
        m_camera3D->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());


        // Floor
        MaterialPtr plastic00Material = Material::Create("Plastic00.mzmaterial");
        MaterialPtr plastic01Material = Material::Create("Plastic01.mzmaterial");
        {
            EntityPtr meshRendererEntity = createEntity();
            Transform3DPtr transform = meshRendererEntity->createComponent<Transform3D>();
            MeshRendererPtr meshRenderer = meshRendererEntity->createComponent<MeshRenderer>();
            meshRenderer->setRenderMesh(renderMeshManager->getDefaultCubeMesh());
            meshRenderer->setMaterial(plastic00Material);

            transform->setLocalPosition(0.0f, -2.0f, 0.0f);
            transform->setLocalScale(10.0f, 1.0f);

            meshRendererEntity->ensureComponent<BoxCollider2D>();
            Rigidbody2DPtr rigidbody2D = meshRendererEntity->ensureComponent<Rigidbody2D>();
            rigidbody2D->setType(Rigidbody2DType::Static);
            rigidbody2D->rebuildBody();

            meshRendererEntity->ensureComponent<Name>("Floor");
        }


        // Left Wall
        {
            EntityPtr meshRendererEntity = createEntity();
            Transform3DPtr transform = meshRendererEntity->createComponent<Transform3D>();
            MeshRendererPtr meshRenderer = meshRendererEntity->createComponent<MeshRenderer>();
            meshRenderer->setRenderMesh(renderMeshManager->getDefaultCubeMesh());
            meshRenderer->setMaterial(plastic00Material);

            transform->setLocalPosition(-5.0f, 2.0f - 2.0f, 0.0f);
            transform->setLocalScale(1.0f, 5.0f);

            meshRendererEntity->ensureComponent<BoxCollider2D>();
            Rigidbody2DPtr rigidbody2D = meshRendererEntity->ensureComponent<Rigidbody2D>();
            rigidbody2D->setType(Rigidbody2DType::Static);

            meshRendererEntity->ensureComponent<Name>("Left Wall");
        }

        // Right Wall
        {
            EntityPtr meshRendererEntity = createEntity();
            Transform3DPtr transform = meshRendererEntity->createComponent<Transform3D>();
            MeshRendererPtr meshRenderer = meshRendererEntity->createComponent<MeshRenderer>();
            meshRenderer->setRenderMesh(renderMeshManager->getDefaultCubeMesh());
            meshRenderer->setMaterial(plastic00Material);

            transform->setLocalPosition(5.0f, 2.0f - 2.0f, 0.0f);
            transform->setLocalScale(1.0f, 5.0f);

            meshRendererEntity->ensureComponent<BoxCollider2D>();
            Rigidbody2DPtr rigidbody2D = meshRendererEntity->ensureComponent<Rigidbody2D>();
            rigidbody2D->setType(Rigidbody2DType::Static);
            
            meshRendererEntity->ensureComponent<Name>("Right Wall");
        }

        // Test box
        for (S32 i = 0; i < 8; ++i)
        {
            {
                EntityPtr meshRendererEntity = createEntity();
                Transform3DPtr transform = meshRendererEntity->createComponent<Transform3D>();
                MeshRendererPtr meshRenderer = meshRendererEntity->createComponent<MeshRenderer>();
                meshRenderer->setRenderMesh(renderMeshManager->getDefaultCubeMesh());
                meshRenderer->setMaterial(plastic01Material);

                transform->setLocalPosition((F32)i - 4.0f, 2.0f + (F32)i + 20.0f);
                transform->setLocalScale(1.0f, 0.75f);
                transform->setLocalRotation(Quaternion(0.3f - (F32)i, Vec3DF::c_unitZ));

                BoxCollider2DPtr boxCollider = meshRendererEntity->ensureComponent<BoxCollider2D>();
                Rigidbody2DPtr rigidbody2D = meshRendererEntity->ensureComponent<Rigidbody2D>();
                rigidbody2D->setType(Rigidbody2DType::Dynamic);
                
                meshRendererEntity->ensureComponent<Name>("Box2D");
            }

            {
                EntityPtr meshRendererEntity = createEntity();
                Transform3DPtr transform = meshRendererEntity->createComponent<Transform3D>();
                MeshRendererPtr meshRenderer = meshRendererEntity->createComponent<MeshRenderer>();
                meshRenderer->setRenderMesh(renderMeshManager->getDefaultSphereMesh());
                meshRenderer->setMaterial(plastic01Material);

                transform->setLocalPosition((F32)i - 4.0f, 2.0f + (F32)i + 45.0f);
                transform->setLocalScale(1.0f, 1.0f);
                transform->setLocalRotation(Quaternion(0.3f - (F32)i, Vec3DF::c_unitZ));

                meshRendererEntity->ensureComponent<CircleCollider2D>();
                Rigidbody2DPtr rigidbody2D = meshRendererEntity->ensureComponent<Rigidbody2D>();
                rigidbody2D->setType(Rigidbody2DType::Dynamic);

                meshRendererEntity->ensureComponent<Name>("Circle2D");
            }

            {
                EntityPtr meshRendererEntity = createEntity();
                Transform3DPtr transform = meshRendererEntity->createComponent<Transform3D>();
                MeshRendererPtr meshRenderer = meshRendererEntity->createComponent<MeshRenderer>();
                meshRenderer->setRenderMesh(renderMeshManager->getDefaultCubeMesh());
                meshRenderer->setMaterial(plastic01Material);

                transform->setLocalPosition((F32)i - 4.0f, 2.0f + (F32)i + 70.0f);
                transform->setLocalScale(1.0f, 0.75f);
                transform->setLocalRotation(Quaternion(0.3f - (F32)i, Vec3DF::c_unitZ));

                meshRendererEntity->ensureComponent<BoxCollider2D>();
                Rigidbody2DPtr rigidbody2D = meshRendererEntity->ensureComponent<Rigidbody2D>();
                rigidbody2D->setType(Rigidbody2DType::Dynamic);

                meshRendererEntity->ensureComponent<Name>("Box2D");
            }
        }

        return true;
    }

    //////////////////////////////////////////
    void SceneExample::notifyMainRenderWindowViewportChanged(Rect2DF const& _mainRenderWindowViewport)
    {
        m_camera3D->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
    }

    //////////////////////////////////////////
    void SceneExample::update(F32 _dt)
    {
        ECSRenderScene::update(_dt);
    }


} // namespace Maze
//////////////////////////////////////////
