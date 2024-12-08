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
#include "ScenePlaytestTools.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeAnimationCurve.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
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
#include "maze-editor-tools/managers/MazeColorPickerManager.hpp"
#include "maze-ui/ecs/components/MazeExperimental.hpp"
#include "maze-particles/ecs/components/MazeParticleSystem3D.hpp"
#include "maze-particles/managers/MazeParticlesManager.hpp"
#include "maze-editor-tools/ecs/components/MazeDebugGridRenderer.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "managers/EditorEntityManager.hpp"
#include "managers/EditorPlaytestManager.hpp"
#include "managers/EditorManager.hpp"
#include "Editor.hpp"
#include "layout/EditorLayout.hpp"
#include "ecs/components/EditorMainCanvasController.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ScenePlaytestTools
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ScenePlaytestTools, SceneMainTools);

    //////////////////////////////////////////
    ScenePlaytestTools::ScenePlaytestTools()
    {


    }

    //////////////////////////////////////////
    ScenePlaytestTools::~ScenePlaytestTools()
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
    ScenePlaytestToolsPtr ScenePlaytestTools::Create()
    {
        ScenePlaytestToolsPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ScenePlaytestTools, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool ScenePlaytestTools::init()
    {
        RenderWindowPtr const& renderTarget = Editor::GetInstancePtr()->getMainRenderWindow();
        if (!SceneMainTools::init(renderTarget))
            return false;

        InputManager* inputManager = InputManager::GetInstancePtr();
        inputManager->eventMouse.subscribe(this, &ScenePlaytestTools::notifyMouse);

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        ShaderSystemPtr const& shaderSystem = renderSystem->getShaderSystem();
        SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();
        RenderMeshManagerPtr const& renderMeshManager = renderSystem->getRenderMeshManager();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        EntityManager* entityManager = EntityManager::GetInstancePtr();
        EcsWorldPtr const& world = entityManager->getDefaultWorld();

        Editor::GetInstancePtr()->getMainRenderWindow()->eventRenderTargetResized.subscribe(this, &ScenePlaytestTools::notifyMainRenderWindowResized);

        // Camera
        EntityPtr cameraEntity = createEntity();
        m_camera3D = cameraEntity->createComponent<Camera3D>();
        m_camera3DTargetPosition = Vec3F32(3.0f, 3.0f, -5.0f);
        m_camera3D->getTransform()->setLocalPosition(m_camera3DTargetPosition);
        m_yawAngle = Math::DegreesToRadians(-30.0f);
        m_pitchAngle = Math::DegreesToRadians(20.0f);
        m_camera3D->getTransform()->setLocalRotation(Quaternion(m_pitchAngle, m_yawAngle, 0.0f));

        m_camera3D->setFOV(Math::DegreesToRadians(60));
        m_camera3D->setRenderTarget(m_renderTarget);
        m_camera3D->setClearColor(ColorU32(99, 101, 140, 255));
        m_camera3D->getEntityRaw()->ensureComponent<Name>("Camera");
        m_camera3D->setRenderMask(m_camera3D->getRenderMask() | (S32)DefaultRenderMask::Gizmos);
        m_camera3D->setViewport(EditorLayout::CalculateWorkViewport(EditorLayout::c_previewViewport));
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
            m_mainCanvas->setViewport(EditorLayout::CalculateWorkViewport(EditorLayout::c_previewViewport));
            GizmosManager::GetInstancePtr()->setCanvas(m_mainCanvas);

            EntityPtr mainCanvasControllerEntity = createEntity();
            EditorMainCanvasControllerPtr mainCanvasController = EditorMainCanvasController::Create(m_mainCanvas.get());
            mainCanvasControllerEntity->addComponent(mainCanvasController);
        }

        return true;
    }

    //////////////////////////////////////////
    void ScenePlaytestTools::update(F32 _dt)
    {
        _dt = UpdateManager::GetInstancePtr()->getUnscaledDeltaTime();

        SceneMainTools::update(_dt);

        if (!Editor::GetInstancePtr()->getMainRenderWindow()->getFocused())
            return;

        Rect2F viewportRect = m_camera3D->getViewport();
        viewportRect.position *= (Vec2F32)m_camera3D->getRenderTarget()->getRenderTargetSize();
        viewportRect.size *= (Vec2F32)m_camera3D->getRenderTarget()->getRenderTargetSize();

        AABB2D aabb = AABB2D::FromRect2(viewportRect);

        Vec2F32 cursorPositionRWS = InputManager::GetInstancePtr()->getCursorPosition(0);
        if (m_camera3D && aabb.contains(cursorPositionRWS))
        {
            Vec3F32 cameraForwardDirection = m_camera3D->getTransform()->getLocalRotation() * Vec3F32::c_unitZ;
            Vec3F32 cameraRightDirection = m_camera3D->getTransform()->getLocalRotation() * Vec3F32::c_unitX;

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
    void ScenePlaytestTools::notifyMouse(InputEventMouseData const& _data)
    {
        if (!Editor::GetInstancePtr()->getMainRenderWindow()->getFocused())
            return;

        switch (_data.type)
        {
            case InputEventMouseType::Move:
            {
                Vec2F32 cursorPosition = Vec2F32((F32)_data.x, (F32)_data.y);

                if (m_cursorDrag)
                {
                    Vec2F32 deltaPosition = cursorPosition - m_cursorPositionLastFrame;

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
                        Vec2F32 cursorPosition = Vec2F32((F32)_data.x, (F32)_data.y);
                        Rect2F viewportRect(
                            m_camera3D->getViewport().position.x * m_renderTarget->getRenderTargetSize().x,
                            m_camera3D->getViewport().position.y * m_renderTarget->getRenderTargetSize().y,
                            m_camera3D->getViewport().size.x * m_renderTarget->getRenderTargetSize().x,
                            m_camera3D->getViewport().size.y * m_renderTarget->getRenderTargetSize().y);

                        if (viewportRect.contains(cursorPosition))
                        {
                            m_cursorDrag = true;
                            m_cursorPositionLastFrame = cursorPosition;
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
    EcsWorld* ScenePlaytestTools::assignWorld()
    {
        return EntityManager::GetInstancePtr()->getDefaultWorld().get();
    }

    //////////////////////////////////////////
    void ScenePlaytestTools::notifyMainRenderWindowResized(RenderTarget* _renderTarget)
    {
        Rect2F cameraRect = EditorLayout::CalculateWorkViewport(EditorLayout::c_previewViewport);
        Rect2F sceneRect = EditorLayout::CalculateWorkViewport(EditorLayout::c_previewViewport);

        if (m_camera3D)
            m_camera3D->setViewport(cameraRect);

        if (m_mainCanvas)
            m_mainCanvas->setViewport(sceneRect);
    }

    //////////////////////////////////////////
    void ScenePlaytestTools::processSceneStateChanged(EcsSceneState _state)
    {
        if (_state == EcsSceneState::Destroy)
        {
            if (m_camera3D == GizmosManager::GetInstancePtr()->getCamera())
                GizmosManager::GetInstancePtr()->setCamera(nullptr);

            if (m_mainCanvas == GizmosManager::GetInstancePtr()->getCanvas())
                GizmosManager::GetInstancePtr()->setCanvas(nullptr);
        }

    }


} // namespace Maze
//////////////////////////////////////////
