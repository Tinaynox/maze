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
#if (!defined(_MazeGraphicsEvents_hpp_))
#define _MazeGraphicsEvents_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/events/MazeEvent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    class Texture;
    class Shader;
    class RenderBuffer;


    //////////////////////////////////////////
    // Class TextureDestroyedEvent
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API TextureDestroyedEvent
        : public GenericEvent<TextureDestroyedEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(TextureDestroyedEvent, Event);

    public:
        //////////////////////////////////////////
        TextureDestroyedEvent(
            Texture* _texture = nullptr)
            : texture(_texture)
        {}

    public:
        Texture* texture = nullptr;
    };


    //////////////////////////////////////////
    // Class ShaderDestroyedEvent
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API ShaderDestroyedEvent
        : public GenericEvent<ShaderDestroyedEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ShaderDestroyedEvent, Event);

    public:
        //////////////////////////////////////////
        ShaderDestroyedEvent(
            Shader* _shader = nullptr)
            : shader(_shader)
        {}

    public:
        Shader* shader = nullptr;
    };


    //////////////////////////////////////////
    // Class RenderBufferDestroyedEvent
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API RenderBufferDestroyedEvent
        : public GenericEvent<RenderBufferDestroyedEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(RenderBufferDestroyedEvent, Event);

    public:
        //////////////////////////////////////////
        RenderBufferDestroyedEvent(
            RenderBuffer* _renderBuffer = nullptr)
            : renderBuffer(_renderBuffer)
        {}

    public:
        RenderBuffer* renderBuffer = nullptr;
    };

    
} // namespace Maze
//////////////////////////////////////////


#endif // _MazeGraphicsEvents_hpp_
//////////////////////////////////////////