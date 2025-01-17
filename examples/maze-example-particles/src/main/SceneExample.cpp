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
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
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
#include "maze-plugin-loader-png/MazeLoaderPNGPlugin.hpp"
#include "maze-plugin-particles-editor-tools/MazeParticlesEditorToolsPlugin.hpp"
#include "Example.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    String GetExampleName()
    {
        return "Particles";
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
        MAZE_LOAD_PLATFORM_PLUGIN(ParticlesEditorTools);

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
        if (!BaseSceneExample::init(Vec2F(100.0f, 30.0f)))
            return false;

        m_camera3D->setClearColor(ColorU32::c_blackSoft);
        m_camera3D->setClearColorFlag(true);

        createParticleSystem();

        return true;
    }

    //////////////////////////////////////////
    void SceneExample::update(F32 _dt)
    {
        BaseSceneExample::update(_dt);

    }

    //////////////////////////////////////////
    void SceneExample::createParticleSystem()
    {
        // Particle System
        EntityPtr psEntity = createEntity();
        ParticleSystem3DPtr ps = psEntity->ensureComponent<ParticleSystem3D>();
        ps->getTransform()->setLocalY(0.66f);
        ps->getTransform()->setLocalZ(-6.0f);
        ps->getTransform()->rotate(Vec3F32::c_unitX, -Math::c_halfPi);

        ps->getMainModule().setTransformPolicy(ParticleSystemSimulationSpace::World);
        ps->getMainModule().getLifetime().setConstant(0.9f);
        ps->getMainModule().getSpeed().setConstant(2.0f);
        ps->getMainModule().getRotation().setRandomBetweenConstants(0.0f, Math::c_twoPi);
        ps->getMainModule().getGravity().setConstant(0.0f);
        ps->getMainModule().getSize().setRandomBetweenConstants(2.0f, 3.0f);
        ps->getMainModule().getRotationOverLifetime().enabled = true;

        MaterialPtr material = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getMaterialManager()->getOrLoadMaterial("Fireball00.mzmaterial");

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

        ps->getRendererModule().setParticlesMaxCount(10000);
        ps->getMainModule().getEmission().emissionPerSecond.setConstant(10.0f);

        ps->getShapeModule().setType(ParticleSystem3DZoneType::Cone);
        ps->getShapeModule().getZoneData().cone.angle = 0.436f;
        ps->getShapeModule().getZoneData().cone.length = 0.001f;
        ps->getShapeModule().getZoneData().cone.radius = 0.619f;
        ps->getShapeModule().setShapeVolume(true);

        ps->setMaterial(material);
        ps->play();
        psEntity->ensureComponent<Name>("ParticleSystem");
    }

} // namespace Maze
//////////////////////////////////////////
