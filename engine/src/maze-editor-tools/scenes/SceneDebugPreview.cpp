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
#include "MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/scenes/SceneDebugPreview.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
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
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-editor-tools/ecs/components/MazeHierarchyController.hpp"
#include "maze-editor-tools/ecs/components/MazeInspectorController.hpp"
#include "maze-editor-tools/ecs/components/MazeAssetsController.hpp"
#include "maze-editor-tools/ecs/components/MazeDebugGridRenderer.hpp"
#include "maze-editor-tools/ecs/systems/MazeGizmosSystem.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/systems/MazeRenderPreparationSystem.hpp"
#include "maze-ui/ecs/systems/MazeInputSystem2D.hpp"
#include "maze-ui/ecs/systems/MazeUITweenTransitionSystem.hpp"
#include "maze-ui/ecs/systems/MazeUISystem2D.hpp"
#include "maze-physics2d/ecs/systems/MazePhysicsControlSystem2D.hpp"
#include "layout/MazeEditorToolsStyles.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneDebugPreview
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneDebugPreview, ECSRenderScene);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_DEFAULT(SceneDebugPreview);

    //////////////////////////////////////////
    SceneDebugPreview::SceneDebugPreview()
        : m_yawAngle(0.0f)
        , m_pitchAngle(0.0f)
        , m_cursorPositionLastFrame(Vec2F::c_zero)
        , m_cursorDrag(false)
    {
        setIsSystemScene();
    }

    //////////////////////////////////////////
    SceneDebugPreview::~SceneDebugPreview()
    {
        InputManager* inputManager = InputManager::GetInstancePtr();
        if (inputManager)
        {
            inputManager->eventMouse.unsubscribe(this);
        }

    }

    //////////////////////////////////////////
    SceneDebugPreviewPtr SceneDebugPreview::Create(RenderTargetPtr const& _renderTarget)
    {
        SceneDebugPreviewPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneDebugPreview, object, init(_renderTarget));
        return object;
    }

    //////////////////////////////////////////
    bool SceneDebugPreview::init(RenderTargetPtr const& _renderTarget)
    {
        if (!ECSRenderScene::init(_renderTarget))
            return false;

        create3D();
        create2D();
        createSystems();

        return true;
    }

    //////////////////////////////////////////
    void SceneDebugPreview::update(F32 _dt)
    {
        _dt = UpdateManager::GetInstancePtr()->getUnscaledDeltaTime();

        m_camera3D->getTransform()->setLocalRotation(Quaternion(m_pitchAngle, m_yawAngle, 0.0f));
        m_previewWorld->update(_dt);
    }

    //////////////////////////////////////////
    void SceneDebugPreview::create3D()
    {
        // Preview node
        EntityPtr previewNodeEntity = createEntity();
        m_previewNodeTransform = previewNodeEntity->createComponent<Transform3D>();

        // Camera
        EntityPtr cameraEntity = createEntity();
        m_camera3D = cameraEntity->createComponent<Camera3D>();
        m_camera3D->getTransform()->setLocalPosition(Vec3F(3.0f, 3.0f, -5.0f));
        m_yawAngle = Math::DegreesToRadians(-30.0f);
        //m_pitchAngle = DegreesToRadians(27.0f);
        m_pitchAngle = Math::DegreesToRadians(17.0f);
        m_camera3D->getTransform()->setLocalRotation(Quaternion(m_pitchAngle, m_yawAngle, 0.0f));
        m_camera3D->setRenderMask(0xFFFFFFFF);

        m_camera3D->setRenderTarget(m_renderTarget);
        m_camera3D->setClearColor(ColorU32(49, 49, 49, 255));
        m_camera3D->setClearColorFlag(true);
        m_camera3D->setClearDepthFlag(true);
        m_camera3D->setFOV(Maze::Math::DegreesToRadians(60));
        cameraEntity->setActiveSelf(false);
            
    }

    //////////////////////////////////////////
    void SceneDebugPreview::create2D()
    {
        EntityPtr canvasEntity = createEntity();
        m_canvas = canvasEntity->createComponent<Canvas>();
        m_canvas->setClearColorFlag(true);
        m_canvas->setClearColor(ColorU32::c_lightGray);
        m_canvas->setRenderTarget(m_renderTarget);
        canvasEntity->setActiveSelf(false);
    }

    //////////////////////////////////////////
    void SceneDebugPreview::clear()
    {
        if (getPreviewNodeTransform())
            getPreviewNodeTransform()->destroyAllChildren();

        if (getCanvas())
            getCanvas()->getTransform()->destroyAllChildren();
    }

    //////////////////////////////////////////
    void SceneDebugPreview::createSystems()
    {        
        m_previewWorld->addSystem(RenderPreparationSystem::Create(GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()));
        m_previewWorld->addSystem(RenderControlSystem::Create(GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()));
        m_previewWorld->addSystem(UISystem2D::Create());
        m_previewWorld->addSystem(InputSystem2D::Create());

        m_previewWorld->addSystem(UITweenTransitionSystem::Create());
    }

    //////////////////////////////////////////
    ECSWorld* SceneDebugPreview::assignWorld()
    {
        m_previewWorld = ECSWorld::Create();
        return m_previewWorld.get();
    }

} // namespace Maze
//////////////////////////////////////////
