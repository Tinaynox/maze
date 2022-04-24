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
#include "SceneTest00.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeRotor3D.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/loaders/texture/MazeLoaderPNG.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/MazeGPUTextureBuffer.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-editor-tools/settings/MazeEditorToolsSettings.hpp"
#include "Game.hpp"
#include "scenes/SceneFadePreloader.hpp"
#include "scenes/SceneMainMenu.hpp"
#include "scenes/SceneDebug.hpp"
#include "managers/GameManager.hpp"
#include "game/SpaceObject.hpp"
#include "configs/GameConfig.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneTest00
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneTest00, ECSRenderScene);

    //////////////////////////////////////////
    SceneTest00::SceneTest00()
        : m_timer(0.0f)
    {
    }

    //////////////////////////////////////////
    SceneTest00::~SceneTest00()
    {
        if (Game::GetInstancePtr())
        {
            Game::GetInstancePtr()->eventMainRenderWindowViewportChanged.unsubscribe(this);
        }

        InputManager* inputManager = InputManager::GetInstancePtr();
        if (inputManager)
        {
            inputManager->eventMouse.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    SceneTest00Ptr SceneTest00::Create()
    {
        SceneTest00Ptr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneTest00, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SceneTest00::init()
    {
        RenderWindowPtr const& renderWindow = Game::GetInstancePtr()->getMainRenderWindow();
        if (!ECSRenderScene::init(renderWindow))
            return false;

        Game::GetInstancePtr()->eventMainRenderWindowViewportChanged.subscribe(this, &SceneTest00::notifyMainRenderWindowViewportChanged);

        create2D();

        Game::GetInstancePtr()->updateDebugEditor();

        SceneManager::GetInstancePtr()->loadScene<SceneDebug>();

        Game::GetInstancePtr()->eventCoreGameResourcesLoaded();


        return true;
    }

    //////////////////////////////////////////
    void SceneTest00::update(F32 _dt)
    {
        m_timer += _dt;
        F32 progress = (Math::Sin(m_timer * 1.0f) + 1) * 0.5f;
        m_spaceObjectMaterial->setUniform("u_dissolveProgress", progress);
    }

    //////////////////////////////////////////
    void SceneTest00::create2D()
    {
        EntityPtr canvasEntity = createEntity();
        m_canvas = canvasEntity->createComponent<Canvas>();
        m_canvas->setViewport(Game::GetInstancePtr()->getMainRenderWindowViewport());
        m_canvas->setRenderTarget(Game::GetInstancePtr()->getMainRenderWindow());
        m_canvas->setClearColorFlag(false);
        m_canvas->setClearColor(ColorU32::c_gray);
        m_canvas->getEntityRaw()->ensureComponent<Name>("Canvas");

        CanvasScalerPtr canvasScaler = canvasEntity->ensureComponent<CanvasScaler>();
        canvasScaler->setReferenceResolution(c_canvasReferenceResolution);
        canvasScaler->setScaleMode(CanvasScaler::ScaleMode::ScaleWithViewportSize);
        canvasScaler->setScreenMatchMode(CanvasScaler::ScreenMatchMode::MatchWidthOrHeight);
        canvasScaler->setMatchWidthOrHeight(1.0f);

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();

        MaterialPtr spriteMaterial = 
            GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getMaterialManager()->getMaterial("SplashUI00.mzmaterial");
        spriteManager->setDefaultSpriteMaterial(spriteMaterial);


        EntityPtr cameraEntity = createEntity();
        Camera3DPtr camera3D = cameraEntity->createComponent<Camera3D>();
        camera3D->getTransform()->setLocalPosition(Vec3DF(0.0f, 0.5f, -18.0f));
        camera3D->setFOV(Math::DegreesToRadians(30));
        camera3D->setViewport(Game::GetInstancePtr()->getMainRenderWindowViewport());
        camera3D->setRenderTarget(Game::GetInstancePtr()->getMainRenderWindow());
        camera3D->getEntityRaw()->ensureComponent<Name>("Camera");


        RenderMeshPtr renderMesh = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getRenderMeshManager()->getDefaultQuadMesh();
        


        RenderMeshPtr spaceObjectRenderMesh = renderSystem->getRenderMeshManager()->getRenderMesh("ShininessTest.obj");
        m_spaceObjectMaterial = renderSystem->getMaterialManager()->getMaterial("ShininessTest.mzmaterial");
        
        {
            EntityPtr spaceObjectEntity = createEntity();
            Transform3DPtr transform = spaceObjectEntity->createComponent<Transform3D>();
            MeshRendererPtr meshRenderer = spaceObjectEntity->createComponent<MeshRenderer>();
            meshRenderer->setRenderMesh(spaceObjectRenderMesh);
            meshRenderer->setMaterials( { m_spaceObjectMaterial } );
            // transform->setLocalRotation(Quaternion(0.0f, -c_halfPi, 0.0f));
            SpaceObjectPtr spaceObject = spaceObjectEntity->createComponent<SpaceObject>(nullptr);

            transform->setLocalScale(Vec3DF(0.015f, 0.015f, 0.015f));

            spaceObjectEntity->ensureComponent<Name>("SpaceObject");
        }

    }

    //////////////////////////////////////////
    void SceneTest00::notifyMainRenderWindowViewportChanged(Rect2DF const& _mainRenderWindowViewport)
    {
        m_canvas->setViewport(_mainRenderWindowViewport);
    }


} // namespace Maze
//////////////////////////////////////////
