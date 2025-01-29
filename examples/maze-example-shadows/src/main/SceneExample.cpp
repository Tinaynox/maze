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
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/math/MazeTMat.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/MazeMeshSkeletonAnimator.hpp"
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
#include "maze-graphics/ecs/components/MazeSkinnedMeshRenderer.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderMZMESH.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
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


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    String GetExampleName()
    {
        return "Shadows";
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
        MAZE_LOAD_PLATFORM_PLUGIN(LoaderDDS);
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
        if (!BaseSceneExample::init(Vec2F(20.0f, 20.0f)))
            return false;

        m_camera3D->getTransform()->setLocalRotationDegrees(0.0f, 180.0f, 0.0f);
        m_fpsController->setYawAngle(Math::DegreesToRadians(180.0f));
        m_fpsController->setPitchAngle(Math::DegreesToRadians(15.0f));
        //m_mainLight3D->getTransform()->setLocalDirection(0.577f, -0.577f, 0.577f);
        m_mainLight3D->getTransform()->setLocalPosition(-3.822013f, 1.546563f, 5.639657f);
        m_mainLight3D->getTransform()->setLocalRotation(0.540104f, 0.459414f, 0.682045f, -0.179000f);
        m_mainLight3D->setShadowCastSize(5.0f);
        m_mainLight3D->setShadowCast(true);


        m_fpsController->setPosition(Vec3F(0.0f, 0.0f, 2.1f));

        InputManager::GetInstancePtr()->eventKeyboard.subscribe(this, &SceneExample::notifyKeyboard);


        getLightingSettings()->setSkyBoxMaterial("Skybox00.mzmaterial");
        m_simpleLevelConfig.floorMaterial = MaterialManager::GetCurrentInstance()->getOrLoadMaterial("Chessboard00.mzmaterial");
        m_simpleLevelConfig.wallMaterial = MaterialManager::GetCurrentInstance()->getOrLoadMaterial("Chessboard00.mzmaterial");

        ExampleHelper::BuildSimpleLevel(
            this,
            m_fpsController->getLevelSize(),
            m_simpleLevelConfig);

        m_mainLight3D->getTransform()->setLocalPosition(0.0f, 3.5f, 0.0f);
        m_mainLight3D->getTransform()->setLocalRotationDegrees(90.0f, 0.0f, 0.0f);

        auto meshEntity = createEntity("Mesh");
        auto meshTransform = meshEntity->createComponent<Transform3D>();
        meshTransform->setLocalY(1.5f);
        auto meshRenderer = meshEntity->createComponent<MeshRenderer>();
        meshRenderer->setRenderMesh("Cube");
        meshRenderer->setMaterial("Test00.mzmaterial");

        {
            EntityPtr skinnedMeshEntity = createEntity("Buckethead");
            Transform3DPtr skinnedMeshTransform = skinnedMeshEntity->createComponent<Transform3D>();
            skinnedMeshTransform->setLocalZ(0.0f);
            skinnedMeshTransform->setLocalScale(0.2f);
            skinnedMeshTransform->setLocalX(3.0f);
            auto skinnedMeshRenderer = skinnedMeshEntity->createComponent<SkinnedMeshRenderer>();
            skinnedMeshRenderer->setRenderMesh("Buckethead.fbx");
            skinnedMeshRenderer->setMaterial("Buckethead.mzmaterial");
            skinnedMeshRenderer->playAnimation(MAZE_HCS("Run"));
            skinnedMeshRenderer->getAnimator()->getCurrentAnimation()->rewindToRandom();
        }

        auto depthBufferTexture = m_camera3D->getEntityRaw()->getComponent<Camera3DShadowBuffer>()->getShadowBuffer()->getDepthTexture()->cast<Texture2D>();
        SpritePtr shadowSprite = Sprite::Create(depthBufferTexture);

       
        SpriteRenderer2DPtr spriteRenderer = SpriteHelper::CreateSprite(
            shadowSprite,
            Vec2F(256.0f, 256.0f),
            Vec2F::c_zero,
            MaterialManager::GetCurrentInstance()->getOrLoadMaterial("ShadowBuffer00.mzmaterial"),
            m_canvasUI->getTransform(),
            this,
            Vec2F(1.0f, 1.0f),
            Vec2F(1.0f, 1.0f));
        spriteRenderer->getMaterial()->setUniform(
            MAZE_HCS("u_depthMap"),
            depthBufferTexture);


#if 0
        U8 pixels[128 * 128];
        for (U8& pixel : pixels)
            pixel = rand() % 255;
        m_shadowBuffer->getDepthTexture()->cast<Texture2D>()->copyImageFrom(
            (U8 const*)pixels, PixelFormat::R_U8, 128, 128, 0, 0);
#endif

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
        m_hintText->setTextFormatted(
            "[CONTROLS]\n"
            "Movement - WASD, Jump - Space, Camera - RMB (Hold)\n"
            "Change animation - 1, 2, 3\n"
        );
    }

    //////////////////////////////////////////
    void SceneExample::notifyKeyboard(InputEventKeyboardData const& _data)
    {
        switch (_data.type)
        {
            case InputEventKeyboardType::KeyDown:
            {
                if (_data.keyCode == KeyCode::Number1)
                {
                    
                }
                else
                if (_data.keyCode == KeyCode::Number2)
                {
                }
                else
                if (_data.keyCode == KeyCode::Number3)
                {
                }

                break;
            }
            default:
                break;
        }
    }


} // namespace Maze
//////////////////////////////////////////
