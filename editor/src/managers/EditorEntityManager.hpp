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
#if (!defined(_EditorEntityManager_hpp_))
#define _EditorEntityManager_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EditorEntityManager);


    //////////////////////////////////////////
    // Class EditorEntityManager
    //
    //////////////////////////////////////////
    class EditorEntityManager
        : public Updatable
    {
    public:

        //////////////////////////////////////////
        virtual ~EditorEntityManager();

        //////////////////////////////////////////
        static void Initialize(EditorEntityManagerPtr& _updateManager);


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        inline ECSWorldPtr const& getWorkspaceWorld() const { return m_workspaceWorld; }


        //////////////////////////////////////////
        static inline EditorEntityManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline EditorEntityManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        virtual S32 getUpdatableOrder() const { return 19500; }

    protected:

        //////////////////////////////////////////
        EditorEntityManager();

        //////////////////////////////////////////
        virtual bool init();

    private:
        static EditorEntityManager* s_instance;

        ECSWorldPtr m_workspaceWorld;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _EditorEntityManager_hpp_
//////////////////////////////////////////
