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
#include "maze-core/ecs/MazeEcsWorld.hpp"
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
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazeSystemDialogHelper.hpp"
#include "maze-core/assets/MazeAssetDirectory.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeMeshManager.hpp"
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
#include "maze-ui/managers/MazeFontMaterialManager.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/scenes/SceneDragAndDropDefault.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-particles/managers/MazeParticlesManager.hpp"
#include "maze-plugin-archive-assets/MazeArchiveAssetsPlugin.hpp"
#include "maze-plugin-console/MazeConsolePlugin.hpp"
#include "maze-plugin-console/MazeConsoleService.hpp"
#include "maze-plugin-profiler-view/MazeProfilerViewPlugin.hpp"
#include "maze-plugin-profiler-view/MazeProfilerViewService.hpp"
#include "maze-plugin-profiler-view/settings/MazeProfilerViewSettings.hpp"
#include "maze-plugin-particles-editor-tools/MazeParticlesEditorToolsPlugin.hpp"
#include "maze-plugin-physics2d-editor-tools/MazePhysics2DEditorToolsPlugin.hpp"
#include "maze-plugin-loader-ttf/MazeLoaderTTFPlugin.hpp"
#include "maze-plugin-loader-dds/MazeLoaderDDSPlugin.hpp"
#include "maze-plugin-loader-tga/MazeLoaderTGAPlugin.hpp"
#include "maze-plugin-loader-jpg/MazeLoaderJPGPlugin.hpp"
#include "maze-plugin-loader-fbx/MazeLoaderFBXPlugin.hpp"
#include "maze-plugin-csharp/MazeCSharpPlugin.hpp"
#include "maze-plugin-csharp-editor-tools/MazeCSharpEditorToolsPlugin.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "Editor.hpp"
#include "settings/MazeEditorSettings.hpp"
#include "layout/EditorLayout.hpp"
#include "helpers/EditorProjectHelper.hpp"
#include "scenes/SceneEditor.hpp"
#include "scenes/SceneDebug.hpp"
#include "managers/EditorGizmosManager.hpp"
#include "managers/EditorManager.hpp"
#include "managers/EditorConfigManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneSplash
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneSplash, EcsRenderScene);

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
    SceneSplashPtr SceneSplash::Create()
    {
        SceneSplashPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneSplash, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SceneSplash::init()
    {
        RenderWindowPtr const& renderWindow = Editor::GetInstancePtr()->getMainRenderWindow();
        if (!EcsRenderScene::init(renderWindow))
            return false;

        Editor::GetInstancePtr()->getMainRenderWindow()->eventRenderTargetResized.subscribe(this, &SceneSplash::notifyMainRenderWindowResized);

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


        nextLoadingStep(1);

        return true;
    }

    //////////////////////////////////////////
    void SceneSplash::update(F32 _dt)
    {
        if (!Editor::GetInstancePtr()->getRunning())
            return;

    }

    //////////////////////////////////////////
    void SceneSplash::notifyMainRenderWindowResized(RenderTarget* _renderTarget)
    {
    }

    //////////////////////////////////////////
    void SceneSplash::notifyLoadingStep()
    {
        S32 delayToNextStep = 1;

        switch (m_loadingStep)
        {
            case 0:
            {
                MaterialPtr const& material = MaterialManager::GetCurrentInstance()->ensureBuiltinMaterial(BuiltinMaterialType::Sprite);
                SpriteRenderer2DPtr frame = SpriteHelper::CreateSprite(
                    ColorU32::c_lightGray,
                    Vec2F32(96.0f, 10.0f),
                    Vec2F32(0.0f, -34.0f),
                    material,
                    m_canvas->getTransform(),
                    this);

                SpriteRenderer2DPtr back = SpriteHelper::CreateSprite(
                    ColorU32::c_blackSoft,
                    frame->getTransform()->getSize() - 4.0f,
                    Vec2F32::c_zero,
                    material,
                    frame->getTransform(),
                    this);

                m_progressBarFill = SpriteHelper::CreateSprite(
                    ColorU32::c_whiteSoft,
                    Vec2F32(0.0f, back->getTransform()->getHeight()),
                    Vec2F32::c_zero,
                    material,
                    back->getTransform(),
                    this,
                    Vec2F32(0.0f, 0.5f),
                    Vec2F32(0.0f, 0.5f));

                break;
            }
            case 1:
            {
                EditorConfigManager::GetInstancePtr()->loadConfig();
                setCurrentProgress(0.15f);
                break;
            }
            case 2:
            {
                TextureManager::GetCurrentInstancePtr()->createBuiltinTextures();
                setCurrentProgress(0.25f);
                break;
            }
            case 3:
            {
                RenderSystem::GetCurrentInstancePtr()->getShaderSystem()->createBuiltinShaders();
                setCurrentProgress(0.35f);
                break;
            }
            case 4:
            {
                MaterialManager::GetCurrentInstance()->createBuiltinMaterials();
                SpriteManager::GetCurrentInstance()->setDefaultSpriteMaterial(
                    MaterialManager::GetCurrentInstance()->getBuiltinMaterial(BuiltinMaterialType::Sprite));
                setCurrentProgress(0.45f);
                break;
            }
            case 5:
            {
                // UI utils scenes
                /*
                SceneManager::GetInstancePtr()->loadScene<SceneDragAndDropDefault>(
                    true,
                    Editor::GetInstancePtr()->getMainRenderWindow());
                */

                setCurrentProgress(0.5f);
                break;
            }
            case 6:
            {
                MeshManager::GetInstancePtr()->createBuiltinMeshes();
                RenderMeshManager::GetCurrentInstancePtr()->createBuiltinRenderMeshes();
                setCurrentProgress(0.55f);
                break;
            }
            case 7:
            {
                SystemFontManager::GetCurrentInstancePtr()->createBuiltinSystemFonts();
                setCurrentProgress(0.65f);
                break;
            }
            case 8:
            {
                GizmosManager::GetInstancePtr()->createGizmosElements();
                EditorGizmosManager::GetInstancePtr()->createGizmosElements();
                setCurrentProgress(0.7f);
                break;
            }
            case 9:
            {
                UIManager::GetInstancePtr()->createUIElements();
                setCurrentProgress(0.75f);
                break;
            }
            case 10:
            {
                ParticlesManager::GetInstancePtr()->createBuiltinAssets();
                setCurrentProgress(0.8f);
                break;
            }
            case 11:
            {
                Editor::GetInstancePtr()->eventCoreEditorResourcesLoaded();

                MAZE_LOAD_PLATFORM_PLUGIN(LoaderTTF);

                EditorToolsStyles::GetInstancePtr()->loadStyles("EditorStyles.mzdata");
                FontMaterialManager::GetInstancePtr()->setDefaultFontMaterial(EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial());

                setCurrentProgress(0.85f);
                break;
            }
            case 12:
            {
                MAZE_LOAD_PLATFORM_PLUGIN(LoaderDDS);
                MAZE_LOAD_PLATFORM_PLUGIN(LoaderTGA);
                MAZE_LOAD_PLATFORM_PLUGIN(LoaderJPG);
                MAZE_LOAD_PLATFORM_PLUGIN(LoaderFBX);

                setCurrentProgress(0.9f);
                break;
            }
            case 13:
            {
                MAZE_LOAD_PLATFORM_PLUGIN(ArchiveAssets);
                MAZE_LOAD_PLATFORM_PLUGIN(ProfilerView);
                MAZE_LOAD_PLATFORM_PLUGIN(Console);
                MAZE_LOAD_PLATFORM_PLUGIN(ParticlesEditorTools);
                MAZE_LOAD_PLATFORM_PLUGIN(Physics2DEditorTools);

#if (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)
                if (SettingsManager::GetInstancePtr()->getSettingsRaw<ProfilerViewSettings>())
                    SettingsManager::GetInstancePtr()->getSettingsRaw<ProfilerViewSettings>()->setActive(true);
#endif

                ConsoleService::GetInstancePtr()->registerCommand(
                    MAZE_HCS("profile"),
                    [](String* _argv, S32 _argc)
                {
                    if (_argc > 1)
                        return false;

                    if (_argc == 0)
                        SettingsManager::GetInstancePtr()->getSettingsRaw<ProfilerViewSettings>()->switchActive();
                    else
                        SettingsManager::GetInstancePtr()->getSettingsRaw<ProfilerViewSettings>()->setActive(StringHelper::StringToBool(_argv[0]));

                    return true;
                }, 1);

                setCurrentProgress(0.95f);
                break;
            }
            case 14:
            {
                MAZE_LOAD_PLATFORM_PLUGIN(CSharp);
                MAZE_LOAD_PLATFORM_PLUGIN(CSharpEditorTools);

                setCurrentProgress(0.975f);
                break;
            }
            case 15:
            {
                AssetFilePtr const& pluginsDirectory = AssetManager::GetInstancePtr()->getAssetFile(MAZE_HASHED_CSTRING("plugins"));
                if (pluginsDirectory && pluginsDirectory->getClassUID() == ClassInfo<AssetDirectory>::UID())
                {
                    Vector<AssetFilePtr> pluginAssets = AssetManager::GetInstancePtr()->getAssetFilesInFolder(pluginsDirectory->getFullPath());
                    for (AssetFilePtr const& pluginAsset : pluginAssets)
                    {
                        Path pluginPlatformName = FileHelper::GetFileNameWithoutExtension(pluginAsset->getFileName());
                        Path pluginName = PluginManager::GetPluginNameFromPlatformName(pluginPlatformName);
                        if (!pluginName.empty())
                        {
                            PluginManager::GetInstancePtr()->loadPlugin(pluginAsset->getFullPath());
                        }
                    }
                }

                EditorManager::GetInstancePtr()->start();

                SceneManager::GetInstancePtr()->loadScene<SceneDebug>();

                setCurrentProgress(1.0f);
                delayToNextStep = 2;
                break;
            }
            case 16:
            {
                m_progressBarFill->getEntity()->setActiveSelf(false);

                EditorHelper::ValidateProjectOrSelect();

                SceneManager::GetInstancePtr()->unloadScene<SceneSplash>();
                return;
            }
        }

        nextLoadingStep(delayToNextStep);

        ++m_loadingStep;
    }

    //////////////////////////////////////////
    void SceneSplash::nextLoadingStep(S32 _delayFrames)
    {
        TaskManager::GetInstancePtr()->addDelayedMainThreadTask(
            _delayFrames,
            CreateDelegate(this, &SceneSplash::notifyLoadingStep));
    }

    //////////////////////////////////////////
    void SceneSplash::setCurrentProgress(F32 _progress)
    {
        if (m_progressBarFill)
            m_progressBarFill->getTransform()->setWidth(m_progressBarFill->getTransform()->getParent()->getWidth() * _progress);
    }

} // namespace Maze
//////////////////////////////////////////
