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
#if (!defined(_MazeUpdater_hpp_))
#define _MazeUpdater_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Updatable);
    MAZE_USING_SHARED_PTR(Updater);
    
    
    //////////////////////////////////////////
    // Class Updatable
    //
    //////////////////////////////////////////
    class MAZE_CORE_API Updatable
    {
        //////////////////////////////////////////
        friend class Updater;

    public:

        //////////////////////////////////////////
        virtual ~Updatable();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_ABSTRACT;

        //////////////////////////////////////////
        bool isUpdatable() const { return (m_updater != nullptr); }

    protected:

        //////////////////////////////////////////
        Updatable();

    private:
        Updater* m_updater;
    };
    
    
    //////////////////////////////////////////
    // Class Updater
    //
    //////////////////////////////////////////
    class MAZE_CORE_API Updater
    {
    public:

        //////////////////////////////////////////
        Updater();

        //////////////////////////////////////////
        virtual ~Updater();

        //////////////////////////////////////////
        void addUpdatable(Updatable* _updatable);

        //////////////////////////////////////////
        void removeUpdatable(Updatable* _updatable);

        //////////////////////////////////////////
        void ensureUpdatable(Updatable* _updatable);

    public:
        MultiDelegate<float> eventUpdate;

    protected:

        //////////////////////////////////////////
        void processUpdate(F32 _dt);

    private:
        Vector<Updatable*> m_updatables;
        bool m_updatablesDirty;
    };

    
} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#endif // _MazeUpdater_hpp_
