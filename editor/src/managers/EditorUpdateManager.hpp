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
#if (!defined(_EditorUpdateManager_hpp_))
#define _EditorUpdateManager_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-core/system/MazeTimer.hpp"
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
#include "scenes/SceneWorkspace.hpp"
#include "editor/EditorSceneMode.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EditorUpdateManager);


    //////////////////////////////////////////
    // Class EditorUpdateManager
    //
    //////////////////////////////////////////
    class EditorUpdateManager
        : public Updatable
        , public Updater
    {
    public:

        //////////////////////////////////////////
        ~EditorUpdateManager();

        //////////////////////////////////////////
        static void Initialize(EditorUpdateManagerPtr& _manager);
        


        //////////////////////////////////////////
        void processUpdate();


        //////////////////////////////////////////
        inline U32 getMilliseconds() { return m_timer.getMilliseconds(); }

        //////////////////////////////////////////
        inline U32 getMicroseconds() { return m_timer.getMicroseconds(); }


        //////////////////////////////////////////
        inline F32 getAppTime() const { return m_appTime; }

        //////////////////////////////////////////
        inline F32 getDeltaTime() const { return m_deltaTime; }


        //////////////////////////////////////////
        static inline EditorUpdateManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline EditorUpdateManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        EditorUpdateManager();

        //////////////////////////////////////////
        bool init();



    protected:
        static EditorUpdateManager* s_instance;

        Timer m_timer;

        U32 m_lastFrameTimeMS = 0u;
        U32 m_maxDeltaTimeMS = 50u;

        F32 m_appTime = 0.0f;

        F32 m_deltaTime = 0.0f;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _EditorUpdateManager_hpp_
//////////////////////////////////////////
