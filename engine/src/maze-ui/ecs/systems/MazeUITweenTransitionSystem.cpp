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
#include "MazeUIHeader.hpp"
#include "maze-ui/ecs/systems/MazeUITweenTransitionSystem.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasRenderer.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-graphics/MazeRenderQueue.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/managers/MazeInputManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class UITweenTransitionSystem
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(UITweenTransitionSystem, ComponentSystem);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(UITweenTransitionSystem);

    //////////////////////////////////////////
    UITweenTransitionSystem::UITweenTransitionSystem()
    {
    }

    //////////////////////////////////////////
    UITweenTransitionSystem::~UITweenTransitionSystem()
    {
    }

    //////////////////////////////////////////
    UITweenTransitionSystemPtr UITweenTransitionSystem::Create()
    {
        UITweenTransitionSystemPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(UITweenTransitionSystem, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool UITweenTransitionSystem::init()
    {
            
        return true;
    }

    //////////////////////////////////////////
    void UITweenTransitionSystem::processSystemAdded()
    {
        m_UITweenTransitionAlphaSample = m_worldRaw->requestInclusiveSample<UITweenTransitionAlpha>();
        m_UITweenTransitionScaleSample = m_worldRaw->requestInclusiveSample<UITweenTransitionScale>();
        m_UITweenTransitionTranslationSample = m_worldRaw->requestInclusiveSample<UITweenTransitionTranslation>();
    }

    //////////////////////////////////////////
    void UITweenTransitionSystem::processUpdate(UpdateEvent const* _event)
    {
        MAZE_PROFILE_EVENT("UITweenTransitionSystem::processUpdate");

        F32 dt = _event->getDt();

        m_UITweenTransitionAlphaSample->process(
            [dt](Entity* _entity, UITweenTransitionAlpha* _tweenTransition)
            {
                _tweenTransition->processUpdate(dt);
            });

        m_UITweenTransitionScaleSample->process(
            [dt](Entity* _entity, UITweenTransitionScale* _tweenTransition)
            {
                _tweenTransition->processUpdate(dt);
            });

        m_UITweenTransitionTranslationSample->process(
            [dt](Entity* _entity, UITweenTransitionTranslation* _tweenTransition)
            {
                _tweenTransition->processUpdate(dt);
            });
    }
    
    
} // namespace Maze
//////////////////////////////////////////
