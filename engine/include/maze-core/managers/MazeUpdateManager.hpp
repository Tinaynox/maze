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
#if (!defined(_MazeUpdateManager_hpp_))
#define _MazeUpdateManager_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeTimer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(UpdateManager);


    //////////////////////////////////////////
    // Class UpdateManager
    //
    //////////////////////////////////////////
    class MAZE_CORE_API UpdateManager
        : public Updatable
        , public Updater
    {
    public:

        //////////////////////////////////////////
        virtual ~UpdateManager();

        //////////////////////////////////////////
        static void Initialize(UpdateManagerPtr& _updateManager);
        

        //////////////////////////////////////////
        void processUpdate();
    

        //////////////////////////////////////////
        inline U32 getMilliseconds() { return m_timer.getMilliseconds(); }

        //////////////////////////////////////////
        inline U32 getMicroseconds() { return m_timer.getMicroseconds(); }



        //////////////////////////////////////////
        static inline UpdateManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline UpdateManager& GetInstance() { return *s_instance; }

    protected:

        //////////////////////////////////////////
        UpdateManager();

        //////////////////////////////////////////
        virtual bool init();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

    private:
        static UpdateManager* s_instance;

        Timer m_timer;

        U32 m_lastFrameTimeMS;
        U32 m_maxDeltaTimeMS;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeUpdateManager_hpp_
//////////////////////////////////////////
