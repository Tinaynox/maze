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
#if (!defined(_MazePrefabInstance_hpp_))
#define _MazePrefabInstance_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/math/MazeRotation2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PrefabInstance);


    //////////////////////////////////////////
    // Class PrefabInstance
    //
    //////////////////////////////////////////
    class MAZE_CORE_API PrefabInstance
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PrefabInstance, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PrefabInstance);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~PrefabInstance();

        //////////////////////////////////////////
        static PrefabInstancePtr Create(String const& _prefab = String());


        //////////////////////////////////////////
        inline String const& getPrefabName() const { return m_prefabName; }

        //////////////////////////////////////////
        inline void setPrefabName(String const& _prefabName) { m_prefabName = _prefabName; }

    protected:

        //////////////////////////////////////////
        PrefabInstance();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(String const& _name);

    protected:
        String m_prefabName;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazePrefabInstance_hpp_
//////////////////////////////////////////
