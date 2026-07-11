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
#if (!defined(_MazeInstanceStreamsVulkan_hpp_))
#define _MazeInstanceStreamsVulkan_hpp_


//////////////////////////////////////////
#include "maze-render-system-vulkan/MazeRenderSystemVulkanHeader.hpp"
#include "maze-graphics/instance-stream/MazeInstanceStreamModelMatrix.hpp"
#include "maze-graphics/instance-stream/MazeInstanceStreamColor.hpp"
#include "maze-graphics/instance-stream/MazeInstanceStreamUV.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(InstanceStreamModelMatrixVulkan);
    MAZE_USING_SHARED_PTR(InstanceStreamColorVulkan);
    MAZE_USING_SHARED_PTR(InstanceStreamUVVulkan);
    class RenderSystemVulkan;


    //////////////////////////////////////////
    // These write directly into RenderSystemVulkan's shared, frame-in-flight
    // SSBOs (set 0, bindings 1-3) instead of going through the generic
    // per-shader named-uniform path DX11/GL use (see MazeShaderVulkan.hpp
    // banner comment) - SPIRV-Reflect deliberately does not expose SSBO
    // members as named ShaderUniforms, since they're engine-internal,
    // per-draw-batch data shared by every shader rather than a per-material
    // value. The starting offset returned by
    // RenderSystemVulkan::allocateInstanceStreamSlots() must be pushed as the
    // 'u_instanceOffset' push constant for the following draw call(s) -
    // RenderQueueVulkan::draw() does this right after calling prepareForRender().
    //////////////////////////////////////////


    //////////////////////////////////////////
    // Class InstanceStreamModelMatrixVulkan
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_VULKAN_API InstanceStreamModelMatrixVulkan
        : public InstanceStreamModelMatrix
    {
    public:

        //////////////////////////////////////////
        static InstanceStreamModelMatrixVulkanPtr Create(RenderSystemVulkan* _renderSystem);

        //////////////////////////////////////////
        // Writes _instancesCount matrices (starting at this stream's own
        // getOffset() into the CPU-side m_data array) into the shared SSBO at
        // _slotOffset. _slotOffset is allocated once per draw batch by
        // RenderQueueVulkan (via RenderSystemVulkan::allocateInstanceStreamSlots)
        // and shared across the model-matrix/color/UV streams for that batch,
        // so GetModelMatrix/GetColorStream/GetUV0Stream all index consistently.
        void prepareForRender(S32 _instancesCount, S32 _slotOffset);

        //////////////////////////////////////////
        void processDrawBegin();

    protected:

        //////////////////////////////////////////
        bool init(RenderSystemVulkan* _renderSystem);

    protected:
        RenderSystemVulkan* m_renderSystem = nullptr;
    };


    //////////////////////////////////////////
    // Class InstanceStreamColorVulkan
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_VULKAN_API InstanceStreamColorVulkan
        : public InstanceStreamColor
    {
    public:

        //////////////////////////////////////////
        static InstanceStreamColorVulkanPtr Create(RenderSystemVulkan* _renderSystem);

        //////////////////////////////////////////
        void prepareForRender(S32 _instancesCount, S32 _slotOffset);

        //////////////////////////////////////////
        void processDrawBegin();

    protected:

        //////////////////////////////////////////
        bool init(RenderSystemVulkan* _renderSystem);

    protected:
        RenderSystemVulkan* m_renderSystem = nullptr;
    };


    //////////////////////////////////////////
    // Class InstanceStreamUVVulkan
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_VULKAN_API InstanceStreamUVVulkan
        : public InstanceStreamUV
    {
    public:

        //////////////////////////////////////////
        static InstanceStreamUVVulkanPtr Create(
            S32 _index,
            RenderSystemVulkan* _renderSystem);

        //////////////////////////////////////////
        void prepareForRender(S32 _instancesCount, S32 _slotOffset);

        //////////////////////////////////////////
        void processDrawBegin();

    protected:

        //////////////////////////////////////////
        bool init(
            S32 _index,
            RenderSystemVulkan* _renderSystem);

    protected:
        RenderSystemVulkan* m_renderSystem = nullptr;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeInstanceStreamsVulkan_hpp_
//////////////////////////////////////////
