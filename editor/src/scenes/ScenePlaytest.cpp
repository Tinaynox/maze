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
#include "ScenePlaytest.hpp"
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
#include "maze-core/math/MazeAnimationCurve.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
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
#include "maze-graphics/MazeShaderManager.hpp"
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
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-graphics/MazeShaderUniformVariant.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-engine/ecs/components/MazeEnginePlayer.hpp"
#include "helpers/EditorProjectHelper.hpp"
#include "maze-ui/ecs/components/MazeExperimental.hpp"
#include "maze-particles/ecs/components/MazeParticleSystem3D.hpp"
#include "maze-particles/managers/MazeParticlesManager.hpp"
#include "managers/EditorEntityManager.hpp"
#include "Editor.hpp"
#include "layout/EditorLayout.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ScenePlaytestPlayer
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ScenePlaytestPlayer, ScenePlayerBase);

    //////////////////////////////////////////
    ScenePlaytestPlayer::ScenePlaytestPlayer()
    {
    }

    //////////////////////////////////////////
    ScenePlaytestPlayer::~ScenePlaytestPlayer()
    {
        for (GlobalUniformBackup const& backup : m_globalUniformBackups)
        {
            backup.uniform->setVariant(backup.value);
            backup.uniform->applyImmediate();
        }
    }

    //////////////////////////////////////////
    ScenePlaytestPlayerPtr ScenePlaytestPlayer::Create()
    {
        ScenePlaytestPlayerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ScenePlaytestPlayer, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool ScenePlaytestPlayer::init()
    {
        DataBlock playerConfig;
        Path playerConfigPath = EditorHelper::GetProjectFolder() + Path("/Player.mzcfg");
        if (FileHelper::IsFileExists(playerConfigPath))
            playerConfig.loadRegularFile(playerConfigPath);

        applyGlobalShaderUniformDefaults(playerConfig);

        ScenePlayerBaseConfig config;
        config.renderTarget = Editor::GetInstancePtr()->getMainRenderWindow();
        config.bloomMaterialName = playerConfig.getString(MAZE_HCS("bloomMaterial"));
        config.postFXMaterialName = playerConfig.getString(MAZE_HCS("postFXMaterial"));
        config.outlineMaterialName = playerConfig.getString(MAZE_HCS("outlineMaterial"));

        m_lastViewport = getMainViewport();

        if (!ScenePlayerBase::init(config))
            return false;

        m_canvas->setSortOrder(-500000);

        return true;
    }

    //////////////////////////////////////////
    void ScenePlaytestPlayer::update(F32 _dt)
    {
        Rect2F viewport = getMainViewport();
        if (viewport != m_lastViewport)
        {
            m_lastViewport = viewport;
            updateViewport(true);
        }

        ScenePlayerBase::update(_dt);

        if (!m_enginePlayer->getCurrentCamera3D() && m_renderColorSprite->getEntityRaw()->getActiveSelf())
            m_renderColorSprite->getEntityRaw()->setActiveSelf(false);
    }

    //////////////////////////////////////////
    Rect2F ScenePlaytestPlayer::getMainViewport()
    {
        return EditorLayout::CalculateWorkViewport(EditorLayout::c_sceneViewport);
    }

    //////////////////////////////////////////
    void ScenePlaytestPlayer::applyGlobalShaderUniformDefaults(DataBlock const& _playerConfig)
    {
        DataBlock const* uniformsBlock = _playerConfig.getDataBlock(MAZE_HCS("globalShaderUniforms"));
        if (!uniformsBlock)
            return;

        RenderSystem* renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();
        ShaderManagerPtr const& shaderManager = renderSystem->getShaderManager();

        for (DataBlock::DataBlockIndex i = 0; i < (DataBlock::DataBlockIndex)uniformsBlock->getDataBlocksCount(); ++i)
        {
            DataBlock const* uniformBlock = uniformsBlock->getDataBlock(i);
            if (uniformBlock->getName() != MAZE_HCS("uniform"))
                continue;

            ShaderUniformVariant variant(renderSystem);
            if (!variant.loadFromDataBlock(*uniformBlock) || variant.getName().empty())
            {
                MAZE_ERROR("Invalid global shader uniform in the player config!");
                continue;
            }

            HashedCString name = variant.getName().asHashedCString();
            GlobalShaderUniformPtr const& existingUniform = shaderManager->getGlobalShaderUniform(name);
            if (existingUniform)
                m_globalUniformBackups.push_back({ existingUniform, existingUniform->getVariant() });

            GlobalShaderUniformPtr const& uniform = shaderManager->ensureGlobalShaderUniform(name);
            if (!uniform)
                continue;

            uniform->setVariant(variant);
            uniform->applyImmediate();
        }
    }


    //////////////////////////////////////////
    // Class ScenePlaytest
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ScenePlaytest, SceneMain);

    //////////////////////////////////////////
    ScenePlaytest::ScenePlaytest()
    {


    }

    //////////////////////////////////////////
    ScenePlaytest::~ScenePlaytest()
    {
        if (m_scenePlayer)
        {
            SceneManager::GetInstancePtr()->destroyScene(m_scenePlayer);
            m_scenePlayer.reset();
        }
    }

    //////////////////////////////////////////
    ScenePlaytestPtr ScenePlaytest::Create()
    {
        ScenePlaytestPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ScenePlaytest, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool ScenePlaytest::init()
    {
        RenderWindowPtr const& renderWindow = Editor::GetInstancePtr()->getMainRenderWindow();
        if (!SceneMain::init(renderWindow))
            return false;

        makeMainScene();

        m_scenePlayer = SceneManager::GetInstancePtr()->loadScene<ScenePlaytestPlayer>(true);


        return true;
    }

    //////////////////////////////////////////
    void ScenePlaytest::update(F32 _dt)
    {
        if (!Editor::GetInstancePtr()->getRunning())
            return;

    }

    //////////////////////////////////////////
    EcsWorld* ScenePlaytest::assignWorld()
    {
        return EntityManager::GetInstancePtr()->getDefaultWorld().get();
    }


} // namespace Maze
//////////////////////////////////////////
