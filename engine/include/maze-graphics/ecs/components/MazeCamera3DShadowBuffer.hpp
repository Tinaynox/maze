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
#if (!defined(_MazeCamera3DShadowBuffer_hpp_))
#define _MazeCamera3DShadowBuffer_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Camera3DShadowBuffer);
    MAZE_USING_SHARED_PTR(Camera3D);


    //////////////////////////////////////////
    // Class Camera3DShadowBuffer
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API Camera3DShadowBuffer
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Camera3DShadowBuffer, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Camera3DShadowBuffer);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~Camera3DShadowBuffer();

        //////////////////////////////////////////
        static Camera3DShadowBufferPtr Create();


        //////////////////////////////////////////
        inline RenderBufferPtr const& getShadowBuffer() const { return m_shadowBuffer; }

        //////////////////////////////////////////
        RenderBufferPtr const& updateShadowBuffer(Camera3D const* _camera);

    protected:

        //////////////////////////////////////////
        Camera3DShadowBuffer();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();
            

    protected:
        RenderBufferPtr m_shadowBuffer;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeCamera3DShadowBuffer_hpp_
//////////////////////////////////////////
