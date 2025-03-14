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
#if (!defined(_MazeSpriteHelper_hpp_))
#define _MazeSpriteHelper_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeEcsTypes.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-graphics/MazeAlignment2D.hpp"
#include "maze-graphics/MazeColorU32.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(Material);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(SystemTextRenderer2D);
    MAZE_USING_SHARED_PTR(LineRenderer2D);
    MAZE_USING_SHARED_PTR(SimpleLineRenderer2D);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(EcsWorld);
    MAZE_USING_SHARED_PTR(EcsScene);
    MAZE_USING_SHARED_PTR(Sprite);

    //////////////////////////////////////////
    namespace SpriteHelper
    {
        //////////////////////////////////////////
        MAZE_GRAPHICS_API Transform2DPtr CreateTransform2D(
            Vec2F const& _size, 
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            EcsScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_GRAPHICS_API SpriteRenderer2DPtr CreateSprite(
            SpritePtr const& _sprite,
            Vec2F const& _size, 
            Vec2F const& _position,
            MaterialPtr const& _material,
            Transform2DPtr const& _parent,
            EcsScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_GRAPHICS_API SpriteRenderer2DPtr CreateSprite(
            CString _spriteName,
            Vec2F const& _size, 
            Vec2F const& _position,
            MaterialPtr const& _material,
            Transform2DPtr const& _parent,
            EcsScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_GRAPHICS_API SpriteRenderer2DPtr CreateSprite(
            CString _spriteName,
            Vec2F const& _size, 
            Vec2F const& _position,
            CString _materialName,
            Transform2DPtr const& _parent,
            EcsScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_FORCEINLINE SpriteRenderer2DPtr CreateSprite(
            CString _spriteName,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            EcsScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f))
        {
            return CreateSprite(
                _spriteName,
                _size,
                _position,
                nullptr,
                _parent,
                _ecsScene,
                _anchor,
                _pivot);
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API SpriteRenderer2DPtr CreateSprite(
            ColorU32 _color,
            Vec2F const& _size, 
            Vec2F const& _position,
            MaterialPtr const& _material,
            Transform2DPtr const& _parent,
            EcsScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_GRAPHICS_API LineRenderer2DPtr CreateLineRenderer(
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            EcsScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_GRAPHICS_API SimpleLineRenderer2DPtr CreateSimpleLineRenderer(
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            EcsScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f));

    } // namespace SpriteHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSpriteHelper_hpp_
//////////////////////////////////////////
