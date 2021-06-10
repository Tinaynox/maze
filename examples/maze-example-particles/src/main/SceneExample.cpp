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
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
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
#include "main/LevelBloomController.hpp"
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
        _sceneManager->loadScene<SceneExample>();
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

        Vec2DU renderBufferSize = Example::GetInstancePtr()->getMainRenderWindowAbsoluteSize();
        m_renderBuffer = RenderBuffer::Create(
            {
                renderBufferSize,
                PixelFormat::RGBA_F16,
                PixelFormat::DEPTH_U24
            });
        m_renderBuffer->getColorTexture()->setMinFilter(TextureFilter::Linear);
        m_renderBuffer->getColorTexture()->setMagFilter(TextureFilter::Linear);



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


        MaterialPtr const& postFXMaterial = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getMaterialManager()->getMaterial("PostFX00.mzmaterial");
        m_renderColorSprite = SpriteHelper::CreateSprite(
            Sprite::Create(m_renderBuffer->getColorTexture()),
            m_canvas->getTransform()->getSize(),
            Vec2DF::c_zero,
            postFXMaterial,
            m_canvas->getTransform(),
            this);
        m_renderColorSprite->getTransform()->setZ(1000);
        m_renderColorSprite->getEntityRaw()->ensureComponent<Name>("RenderColorSprite");
        m_renderColorSprite->getEntityRaw()->ensureComponent<SizePolicy2D>();


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
        Example::GetInstancePtr()->getMainRenderWindow()->eventRenderTargetResized.subscribe(this, &SceneExample::notifyRenderTargetResized);


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
        m_camera3D->setClearColor(ColorU32(20, 20, 20));
        m_camera3D->setRenderTarget(m_renderBuffer);
        m_camera3D->setRenderMask(m_camera3D->getRenderMask() & ~(S32)DefaultRenderMask::Gizmos);
        m_camera3D->getEntityRaw()->ensureComponent<Name>("Camera");
        m_camera3D->setNearZ(0.01f);
        m_camera3D->setFarZ(100.0f);

        m_bloomController = LevelBloomController::Create(this);


        createParticleSystem();

        return true;
    }

    //////////////////////////////////////////
    void SceneExample::notifyMainRenderWindowViewportChanged(Rect2DF const& _mainRenderWindowViewport)
    {
        m_renderBuffer->setSize(Example::GetInstancePtr()->getMainRenderWindowAbsoluteSize());
    }

    //////////////////////////////////////////
    void SceneExample::notifyRenderTargetResized(RenderTarget* _renderTarget)
    {
        m_renderBuffer->setSize(Example::GetInstancePtr()->getMainRenderWindowAbsoluteSize());
    }

    //////////////////////////////////////////
    void SceneExample::update(F32 _dt)
    {
        ECSRenderScene::update(_dt);

        m_bloomController->update(_dt);
        m_renderColorSprite->getMaterial()->ensureUniform(
            "u_bloomMap",
            ShaderUniformType::UniformTexture2D)->set(
                m_bloomController->getBloomRenderBuffer()->getColorTexture());
    }

    //////////////////////////////////////////
    void SceneExample::createParticleSystem()
    {
        // Particle System
        EntityPtr psEntity = createEntity();
        ParticleSystem3DPtr ps = psEntity->ensureComponent<ParticleSystem3D>();
        ps->getTransform()->rotate(Vec3DF::c_unitX, -Math::c_halfPi);

        ps->getMainModule().setTransformPolicy(ParticleSystemTransformPolicy::World);
        ps->getMainModule().getLifetime().setConstant(0.9f);
        ps->getMainModule().getSpeed().setConstant(2.0f);
        ps->getMainModule().getRotation().setRandomBetweenConstants(0.0f, Math::c_twoPi);
        ps->getMainModule().getGravity().setConstant(0.0f);
        ps->getMainModule().getSize().setRandomBetweenConstants(2.0f, 3.0f);
        ps->getMainModule().getRotationOverLifetime().enabled = true;

        MaterialPtr material = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getMaterialManager()->getMaterial("Fireball00.mzmaterial");

        ps->getRendererModule().getTextureSheetAnimation().enabled = true;
        ps->getRendererModule().getTextureSheetAnimation().tiles = Vec2DS(7, 7);
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
