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
#include "SceneSplash.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-core/managers/MazeTaskManager.hpp"
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
#include "maze-graphics/managers/MazeGizmosManager.hpp"
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
#include "maze-ui/managers/MazeUIManager.hpp"
#include "main/SceneExample.hpp"
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
        _sceneManager->loadScene<SceneSplash>();
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

#if (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)
        if (SettingsManager::GetInstancePtr()->getSettingsRaw<ProfilerViewSettings>())
            SettingsManager::GetInstancePtr()->getSettingsRaw<ProfilerViewSettings>()->setActive(true);
#endif

        return true;
    }


    //////////////////////////////////////////
    // Class SceneSplash
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneSplash, ECSRenderScene);

    //////////////////////////////////////////
    SceneSplash::SceneSplash()
    {
    }

    //////////////////////////////////////////
    SceneSplash::~SceneSplash()
    {
        InputManager* inputManager = InputManager::GetInstancePtr();
        if (inputManager)
        {
            inputManager->eventMouse.unsubscribe(this);
            inputManager->eventTouch.unsubscribe(this);
        }

        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.unsubscribe(this);
    }

    //////////////////////////////////////////
    SceneSplashPtr SceneSplash::Create()
    {
        SceneSplashPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneSplash, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SceneSplash::init()
    {
        if (!ECSRenderScene::init(Example::GetInstancePtr()->getMainRenderWindow()))
            return false;

        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.subscribe(this, &SceneSplash::notifyMainRenderWindowViewportChanged);

        EntityPtr canvasEntity = createEntity("Canvas");
        m_canvas = canvasEntity->createComponent<Canvas>();
        m_canvas->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
        m_canvas->setRenderTarget(Example::GetInstancePtr()->getMainRenderWindow());
        m_canvas->setClearColorFlag(true);
        m_canvas->setClearColor(ColorU32::c_blackSoft);
        m_canvas->setSortOrder(1000);

        CanvasScalerPtr canvasScaler = canvasEntity->ensureComponent<CanvasScaler>();
        canvasScaler->setScaleMode(CanvasScaler::ScaleMode::ScaleWithViewportSize);
        canvasScaler->setScreenMatchMode(CanvasScaler::ScreenMatchMode::MatchWidthOrHeight);
        canvasScaler->setMatchWidthOrHeight(1.0f);
        canvasScaler->updateCanvasScale();

        MaterialPtr const& material = MaterialManager::GetCurrentInstance()->ensureBuiltinMaterial(BuiltinMaterialType::ColorTexture);
        SpriteManager::GetCurrentInstance()->setDefaultSpriteMaterial(material);
        SpriteHelper::CreateSprite(
            "MazeLogo_128x128.mztexture",
            Vec2DF(128.0f, 128.0f),
            Vec2DF::c_zero,
            m_canvas->getTransform(),
            this);

        TaskManager::GetInstancePtr()->addDelayedMainThreadTask(
            1,
            []()
            {
                RenderSystem::GetCurrentInstancePtr()->createBuiltinAssets();
                GraphicsManager::GetInstancePtr()->getGizmosManager()->createGizmosElements();
                UIManager::GetInstancePtr()->createUIElements();
                ParticlesManager::GetInstancePtr()->createBuiltinAssets();

                MAZE_LOAD_PLATFORM_PLUGIN(Water, "maze-plugin-water");

                Example::GetInstancePtr()->eventCoreGameResourcesLoaded();
                
                SceneManager::GetInstancePtr()->loadScene<SceneExample>();
                TaskManager::GetInstancePtr()->addDelayedMainThreadTask(
                    2,
                    [](){ SceneManager::GetInstancePtr()->unloadScene<SceneSplash>(); });

            });

        return true;
    }

    //////////////////////////////////////////
    void SceneSplash::notifyMainRenderWindowViewportChanged(Rect2DF const& _mainRenderWindowViewport)
    {
        if (!Example::GetInstancePtr()->isMainWindowReadyToRender())
            return;

        m_canvas->setViewport(_mainRenderWindowViewport);
    }

    //////////////////////////////////////////
    void SceneSplash::update(F32 _dt)
    {
        ECSRenderScene::update(_dt);
    }

} // namespace Maze
//////////////////////////////////////////
