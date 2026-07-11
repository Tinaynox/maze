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
#if (!defined(_MazeRenderBufferVulkan_hpp_))
#define _MazeRenderBufferVulkan_hpp_


//////////////////////////////////////////
#include "maze-render-system-vulkan/MazeRenderSystemVulkanHeader.hpp"
#include "maze-graphics/MazeRenderBuffer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(RenderBufferVulkan);
    class RenderSystemVulkan;


    //////////////////////////////////////////
    // Class RenderBufferVulkan
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_VULKAN_API RenderBufferVulkan
        : public RenderBuffer
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(RenderBufferVulkan, RenderBuffer);

    public:

        //////////////////////////////////////////
        virtual ~RenderBufferVulkan();

        //////////////////////////////////////////
        static RenderBufferVulkanPtr Create(
            RenderSystemVulkan* _renderSystem,
            RenderBufferDeleter const& _deleter = DefaultDelete<RenderBuffer>());

        //////////////////////////////////////////
        static RenderBufferVulkanPtr Create(
            RenderBufferVulkanPtr const& _renderBuffer,
            RenderBufferDeleter const& _deleter = DefaultDelete<RenderBuffer>());

        //////////////////////////////////////////
        virtual RenderBufferPtr createCopy() MAZE_OVERRIDE;


        //////////////////////////////////////////
        RenderSystemVulkan* getRenderSystemVulkanRaw() const;


        //////////////////////////////////////////
        virtual bool setSize(Vec2U const& _size) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void endDraw() MAZE_OVERRIDE;


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
        RenderBufferVulkan();

        //////////////////////////////////////////
        using RenderBuffer::init;

        //////////////////////////////////////////
        bool init(RenderSystemVulkan* _renderSystem);

        //////////////////////////////////////////
        bool init(RenderBufferVulkanPtr const& _renderBuffer);

        //////////////////////////////////////////
        void resizeTexture(TexturePtr const& _texture, Vec2U const& _size);
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderBufferVulkan_hpp_
//////////////////////////////////////////
