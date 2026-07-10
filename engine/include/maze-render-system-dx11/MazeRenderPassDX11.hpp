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
#if (!defined(_MazeRenderPassDX11_hpp_))
#define _MazeRenderPassDX11_hpp_


//////////////////////////////////////////
#include "maze-render-system-dx11/MazeRenderSystemDX11Header.hpp"
#include "maze-graphics/MazeRenderPass.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderPassDX11);
    class RenderSystemDX11;


    //////////////////////////////////////////
    // Class RenderPassDX11
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_DX11_API RenderPassDX11
        : public RenderPass
    {
    public:

        //////////////////////////////////////////
        using RenderPassDeleter = std::function<void(RenderPassDX11* _ptr)>;

    public:

        //////////////////////////////////////////
        virtual ~RenderPassDX11();

        //////////////////////////////////////////
        static RenderPassDX11Ptr Create(
            RenderSystem* _renderSystem,
            RenderPassType _passType,
            RenderPassDeleter const& _deleter = DefaultDelete<RenderPassDX11>());

        //////////////////////////////////////////
        static RenderPassDX11Ptr Create(
            RenderPassDX11Ptr const& _renderPass,
            RenderPassDeleter const& _deleter = DefaultDelete<RenderPassDX11>());

        //////////////////////////////////////////
        virtual RenderPassPtr createCopy() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        RenderPassDX11();

        //////////////////////////////////////////
        virtual bool init(
            RenderSystem* _renderSystem,
            RenderPassType _passType) MAZE_OVERRIDE;

        //////////////////////////////////////////
        bool init(RenderPassDX11Ptr const& _renderPass);
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderPassDX11_hpp_
//////////////////////////////////////////
