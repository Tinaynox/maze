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
#include "SceneWorkspace.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-core/math/MazeAnimationCurve.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
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
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-ui/managers/MazeColorPickerManager.hpp"
#include "maze-ui/ecs/components/MazeExperimental.hpp"
#include "maze-particles/ecs/components/MazeParticleSystem3D.hpp"
#include "maze-particles/managers/MazeParticlesManager.hpp"
#include "Editor.hpp"
#include "layout/EditorLayout.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneWorkspace
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneWorkspace, ECSRenderScene);

    //////////////////////////////////////////
    SceneWorkspace::SceneWorkspace()
    {


    }

    //////////////////////////////////////////
    SceneWorkspace::~SceneWorkspace()
    {
        InputManager* inputManager = InputManager::GetInstancePtr();
        if (inputManager)
        {
            inputManager->eventMouse.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    SceneWorkspacePtr SceneWorkspace::Create()
    {
        SceneWorkspacePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneWorkspace, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SceneWorkspace::init()
    {
        RenderWindowPtr const& renderWindow = Editor::GetInstancePtr()->getMainRenderWindow();
        if (!ECSRenderScene::init(renderWindow))
            return false;

        RenderMeshManagerPtr const& renderMeshManager = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getRenderMeshManager();

        makeMainScene();


        // createTest();

        /*
        EntityPtr experimental = createEntity();
        Transform3DPtr transform = experimental->createComponent<Transform3D>();
        ExperimentalPtr exp = experimental->createComponent<Experimental>();
        experimental->ensureComponent<Name>("Experimental");


        ColorGradient gradient;
        gradient.setMode(ColorGradient::EvaluateMode::Linear);
        gradient.addKey(0.0f, Vec4DF(1.0f, 0.0f, 0.0f, 1.0f));
        gradient.addKey(0.6f, Vec4DF(1.0f, 1.0f, 0.0f, 1.0f));
        gradient.addKey(1.0f, Vec4DF(0.0f, 1.0f, 1.0f, 0.5f));
        exp->setColorGradient(gradient);

        AnimationCurve curve;
        curve.setMode(AnimationCurve::EvaluateMode::Smooth);
        curve.addKey(0.0f, 0.0f);
        curve.addKey(0.3f, 0.75f);
        curve.addKey(0.7f, 0.3f);
        curve.addKey(1.0f, 0.9f);
        exp->setAnimationCurve(curve);

        AnimationCurve curve2(AnimationCurveMinMaxMode::Normalized);
        curve2.setMode(AnimationCurve::EvaluateMode::Smooth);
        curve2.addKey(0.1f, -0.95f);
        curve2.addKey(0.3f, 0.65f, 2.0f);
        curve2.addKey(0.45f, -0.15f, 1.0f);
        curve2.addKey(0.7f, 0.2f, -4.0f);
        curve2.addKey(0.9f, 0.9f);
        exp->setAnimationCurve2(curve2);
        */


        // Player ship
        /*
        EntityPtr playership = createEntity();
        Transform3DPtr transform = playership->createComponent<Transform3D>();
        m_meshRenderer = playership->createComponent<MeshRenderer>();
        m_meshRenderer->setRenderMesh("PlayerShip00.obj");
        m_meshRenderer->setMaterial("PlayerShip00.mzmaterial");
        transform->setLocalScale(0.01f);
        */


        /*
        EntityPtr playership = createEntity();
        Transform3DPtr transform = playership->createComponent<Transform3D>();
        m_meshRenderer = playership->createComponent<MeshRenderer>();
        // m_meshRenderer->setRenderMesh(GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getRenderMeshManager()->getDefaultSphereMesh());
        m_meshRenderer->setRenderMesh("PlayerShip00.obj");
        m_meshRenderer->setMaterial("PlayerShip00.mzmaterial");
        transform->setLocalScale(0.005f);
        */


        // Main Light
        /*
        EntityPtr lightEntity = createEntity();
        Light3DPtr mainLight3D = lightEntity->createComponent<Light3D>();
        mainLight3D->setColor(ColorU32(255, 244, 214));
        mainLight3D->getTransform()->setLocalDirection(0.577f, -0.577f, 0.577f);
        mainLight3D->getTransform()->setLocalPosition(0.0f, 5.0f, -5.0f);
        lightEntity->ensureComponent<Name>("Light");
        */


        return true;
    }

    //////////////////////////////////////////
    void SceneWorkspace::update(F32 _dt)
    {
        if (!Editor::GetInstancePtr()->getRunning())
            return;

        m_timer += _dt;

        // F32 x = 2 * Cos(m_timer * 3);
        // F32 z = 2 * Sin(m_timer * 3);
        // m_psTransform->setLocalPosition(x, 0.0f, z);
    }

    //////////////////////////////////////////
    void SceneWorkspace::createTest()
    {
        EntityPtr psEntity = createEntity();
        // psEntity->setActiveSelf(false);
        ParticleSystem3DPtr ps = psEntity->ensureComponent<ParticleSystem3D>();

        m_psTransform = ps->getTransform();

        ps->getMainModule().setTransformPolicy(ParticleSystemSimulationSpace::World);
        // ps->getMainModule().getLifetime().setConstant(1.0f);
        ps->getMainModule().getLifetime().setConstant(5.0f);
        // ps->getMainModule().getSpeed().setConstant(1.35f);
        ps->getMainModule().getSpeed().setRandomBetweenConstants(1.35f, 5.0f);
        ps->getMainModule().getRotation().setRandomBetweenConstants(0.0f, Math::c_twoPi);
        ps->getMainModule().getGravity().setConstant(0.0f);
        ps->getMainModule().getSize().setConstant(0.5f);
        // ps->getMainModule().getRotationOverLifetime().enabled = true;


        {
            AnimationCurve curve;
            curve.setMode(AnimationCurve::EvaluateMode::Smooth);
            curve.addKey(0.0f, 0.0f);
            curve.addKey(0.3f, 0.75f);
            curve.addKey(0.7f, 0.3f);
            curve.addKey(1.0f, 0.9f);

            ps->getMainModule().getSize().setCurve(curve);
        }


        //{

        //    AnimationCurve curve0;
        //    curve0.setMode(AnimationCurve::EvaluateMode::Smooth);
        //    curve0.addKey(0.0f, 4.0f);
        //    curve0.addKey(1.0f, 0.0f);

        //    AnimationCurve curve1;
        //    curve1.setMode(AnimationCurve::EvaluateMode::Smooth);
        //    curve1.addKey(0.0f, -4.0f);
        //    curve1.addKey(1.0f, 0.0f);

        //    ps->getMainModule().getRotationOverLifetime().parameter.setRandomBetweenCurves(curve0, curve1);
        //}




        //ps->getMainModule().getColorOverLifetime().enabled = true;
        //{
        //    ColorGradient gradient;
        //    gradient.addKey(0.0f, Vec4DF(1.0f, 0.0f, 0.0f, 0.0f));
        //    gradient.addKey(0.15f, Vec4DF(1.0f, 0.65f, 0.0f, 1.0f));
        //    gradient.addKey(0.3f, Vec4DF(1.0f, 1.0f, 0.0f, 1.0f));
        //    gradient.addKey(0.45f, Vec4DF(0.0f, 1.0f, 0.0f, 1.0f));
        //    gradient.addKey(0.6f, Vec4DF(0.0f, 0.65f, 1.0f, 1.0f));
        //    gradient.addKey(0.75f, Vec4DF(0.0f, 0.0f, 1.0f, 1.0f));
        //    gradient.addKey(0.9f, Vec4DF(1.0f, 0.0f, 1.0f, 1.0f));
        //    gradient.addKey(1.0f, Vec4DF(1.0f, 1.0f, 1.0f, 0.0f));
        //    ps->getMainModule().getColorOverLifetime().parameter.setGradient(gradient);
        //}

        ps->getMainModule().getColorOverLifetime().enabled = true;
        {
            ColorGradient gradient;
            gradient.addKey(0.0f, Vec4DF(1.0f, 1.0f, 1.0f, 0.0f));
            gradient.addKey(0.15f, Vec4DF(1.0f, 1.0f, 1.0f, 1.0f));
            gradient.addKey(0.85f, Vec4DF(1.0f, 1.0f, 1.0f, 1.0f));
            gradient.addKey(1.0f, Vec4DF(1.0f, 1.0f, 1.0f, 0.0f));
            ps->getMainModule().getColorOverLifetime().parameter.setGradient(gradient);
        }


        //ps->getMainModule().getSizeOverLifetime().enabled = true;
        //{
        //    AnimationCurve curve;
        //    curve.setMode(AnimationCurve::EvaluateMode::Smooth);
        //    curve.addKey(0.0f, 1.0f);
        //    curve.addKey(1.0f, 0.0f);
        //    ps->getMainModule().getSizeOverLifetime().parameter.setCurve(curve);
        //}

        //ps->getMainModule().getVelocityOverLifetime().enabled = true;
        //{
        //    AnimationCurve curveY;
        //    curveY.setMode(AnimationCurve::EvaluateMode::Smooth);
        //    curveY.addKey(0.0f, 240.0f);
        //    curveY.addKey(0.2f, -240.0f);
        //    curveY.addKey(0.4f, 240.0f);
        //    curveY.addKey(0.6f, -240.0f);
        //    curveY.addKey(0.8f, 240.0f);
        //    curveY.addKey(1.0f, -240.0f);
        //    ps->getMainModule().getVelocityOverLifetime().linearYParameter.setCurve(curveY);
        //}


        // ps->getMainModule().getSize().setRandomBetweenConstants(0.01f, 0.2f);
        // ps->getMainModule().getColor().setRandomBetweenConstants(ColorF128::c_yellow, ColorF128::c_cyan);




        // MaterialPtr material = ParticlesManager::GetInstancePtr()->getDefaultParticleMaterial()->createCopy();
        MaterialPtr material = ParticlesManager::GetInstancePtr()->getDefaultParticleMaterial();
        // material->setUniform("u_baseMap", GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getTextureManager()->getTexture2D("heart.mztexture"));
        // material->setUniform("u_baseMap", GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getTextureManager()->getTexture2D("confetti_outlined.mztexture"));


        // material->setUniform("u_baseMap", GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getTextureManager()->getTexture2D("FireExplosion.mztexture"));
        // material->getFirstRenderPass()->setBlend(
        //    BlendOperation::Add,
        //    BlendFactor::One,
        //    BlendFactor::One);


        ps->getRendererModule().getTextureSheetAnimation().enabled = false;
        ps->getRendererModule().getTextureSheetAnimation().tiles = Vec2DS(6, 6);
        //ps->getRendererModule().getTextureSheetAnimation().startFrame.setRandomBetweenConstants(0, 6 * 6);
        ps->getRendererModule().getTextureSheetAnimation().startFrame.setConstant(0);

        {
            AnimationCurve curve;
            curve.addKey(0.0f, 0.0f);
            curve.addKey(1.0f, 36.0f);
            ps->getRendererModule().getTextureSheetAnimation().frameOverTime.setCurve(curve);
        }

        // ps->getMainModule().getColor().setConstant(ColorF128::c_red);
        ps->getRendererModule().setParticlesMaxCount(10000);
        ps->getMainModule().getEmission().getEmissionPerSecond().setConstant(10.0f);

        ps->getShapeModule().setType(ParticleSystem3DZoneType::Cone);
        ps->getShapeModule().getZoneData().sphere.radius = 3.0f;
        ps->getShapeModule().getZoneData().hemisphere.radius = 3.0f;
        ps->getShapeModule().getZoneData().box.scale = Vec3DF(6.0f, 6.0f, 6.0f);
        ps->getShapeModule().getZoneData().torus.radius = 3.0f;
        ps->getShapeModule().getZoneData().torus.torusRadius = 1.0f;
        ps->getShapeModule().getZoneData().torus.radiusThickness = 1.0f;
        ps->getShapeModule().getZoneData().cone.angle = 0.5f;
        ps->getShapeModule().getZoneData().cone.length = 0.001f;
        // ps->getShapeModule().getZoneData().donut.scale = Vec3DF(6.0f, 6.0f, 6.0f);
        ps->getShapeModule().setShapeVolume(true);

        // ps->getRendererModule().setRenderMesh(renderMeshManager->getDefaultCubeMesh());
        // RenderMeshPtr const& renderMesh = renderMeshManager->getRenderMesh("PlayerShip00.obj");
        // ps->getRendererModule().setRenderMesh(renderMesh);

        //ps->setMaterial("PlayerShip00.mzmaterial");
        ps->setMaterial(material);
        ps->play();
        psEntity->ensureComponent<Name>("ParticleSystem");
    }


} // namespace Maze
//////////////////////////////////////////
