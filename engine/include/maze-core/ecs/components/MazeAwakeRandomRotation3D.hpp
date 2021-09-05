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
#if (!defined(_MazeAwakeRandomRotation3D_hpp_))
#define _MazeAwakeRandomRotation3D_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/math/MazeMat4D.hpp"
#include "maze-core/math/MazeRotation2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(AwakeRandomRotation3D);
    MAZE_USING_SHARED_PTR(Transform3D);


    //////////////////////////////////////////
    // Class AwakeRandomRotation3D
    //
    //////////////////////////////////////////
    class MAZE_CORE_API AwakeRandomRotation3D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AwakeRandomRotation3D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(AwakeRandomRotation3D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~AwakeRandomRotation3D();

        //////////////////////////////////////////
        static AwakeRandomRotation3DPtr Create();

    protected:

        //////////////////////////////////////////
        AwakeRandomRotation3D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

    protected:
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAwakeRandomRotation3D_hpp_
//////////////////////////////////////////
