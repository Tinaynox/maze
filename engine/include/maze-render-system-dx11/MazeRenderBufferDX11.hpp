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
#if (!defined(_MazeRenderBufferDX11_hpp_))
#define _MazeRenderBufferDX11_hpp_


//////////////////////////////////////////
#include "maze-render-system-dx11/MazeRenderSystemDX11Header.hpp"
#include "maze-graphics/MazeRenderBuffer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(RenderBufferDX11);
    class RenderSystemDX11;


    //////////////////////////////////////////
    // Class RenderBufferDX11
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_DX11_API RenderBufferDX11
        : public RenderBuffer
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(RenderBufferDX11, RenderBuffer);

    public:

        //////////////////////////////////////////
        virtual ~RenderBufferDX11();

        //////////////////////////////////////////
        static RenderBufferDX11Ptr Create(
            RenderSystemDX11* _renderSystem,
            RenderBufferDeleter const& _deleter = DefaultDelete<RenderBuffer>());

        //////////////////////////////////////////
        static RenderBufferDX11Ptr Create(
            RenderBufferDX11Ptr const& _renderBuffer,
            RenderBufferDeleter const& _deleter = DefaultDelete<RenderBuffer>());

        //////////////////////////////////////////
        virtual RenderBufferPtr createCopy() MAZE_OVERRIDE;


        //////////////////////////////////////////
        RenderSystemDX11* getRenderSystemDX11Raw() const;


        //////////////////////////////////////////
        virtual bool processRenderTargetWillSet() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processRenderTargetSet() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processRenderTargetWillReset() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void blit(RenderBufferPtr const& _srcBuffer) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        RenderBufferDX11();

        //////////////////////////////////////////
        using RenderBuffer::init;

        //////////////////////////////////////////
        bool init(RenderSystemDX11* _renderSystem);

        //////////////////////////////////////////
        bool init(RenderBufferDX11Ptr const& _renderBuffer);
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderBufferDX11_hpp_
//////////////////////////////////////////
