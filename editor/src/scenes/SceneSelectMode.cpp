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
#include "SceneSelectMode.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeTaskManager.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazeSystemDialogHelper.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeSystemFontManager.hpp"
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
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-particles/managers/MazeParticlesManager.hpp"
#include "maze-plugin-console/MazeConsolePlugin.hpp"
#include "maze-plugin-console/MazeConsoleService.hpp"
#include "maze-plugin-profiler-view/MazeProfilerViewPlugin.hpp"
#include "maze-plugin-profiler-view/MazeProfilerViewService.hpp"
#include "maze-plugin-profiler-view/settings/MazeProfilerViewSettings.hpp"
#include "Editor.hpp"
#include "settings/MazeEditorSettings.hpp"
#include "layout/EditorLayout.hpp"
#include "scenes/SceneEditor.hpp"
#include "scenes/SceneDebug.hpp"
#include "helpers/EditorAssetsModeHelper.hpp"
#include "helpers/EditorProjectModeHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneSelectMode
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneSelectMode, ECSRenderScene);

    //////////////////////////////////////////
    SceneSelectMode::SceneSelectMode()
    {
    }

    //////////////////////////////////////////
    SceneSelectMode::~SceneSelectMode()
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
    }

    //////////////////////////////////////////
    SceneSelectModePtr SceneSelectMode::Create()
    {
        SceneSelectModePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneSelectMode, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SceneSelectMode::init()
    {
        RenderWindowPtr const& renderWindow = Editor::GetInstancePtr()->getMainRenderWindow();
        if (!ECSRenderScene::init(renderWindow))
            return false;

        EntityPtr canvasEntity = createEntity("Canvas");
        m_canvas = canvasEntity->createComponent<Canvas>();        
        m_canvas->setRenderTarget(Editor::GetInstancePtr()->getMainRenderWindow());
        m_canvas->setClearColorFlag(true);
        m_canvas->setClearColor(ColorU32::c_blackSoft);
        m_canvas->setSortOrder(1010000);        

        CanvasScalerPtr canvasScaler = canvasEntity->ensureComponent<CanvasScaler>();
        canvasScaler->setScaleMode(CanvasScalerScaleMode::ScaleWithViewportSize);
        canvasScaler->setScreenMatchMode(CanvasScalerScreenMatchMode::MatchWidthOrHeight);
        canvasScaler->setMatchWidthOrHeight(1.0f);
        canvasScaler->updateCanvasScale();


        MaterialPtr const& material = MaterialManager::GetCurrentInstance()->ensureBuiltinMaterial(BuiltinMaterialType::Color);
        SpriteRenderer2DPtr frame = SpriteHelper::CreateSprite(
            ColorU32::c_lightGray,
            Vec2F32(200.0f, 100.0f),
            Vec2F32(0.0f, 0.0f),
            material,
            m_canvas->getTransform(),
            this);

        VerticalLayout2DPtr layout = UIHelper::CreateVerticalLayout(
            HorizontalAlignment2D::Center,
            VerticalAlignment2D::Middle,
            frame->getTransform()->getSize(),
            Vec2F32::c_zero,
            frame->getTransform(),
            this);
        layout->setSpacing(5.0f);

        SystemTextRenderer2DPtr label = SystemUIHelper::CreateSystemText(
            "EDITOR MODE",
            8,
            HorizontalAlignment2D::Center,
            VerticalAlignment2D::Middle,
            Vec2F32(frame->getTransform()->getWidth(), 18.0f),
            Vec2F32::c_zero,
            layout->getTransform(),
            this);
        label->setSystemFont(SystemFontManager::GetCurrentInstancePtr()->getSystemFontDefaultOutlined());

        Vec2F32 const c_buttonSize = Vec2F32(100.0f, 18.0f);
        {
            ClickButton2DPtr button = UIHelper::CreateDefaultClickButton(
                "PROJECT",
                c_buttonSize,
                Vec2F32::c_zero,
                layout->getTransform(),
                this);

        }
        {
            ClickButton2DPtr button = UIHelper::CreateDefaultClickButton(
                "ASSETS",
                c_buttonSize,
                Vec2F32::c_zero,
                layout->getTransform(),
                this);
            button->eventClick.subscribe(
                [](Button2D* _button, CursorInputEvent const& _event)
                {
                    EditorSettings* editorSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorSettings>();
                    editorSettings->setEditorMode(EditorMode::Assets);

                    RenderWindowPtr const& renderWindow = Editor::GetInstancePtr()->getMainRenderWindow();
                    String path = EditorHelper::SelectAssetsFolder();
                    if (!path.empty())
                    {
                        editorSettings->setAssetsFullPath(path);

                        SettingsManager::GetInstancePtr()->saveSettings();

                        SceneManager::GetInstancePtr()->loadScene<SceneEditor>();
                        SceneManager::GetInstancePtr()->unloadScene<SceneSelectMode>();
                    }
                });
        }


        return true;
    }

    //////////////////////////////////////////
    void SceneSelectMode::update(F32 _dt)
    {
        if (!Editor::GetInstancePtr()->getRunning())
            return;

    }


} // namespace Maze
//////////////////////////////////////////
