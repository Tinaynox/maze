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
#include "MazeRenderSystemDX11Header.hpp"
#include "maze-render-system-dx11/MazeRenderPassDX11.hpp"
#include "maze-render-system-dx11/MazeRenderSystemDX11.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class RenderPassDX11
    //
    //////////////////////////////////////////
    RenderPassDX11::RenderPassDX11()
    {
    }

    //////////////////////////////////////////
    RenderPassDX11::~RenderPassDX11()
    {
    }

    //////////////////////////////////////////
    RenderPassDX11Ptr RenderPassDX11::Create(
        RenderSystem* _renderSystem,
        RenderPassType _passType,
        RenderPassDeleter const& _deleter)
    {
        RenderPassDX11Ptr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR_EX(RenderPassDX11, object, _deleter, init(_renderSystem, _passType));
        return object;
    }

    //////////////////////////////////////////
    RenderPassDX11Ptr RenderPassDX11::Create(
        RenderPassDX11Ptr const& _renderPass,
        RenderPassDeleter const& _deleter)
    {
        RenderPassDX11Ptr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR_EX(RenderPassDX11, object, _deleter, init(_renderPass));
        return object;
    }

    //////////////////////////////////////////
    bool RenderPassDX11::init(
        RenderSystem* _renderSystem,
        RenderPassType _passType)
    {
        if (!RenderPass::init(_renderSystem, _passType))
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool RenderPassDX11::init(RenderPassDX11Ptr const& _renderPass)
    {
        if (!RenderPass::init(_renderPass))
            return false;

        return true;
    }

    //////////////////////////////////////////
    RenderPassPtr RenderPassDX11::createCopy()
    {
        return RenderPassDX11::Create(cast<RenderPassDX11>());
    }


} // namespace Maze
//////////////////////////////////////////
