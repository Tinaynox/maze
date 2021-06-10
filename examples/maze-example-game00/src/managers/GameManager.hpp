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
#pragma once
#if (!defined(_GameManager_hpp_))
#define _GameManager_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "input/PlayerGamepad.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(GameManager);
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(ProjectileManager);
    MAZE_USING_SHARED_PTR(UnitManager);


    //////////////////////////////////////////
    // Class GameManager
    //
    //////////////////////////////////////////
    class GameManager
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        ~GameManager();

        //////////////////////////////////////////
        static void Initialize(GameManagerPtr& _gameManager);
        

        //////////////////////////////////////////
        static inline GameManager* GetInstancePtr(){ return s_instance; }

        //////////////////////////////////////////
        static inline GameManager& GetInstance(){ return *s_instance; }


        //////////////////////////////////////////
        void setDrawCallsMaxCount(S32 _drawCallsMaxCount) { m_drawCallsMaxCount = _drawCallsMaxCount; }

    protected:

        //////////////////////////////////////////
        GameManager();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        void notifyKeyboardEvent(InputEventKeyboardData const& _event);

        //////////////////////////////////////////
        void updateDrawCallsLimit();

        //////////////////////////////////////////
        void incDrawCallsLimit();

        //////////////////////////////////////////
        void decDrawCallsLimit();


        //////////////////////////////////////////
        void setDrawCallsLimit(S32 _drawCallsLimit);

        //////////////////////////////////////////
        inline S32 getDrawCallsLimit() const { return m_drawCallsLimit; }

    protected:
        static GameManager* s_instance;

        ProjectileManagerPtr m_projectileManager;
        UnitManagerPtr m_unitManager;

        S32 m_drawCallsLimit;
        S32 m_drawCallsMaxCount;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _GameManager_hpp_
//////////////////////////////////////////
