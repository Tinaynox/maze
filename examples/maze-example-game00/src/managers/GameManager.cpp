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
#include "GameManager.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
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
#include "maze-graphics/managers/MazeGizmosManager.hpp"
#include "maze-gamepad/managers/MazeGamepadManager.hpp"
#include "maze-gamepad/gamepad/MazeGamepad.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-debugger/settings/MazeDebuggerSettings.hpp"
#include "Game.hpp"
#include "scenes/SceneFadePreloader.hpp"
#include "scenes/SceneMainMenu.hpp"
#include "player/Player.hpp"
#include "managers/ProjectileManager.hpp"
#include "managers/UnitManager.hpp"
#include "game/SpaceObject.hpp"
#include "game/gizmos/SpaceObjectGizmos.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class GameManager
    //
    //////////////////////////////////////////
    GameManager* GameManager::s_instance = nullptr;

    //////////////////////////////////////////
    GameManager::GameManager()
        : m_drawCallsLimit(0)
        , m_drawCallsMaxCount(0)
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    GameManager::~GameManager()
    {
        s_instance = nullptr;
    

        if (InputManager::GetInstancePtr())
        {
            InputManager::GetInstancePtr()->eventKeyboard.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    void GameManager::Initialize(GameManagerPtr& _playerManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(GameManager, _playerManager, init());
    }

    //////////////////////////////////////////
    bool GameManager::init()
    {
        ProjectileManager::Initialize(m_projectileManager);
        if (!m_projectileManager)
            return false;

        UnitManager::Initialize(m_unitManager);
        if (!m_unitManager)
            return false;
        
        GizmosManager::GetInstancePtr()->registerGizmos<SpaceObject, SpaceObjectGizmos>();

        InputManager::GetInstancePtr()->eventKeyboard.subscribe(this, &GameManager::notifyKeyboardEvent);

        updateDrawCallsLimit();

        return true;
    }

    //////////////////////////////////////////
    void GameManager::updateDrawCallsLimit()
    {
        DebuggerSettingsPtr debuggerSettings = SettingsManager::GetInstancePtr()->getSettings<DebuggerSettings>();

        RenderWindowPtr const& renderWindow = Game::GetInstancePtr()->getMainRenderWindow();

        GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->setDrawCallsLimit(m_drawCallsLimit);
    }

    //////////////////////////////////////////
    void GameManager::notifyKeyboardEvent(InputEventKeyboardData const& _event)
    {
        switch (_event.type)
        {
            case InputEventKeyboardType::KeyDown:
            {
                switch (_event.keyCode)
                {
                    case KeyCode::LBracket:
                    {
                        decDrawCallsLimit();
                        break;
                    }
                    case KeyCode::RBracket:
                    {
                        incDrawCallsLimit();
                        break;
                    }

                    default:
                    {
                        break;
                    }
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
    void GameManager::incDrawCallsLimit()
    {
        if (getDrawCallsLimit() + 1 >= m_drawCallsMaxCount)
            setDrawCallsLimit(0);
        else
            setDrawCallsLimit(getDrawCallsLimit() + 1);
    }

    //////////////////////////////////////////
    void GameManager::decDrawCallsLimit()
    {
        if (getDrawCallsLimit() - 1 < 0)
            setDrawCallsLimit(Math::Max(0, m_drawCallsMaxCount - 1));
        else
            setDrawCallsLimit(getDrawCallsLimit() - 1);
    }

    //////////////////////////////////////////
    void GameManager::setDrawCallsLimit(S32 _drawCallsLimit)
    {
        if (m_drawCallsLimit == _drawCallsLimit)
            return;

        m_drawCallsLimit = _drawCallsLimit;

        updateDrawCallsLimit();
        Debug::Log("DC: %d/%d", m_drawCallsLimit, m_drawCallsMaxCount);
    }

} // namespace Maze
//////////////////////////////////////////
