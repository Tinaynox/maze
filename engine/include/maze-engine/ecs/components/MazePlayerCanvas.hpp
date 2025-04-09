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
#if (!defined(_MazePlayerCanvas_hpp_))
#define _MazePlayerCanvas_hpp_


//////////////////////////////////////////
#include "maze-engine/MazeEngineHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PlayerCanvas);
    MAZE_USING_SHARED_PTR(Camera3D);


    //////////////////////////////////////////
    // Class PlayerCanvas
    //
    //////////////////////////////////////////
    class MAZE_ENGINE_API PlayerCanvas
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PlayerCanvas, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PlayerCanvas);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~PlayerCanvas();

        //////////////////////////////////////////
        static PlayerCanvasPtr Create();


    protected:

        //////////////////////////////////////////
        PlayerCanvas();

        //////////////////////////////////////////
        using Component::init;

        //////////////////////////////////////////
        bool init();


    protected:
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazePlayerCanvas_hpp_
//////////////////////////////////////////
