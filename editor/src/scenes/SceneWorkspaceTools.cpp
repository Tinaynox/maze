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
#include "SceneWorkspaceTools.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-core/math/MazeAnimationCurve.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
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
#include "maze-graphics/managers/MazeGizmosManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
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
#include "maze-editor-tools/ecs/components/MazeDebugGridRenderer.hpp"
#include "maze-editor-tools/helpers/MazeGizmosHelper.hpp"
#include "managers/EditorEntityManager.hpp"
#include "managers/EditorWorkspaceManager.hpp"
#include "managers/EditorManager.hpp"
#include "Editor.hpp"
#include "layout/EditorLayout.hpp"
#include "ecs/components/EditorMainCanvasController.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneWorkspaceTools
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneWorkspaceTools, SceneMainTools);

    //////////////////////////////////////////
    SceneWorkspaceTools::SceneWorkspaceTools()
    {


    }

    //////////////////////////////////////////
    SceneWorkspaceTools::~SceneWorkspaceTools()
    {
        InputManager* inputManager = InputManager::GetInstancePtr();
        if (inputManager)
        {
            inputManager->eventMouse.unsubscribe(this);
        }

        if (Editor::GetInstancePtr() && Editor::GetInstancePtr()->getMainRenderWindow())
        {
            Editor::GetInstancePtr()->getMainRenderWindow()->eventRenderTargetResized.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    SceneWorkspaceToolsPtr SceneWorkspaceTools::Create()
    {
        SceneWorkspaceToolsPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneWorkspaceTools, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SceneWorkspaceTools::init()
    {
        RenderBufferPtr const& renderTarget = EditorWorkspaceManager::GetInstancePtr()->getWorkspaceRenderBuffer();
        if (!SceneMainTools::init(renderTarget))
            return false;

        InputManager* inputManager = InputManager::GetInstancePtr();
        inputManager->eventMouse.subscribe(this, &SceneWorkspaceTools::notifyMouse);

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        ShaderSystemPtr const& shaderSystem = renderSystem->getShaderSystem();
        SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();
        RenderMeshManagerPtr const& renderMeshManager = renderSystem->getRenderMeshManager();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        EntityManager* entityManager = EntityManager::GetInstancePtr();
        ECSWorldPtr const& world = entityManager->getDefaultWorld();

        Editor::GetInstancePtr()->getMainRenderWindow()->eventRenderTargetResized.subscribe(this, &SceneWorkspaceTools::notifyMainRenderWindowResized);

        // Camera
        EntityPtr cameraEntity = createEntity();
        m_camera3D = cameraEntity->createComponent<Camera3D>();
        m_camera3DTargetPosition = Vec3DF(3.0f, 3.0f, -5.0f);
        m_camera3D->getTransform()->setLocalPosition(m_camera3DTargetPosition);
        m_yawAngle = Math::DegreesToRadians(-30.0f);
        m_pitchAngle = Math::DegreesToRadians(20.0f);
        m_camera3D->getTransform()->setLocalRotation(Quaternion(m_pitchAngle, m_yawAngle, 0.0f));

        m_camera3D->setFOV(Math::DegreesToRadians(60));
        m_camera3D->setRenderTarget(m_renderTarget);
        m_camera3D->setClearColor(ColorU32(99, 101, 140, 255));
        m_camera3D->getEntityRaw()->ensureComponent<Name>("Camera");
        m_camera3D->setRenderMask(m_camera3D->getRenderMask() | (S32)DefaultRenderMask::Gizmos);
        GizmosManager::GetInstancePtr()->setCamera(m_camera3D);

        // DebugGrid
        EntityPtr debugGridRendererEntity = createEntity();
        m_debugGridRenderer = debugGridRendererEntity->createComponent<DebugGridRenderer>(m_camera3D);

        // Axes
        EntityPtr axesMeshRendererEntity = createEntity();
        axesMeshRendererEntity->createComponent<Transform3D>();
        m_debugAxesRenderer = axesMeshRendererEntity->createComponent<MeshRenderer>();
        m_debugAxesRenderer->setRenderMesh(RenderMesh::Create(MeshHelper::CreateCoordinateAxes()));
        m_debugAxesRenderer->setMaterial(GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getBuiltinMaterial(BuiltinMaterialType::DebugAxis));
        m_debugAxesRenderer->getRenderMask()->setMask(DefaultRenderMask::Gizmos);

        {
            EntityPtr mainCanvasEntity = createEntity();
            m_mainCanvas = mainCanvasEntity->createComponent<Canvas>();
            m_mainCanvas->setClearColorFlag(false);
            m_mainCanvas->setClearColor(ColorU32::c_zero);
            m_mainCanvas->setRenderTarget(m_renderTarget);
            m_mainCanvas->setSortOrder(-1000000);

            EntityPtr mainCanvasControllerEntity = createEntity();
            EditorMainCanvasControllerPtr mainCanvasController = EditorMainCanvasController::Create(m_mainCanvas.get());
            mainCanvasControllerEntity->addComponent(mainCanvasController);
        }

        return true;
    }

    //////////////////////////////////////////
    void SceneWorkspaceTools::update(F32 _dt)
    {
        F32 cameraDistance = m_camera3D->getTransform()->getLocalPosition().length();
        GizmosHelper::PushTransform(Mat4DF::CreateScaleMatrix(cameraDistance * 0.1f));
       
        F32 const length = 2.0f;
        GizmosHelper::SetColor(ColorF128::c_red);
        GizmosHelper::DrawLine(
            Vec3DF::c_zero,
            Vec3DF::c_unitX * length,
            0.0f,
            GizmosDrawer::MeshRenderMode::Opaque);
        GizmosHelper::SetColor(ColorF128::c_green);
        GizmosHelper::DrawLine(
            Vec3DF::c_zero,
            Vec3DF::c_unitY * length,
            0.0f,
            GizmosDrawer::MeshRenderMode::Opaque);
        GizmosHelper::SetColor(ColorF128::c_blue);
        GizmosHelper::DrawLine(
            Vec3DF::c_zero,
            Vec3DF::c_unitZ * length,
            0.0f,
            GizmosDrawer::MeshRenderMode::Opaque);

        GizmosHelper::DrawCone(
            Vec3DF::c_unitX * length,
            Vec3DF::c_unitX,
            0.135f,
            0.475f,
            ColorF128::c_red);
        GizmosHelper::DrawCone(
            Vec3DF::c_unitY * length,
            Vec3DF::c_unitY,
            0.135f,
            0.475f,
            ColorF128::c_green);
        GizmosHelper::DrawCone(
            Vec3DF::c_unitZ * length,
            Vec3DF::c_unitZ,
            0.135f,
            0.475f,
            ColorF128::c_blue);
        GizmosHelper::PopTransform();

        _dt = UpdateManager::GetInstancePtr()->getUnscaledDeltaTime();

        ECSRenderScene::update(_dt);

        if (!Editor::GetInstancePtr()->getMainRenderWindow()->getFocused())
            return;

        Rect2DF viewportRect = m_camera3D->getViewport();
        viewportRect.position *= (Vec2DF)m_camera3D->getRenderTarget()->getRenderTargetSize();
        viewportRect.size *= (Vec2DF)m_camera3D->getRenderTarget()->getRenderTargetSize();

        AABB2D aabb = AABB2D::FromRect2D(viewportRect);

        Vec2DF cursorPositionRWS = InputManager::GetInstancePtr()->getCursorPosition(0);
        Vec2DF cursorPosition = EditorLayout::ConvertRenderWindowCoordsToWorkspaceViewport(cursorPositionRWS);

        if (m_camera3D && aabb.contains(cursorPosition))
        {
            Vec3DF cameraForwardDirection = m_camera3D->getTransform()->getLocalRotation() * Vec3DF::c_unitZ;
            Vec3DF cameraRightDirection = m_camera3D->getTransform()->getLocalRotation() * Vec3DF::c_unitX;

            F32 speed = 3.0f;

            if (InputManager::GetInstancePtr()->getKeyState(KeyCode::LShift) || InputManager::GetInstancePtr()->getKeyState(KeyCode::RShift))
            {
                speed *= 3.0f;
            }

            if (InputManager::GetInstancePtr()->getKeyState(KeyCode::W))
            {
                m_camera3DTargetPosition += cameraForwardDirection * _dt * speed;
            }
            else
            if (InputManager::GetInstancePtr()->getKeyState(KeyCode::S))
            {
                m_camera3DTargetPosition += -cameraForwardDirection * _dt * speed;
            }

            if (InputManager::GetInstancePtr()->getKeyState(KeyCode::A))
            {
                m_camera3DTargetPosition += -cameraRightDirection * _dt * speed;
            }
            else
            if (InputManager::GetInstancePtr()->getKeyState(KeyCode::D))
            {
                m_camera3DTargetPosition += cameraRightDirection * _dt * speed;
            }
            else
            if (InputManager::GetInstancePtr()->getKeyState(KeyCode::Q))
            {
                m_yawAngle -= _dt * Math::c_pi;
            }
            else
            if (InputManager::GetInstancePtr()->getKeyState(KeyCode::E))
            {
                m_yawAngle += _dt * Math::c_pi;
            }
        }


        if (m_camera3D)
        {
            m_camera3D->getTransform()->setLocalPosition(
                Math::Lerp(
                    m_camera3D->getTransform()->getLocalPosition(),
                    m_camera3DTargetPosition,
                    _dt * 16.0f));

            Quaternion q = Quaternion::Slerp(
                36.0f * _dt,
                m_camera3D->getTransform()->getLocalRotation(),
                Quaternion(m_pitchAngle, m_yawAngle, 0.0f));
            m_camera3D->getTransform()->setLocalRotation(q);
        }
    }

    //////////////////////////////////////////
    void SceneWorkspaceTools::notifyMouse(InputEventMouseData const& _data)
    {
        if (!Editor::GetInstancePtr()->getMainRenderWindow()->getFocused())
            return;

        switch (_data.type)
        {
            case InputEventMouseType::Move:
            {
                Vec2DF cursorPosition = Vec2DF((F32)_data.x, (F32)_data.y);

                if (m_cursorDrag)
                {
                    Vec2DF deltaPosition = cursorPosition - m_cursorPositionLastFrame;

                    if (m_camera3D && m_camera3D->getEntityRaw()->getActiveSelf())
                    {
                        m_yawAngle += deltaPosition.x * 0.0075f;
                        m_pitchAngle -= deltaPosition.y * 0.0075f;
                    }
                }

                m_cursorPositionLastFrame = cursorPosition;
                break;
            }
            case InputEventMouseType::ButtonDown:
            {
                if (_data.buttonId == 1)
                {
                    if (m_camera3D && m_camera3D->getEntityRaw()->getActiveSelf())
                    {
                        Vec2DF cursorPositionRWS = Vec2DF((F32)_data.x, (F32)_data.y);
                        Vec2DF cursorPosition = EditorLayout::ConvertRenderWindowCoordsToWorkspaceViewport(cursorPositionRWS);
                        Rect2DF viewportRect(
                            m_camera3D->getViewport().position.x * m_renderTarget->getRenderTargetSize().x,
                            m_camera3D->getViewport().position.y * m_renderTarget->getRenderTargetSize().y,
                            m_camera3D->getViewport().size.x * m_renderTarget->getRenderTargetSize().x,
                            m_camera3D->getViewport().size.y * m_renderTarget->getRenderTargetSize().y);

                        if (viewportRect.contains(cursorPosition))
                        {
                            m_cursorDrag = true;
                        }
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
    ECSWorld* SceneWorkspaceTools::assignWorld()
    {
        return EditorEntityManager::GetInstancePtr()->getWorkspaceWorld().get();
    }

    //////////////////////////////////////////
    void SceneWorkspaceTools::notifyMainRenderWindowResized(RenderTarget* _renderTarget)
    {
    }


} // namespace Maze
//////////////////////////////////////////