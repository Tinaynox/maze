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
#if (!defined(_MazeSystem_hpp_))
#define _MazeSystem_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/ecs/MazeECSTypes.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(ComponentSystem);
    MAZE_USING_SHARED_PTR(ECSWorld);


    //////////////////////////////////////////
    class MAZE_CORE_API ComponentSystem
        : public SharedObject<ComponentSystem>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(ComponentSystem);

        //////////////////////////////////////////
        friend class ECSWorld;

    public:

        //////////////////////////////////////////
        virtual ~ComponentSystem();

        //////////////////////////////////////////
        void update(F32 _dt);

        //////////////////////////////////////////
        virtual S32 getOrder() const { return 0; }


        //////////////////////////////////////////
        ECSWorldPtr getWorld() const { return m_world.lock(); }

        //////////////////////////////////////////
        ECSWorld* getWorldRaw() const { return m_worldRaw; }

    protected:

        //////////////////////////////////////////
        ComponentSystem();

        //////////////////////////////////////////
        void processBeginUpdate(F32 _dt);

        //////////////////////////////////////////
        virtual void processUpdate(F32 _dt) {}

        //////////////////////////////////////////
        void processEndUpdate(F32 _dt);

        //////////////////////////////////////////
        void setWorld(ECSWorldPtr const& _world);

        //////////////////////////////////////////
        virtual void processSystemAdded();

        //////////////////////////////////////////
        virtual void processSystemRemoved();


    protected:
        ECSWorldWPtr m_world;
        ECSWorld* m_worldRaw;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSystem_hpp_
//////////////////////////////////////////
