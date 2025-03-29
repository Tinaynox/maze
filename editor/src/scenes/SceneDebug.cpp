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
#include "SceneDebug.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
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
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "Editor.hpp"
#include "layout/EditorLayout.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneDebug
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneDebug, EcsRenderScene);

    //////////////////////////////////////////
    SceneDebug::SceneDebug()
        : m_fpsUpdateInterval(1.0f)
        , m_fpsLowest(0.0f)
        , m_fpsAcc(0.0f)
        , m_framesCount(0)
        , m_fpsTimeLeft(0.0f)
    {
        setIsSystemScene();
    }

    //////////////////////////////////////////
    SceneDebug::~SceneDebug()
    {

        InputManager* inputManager = InputManager::GetInstancePtr();
        if (inputManager)
        {
            inputManager->eventMouse.unsubscribe(this);
        }

        if (Editor::GetInstancePtr())
        {
            if (Editor::GetInstancePtr()->getMainRenderWindow())
            {
                Editor::GetInstancePtr()->getMainRenderWindow()->eventRenderTargetResized.unsubscribe(this);
            }
        }

        if (SettingsManager::GetInstancePtr())
        {
            if (m_editorSettings)
                m_editorSettings->getDebugInfoEnabledChangedEvent().unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    SceneDebugPtr SceneDebug::Create()
    {
        SceneDebugPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneDebug, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SceneDebug::init()
    {
        RenderWindowPtr const& renderWindow = Editor::GetInstancePtr()->getMainRenderWindow();
        if (!EcsRenderScene::init(renderWindow))
            return false;

        m_editorSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorSettings>();
        m_editorSettings->getDebugInfoEnabledChangedEvent().subscribe(this, &SceneDebug::notifyDebugInfoEnabledChanged);

        Editor::GetInstancePtr()->getMainRenderWindow()->eventRenderTargetResized.subscribe(this, &SceneDebug::notifyMainRenderWindowResized);

        create2D();
        updateTextRendererEnabled();

        return true;
    }

    //////////////////////////////////////////
    void SceneDebug::update(F32 _dt)
    {
        if (!Editor::GetInstancePtr()->getRunning())
            return;

        if (m_editorSettings->getDebugInfoEnabled())
        {
            m_fpsTimeLeft -= _dt;
            F32 currentFPS = _dt > 0.0f ? 1.0f / _dt : 0.0f;
            m_fpsLowest = Math::Min(m_fpsLowest, currentFPS);
            m_fpsAcc += currentFPS;

            ++m_framesCount;

            if (m_fpsTimeLeft <= 0.0f)
            {
                F32 fps = m_fpsAcc / m_framesCount;
                String fpsText;

#if (MAZE_DEBUG)
                fpsText += "DEBUG\n";
#else
                fpsText += "RELEASE\n";
#endif

                fpsText += "FPS: " + StringHelper::ToString((S32)Math::Round(fps)) + " (Min: " + StringHelper::ToString((S32)Math::Round(m_fpsLowest)) + ")\n";
                fpsText += "DIP: " + StringHelper::ToString(GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getDrawCalls()) + "\n";
                fpsText += "WE: " + StringHelper::ToString((U32)EntityManager::GetInstancePtr()->getDefaultWorldRaw()->calculateEntitiesCount());

                m_fpsTimeLeft = m_fpsUpdateInterval;
                m_fpsAcc = 0.0f;
                m_framesCount = 0;
                m_fpsLowest = currentFPS;

                m_fpsSystemTextRenderer->setText(fpsText);
            }

            GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->clearDrawCalls();
        }
    }

    //////////////////////////////////////////
    void SceneDebug::create2D()
    {
        EntityPtr canvasEntity = createEntity();
        m_canvas = canvasEntity->createComponent<Canvas>();
        m_canvas->setSortOrder(1000000);
        m_canvas->setRenderTarget(Editor::GetInstancePtr()->getMainRenderWindow());
        m_canvas->getEntityRaw()->ensureComponent<Name>("Canvas");
        Rect2F sceneViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_sceneViewport);
        m_canvas->setViewport(sceneViewport);

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        String fpsText;
        m_fpsSystemTextRenderer = EditorToolsUIHelper::CreateText(
            fpsText.c_str(),
            EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
            12,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Top,
            Vec2F32(300, 300),
            Vec2F32(15.0f, -25.0f),
            m_canvas->getTransform(),
            this,
            Vec2F32(0.0f, 1.0f),
            Vec2F32(0.0f, 1.0f));
        m_fpsSystemTextRenderer->getEntityRaw()->ensureComponent<Name>("FPS");
    }

    //////////////////////////////////////////
    void SceneDebug::notifyMainRenderWindowResized(RenderTarget* _renderTarget)
    {
        if (m_canvas)
        {
            Rect2F sceneViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_sceneViewport);
            m_canvas->setViewport(sceneViewport);
        }
    }

    //////////////////////////////////////////
    void SceneDebug::notifyDebugInfoEnabledChanged(bool const& _value)
    {
        updateTextRendererEnabled();
    }

    //////////////////////////////////////////
    void SceneDebug::updateTextRendererEnabled()
    {
        m_fpsSystemTextRenderer->getEntityRaw()->setActiveSelf(
            m_editorSettings->getDebugInfoEnabled());
    }

} // namespace Maze
//////////////////////////////////////////
