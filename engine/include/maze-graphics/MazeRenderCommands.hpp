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
#if (!defined(_MazeRenderCommands_hpp_))
#define _MazeRenderCommands_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderCommand.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderPass);


    //////////////////////////////////////////
    // Struct RenderCommandSetRenderPass
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API RenderCommandSetRenderPass
        : public RenderCommand
    {
    public:

        //////////////////////////////////////////
        inline RenderCommandSetRenderPass(RenderPass* _renderPass)
            : RenderCommand(RenderCommandType::SetRenderPass)
            , renderPass(_renderPass)
        {}

    public:
        RenderPass* renderPass;
    };


    //////////////////////////////////////////
    // Struct RenderCommandDrawVAOInstanced
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API RenderCommandDrawVAOInstanced
        : public RenderCommand
    {
    public:

        //////////////////////////////////////////
        inline RenderCommandDrawVAOInstanced(VertexArrayObject* _vao)
            : RenderCommand(RenderCommandType::DrawVAOInstanced)
            , vao(_vao)
            , count(1)
        {}

        //////////////////////////////////////////
        inline RenderCommandDrawVAOInstanced(
            VertexArrayObject* _vao,
            S32 _count)
            : RenderCommand(RenderCommandType::DrawVAOInstanced)
            , vao(_vao)
            , count(_count)
        {}

    public:
        VertexArrayObject* vao;
        S32 count;
        bool useColorStream = false;
        U8 uvMask = 0x0;
    };


    //////////////////////////////////////////
    // Struct RenderCommandClearCurrentRenderTarget
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API RenderCommandClearCurrentRenderTarget
        : public RenderCommand
    {
    public:

        //////////////////////////////////////////
        inline RenderCommandClearCurrentRenderTarget(
            bool _colorBuffer = true,
            bool _depthBuffer = true)
            : RenderCommand(RenderCommandType::ClearCurrentRenderTarget)
            , colorBuffer(_colorBuffer)
            , depthBuffer(_depthBuffer)
        {}

    public:
        bool colorBuffer;
        bool depthBuffer;
    };


    //////////////////////////////////////////
    // Struct RenderCommandPushScissorRect
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API RenderCommandPushScissorRect
        : public RenderCommand
    {
    public:

        //////////////////////////////////////////
        inline RenderCommandPushScissorRect(
            Rect2DF const& _scissorRect)
            : RenderCommand(RenderCommandType::PushScissorRect)
            , scissorRect(_scissorRect)
        {}

    public:
        Rect2DF scissorRect;
    };


    //////////////////////////////////////////
    // Struct RenderCommandPopScissorRect
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API RenderCommandPopScissorRect
        : public RenderCommand
    {
    public:

        //////////////////////////////////////////
        inline RenderCommandPopScissorRect()
            : RenderCommand(RenderCommandType::PopScissorRect)
        {}

    public:
    };


    //////////////////////////////////////////
    // Struct RenderCommandEnableClipPlane
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API RenderCommandEnableClipPlane
        : public RenderCommand
    {
    public:

        //////////////////////////////////////////
        inline RenderCommandEnableClipPlane(
            S32 _index,
            Vec4DF const& _plane)
            : RenderCommand(RenderCommandType::EnableClipPlane)
            , index(_index)
            , plane(_plane)
        {}

    public:
        S32 index;
        Vec4DF plane;
    };

    //////////////////////////////////////////
    // Struct RenderCommandDisableClipPlane
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API RenderCommandDisableClipPlane
        : public RenderCommand
    {
    public:

        //////////////////////////////////////////
        inline RenderCommandDisableClipPlane(
            S32 _index)
            : RenderCommand(RenderCommandType::DisableClipPlane)
            , index(_index)
        {}

    public:
        S32 index;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderCommands_hpp_
//////////////////////////////////////////
